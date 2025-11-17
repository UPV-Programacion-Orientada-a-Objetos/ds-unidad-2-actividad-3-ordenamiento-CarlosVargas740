/**
 * @file main.cpp
 * @brief Programa principal del sistema E-Sort
 */

#include "DataSource.h"
#include "SerialSource.h"
#include "FileSource.h"
#include "CircularBuffer.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/stat.h>

void generarNombreChunk(char* buffer, int numero) {
    sprintf(buffer, "chunk_%d.tmp", numero);
}

// Detectar puerto Arduino disponible
const char* detectarPuerto() {
    const char* puertos[] = {
        "/dev/ttyACM0", "/dev/ttyACM1", "/dev/ttyACM2",
        "/dev/ttyUSB0", "/dev/ttyUSB1", "/dev/ttyUSB2",
        "/dev/cu.usbmodem", "/dev/cu.usbserial"
    };
    
    struct stat buffer;
    for (int i = 0; i < 8; i++) {
        if (stat(puertos[i], &buffer) == 0) {
            return puertos[i];
        }
    }
    return nullptr;
}

int capturarDatos(const char* puerto, int buffer_size, int max_lecturas) {
    SerialSource* serial = new SerialSource(puerto, max_lecturas);
    
    if (!serial->isConnected()) {
        printf("No se pudo abrir el puerto\n");
        delete serial;
        return 0;
    }
    
    CircularBuffer buffer(buffer_size);
    int num_chunks = 0;
    int total = 0;
    
    printf("Recibiendo datos (buffer: %d)...\n\n", buffer_size);
    
    while (serial->hasMoreData()) {
        int valor = serial->getNext();
        
        if (!serial->hasMoreData() && valor == 0) {
            break;
        }
        
        printf("%d ", valor);
        if ((total + 1) % 10 == 0) printf("\n");
        total++;
        
        if (!buffer.insertar(valor)) {
            char nombre[64];
            generarNombreChunk(nombre, num_chunks);
            buffer.ordenarYVolcar(nombre);
            num_chunks++;
            buffer.vaciar();
            buffer.insertar(valor);
        }
    }
    
    if (!buffer.estaVacio()) {
        char nombre[64];
        generarNombreChunk(nombre, num_chunks);
        buffer.ordenarYVolcar(nombre);
        num_chunks++;
        buffer.vaciar();
    }
    
    delete serial;
    
    printf("\n\nDatos recibidos: %d\n", total);
    printf("Archivos temporales: %d\n\n", num_chunks);
    
    return num_chunks;
}

bool fusionarArchivos(int num_chunks, const char* salida_final) {
    if (num_chunks == 0) {
        return false;
    }
    
    printf("Fusionando archivos...\n");
    
    DataSource** fuentes = new DataSource*[num_chunks];
    int* valores = new int[num_chunks];
    bool* activos = new bool[num_chunks];
    
    for (int i = 0; i < num_chunks; i++) {
        char nombre[64];
        generarNombreChunk(nombre, i);
        fuentes[i] = new FileSource(nombre);
        
        if (!((FileSource*)fuentes[i])->isOpen()) {
            for (int j = 0; j <= i; j++) {
                delete fuentes[j];
            }
            delete[] fuentes;
            delete[] valores;
            delete[] activos;
            return false;
        }
        
        if (fuentes[i]->hasMoreData()) {
            valores[i] = fuentes[i]->getNext();
            activos[i] = fuentes[i]->hasMoreData();
        } else {
            activos[i] = false;
        }
    }
    
    FILE* salida = fopen(salida_final, "w");
    if (salida == nullptr) {
        for (int i = 0; i < num_chunks; i++) {
            delete fuentes[i];
        }
        delete[] fuentes;
        delete[] valores;
        delete[] activos;
        return false;
    }
    
    int escritos = 0;
    
    while (true) {
        int idx_min = -1;
        int val_min = 0;
        
        for (int i = 0; i < num_chunks; i++) {
            if (activos[i] || fuentes[i]->hasMoreData()) {
                if (idx_min == -1 || valores[i] < val_min) {
                    idx_min = i;
                    val_min = valores[i];
                }
            }
        }
        
        if (idx_min == -1) {
            break;
        }
        
        fprintf(salida, "%d\n", val_min);
        escritos++;
        
        if (fuentes[idx_min]->hasMoreData()) {
            valores[idx_min] = fuentes[idx_min]->getNext();
            activos[idx_min] = fuentes[idx_min]->hasMoreData();
        } else {
            activos[idx_min] = false;
        }
    }
    
    fclose(salida);
    
    for (int i = 0; i < num_chunks; i++) {
        delete fuentes[i];
    }
    delete[] fuentes;
    delete[] valores;
    delete[] activos;
    
    printf("Elementos ordenados: %d\n", escritos);
    printf("Resultado: %s\n\n", salida_final);
    
    return true;
}

int main(int argc, char* argv[]) {
    printf("E-Sort - Ordenamiento externo\n");
    printf("================================\n\n");
    
    const char* puerto = nullptr;
    int buffer_size = 100;
    int max_lecturas = 0;
    
    // Detectar puerto automáticamente si no se especifica
    if (argc > 1) {
        puerto = argv[1];
    } else {
        printf("Buscando Arduino...\n");
        puerto = detectarPuerto();
        if (puerto == nullptr) {
            printf("No se encontró ningún puerto disponible\n");
            printf("Prueba: ./esort /dev/ttyACM0\n");
            return 1;
        }
        printf("Encontrado: %s\n\n", puerto);
    }
    
    if (argc > 2) {
        buffer_size = atoi(argv[2]);
    }
    if (argc > 3) {
        max_lecturas = atoi(argv[3]);
    }
    
    // Capturar datos
    int num_chunks = capturarDatos(puerto, buffer_size, max_lecturas);
    
    if (num_chunks == 0) {
        printf("No se recibieron datos\n");
        return 1;
    }
    
    // Fusionar
    const char* salida = "output.sorted.txt";
    if (!fusionarArchivos(num_chunks, salida)) {
        printf("Error al fusionar archivos\n");
        return 1;
    }
    
    printf("Listo!\n");
    
    return 0;
}
