/**
 * @file SerialSource.cpp
 * @brief Implementación de la clase SerialSource
 */

#include "SerialSource.h"
#include <fcntl.h>      // Para open()
#include <unistd.h>     // Para read(), close()
#include <termios.h>    // Para configuración serial
#include <cstring>      // Para memset, strlen
#include <cstdio>       // Para printf

SerialSource::SerialSource(const char* port_name, int max_reads) 
    : fd(-1), buffer_pos(0), is_connected(false), 
      max_readings(max_reads), readings_count(0) {
    
    // Abrir el puerto serial
    fd = open(port_name, O_RDWR | O_NOCTTY);
    
    if (fd < 0) {
        printf("Error: No se pudo abrir el puerto %s\n", port_name);
        return;
    }
    
    // Configurar el puerto serial
    struct termios tty;
    memset(&tty, 0, sizeof(tty));
    
    if (tcgetattr(fd, &tty) != 0) {
        printf("Error al obtener atributos del puerto\n");
        close(fd);
        fd = -1;
        return;
    }
    
    // Configurar velocidad: 9600 baudios
    cfsetospeed(&tty, B9600);
    cfsetispeed(&tty, B9600);
    
    // Configuración 8N1 (8 bits, sin paridad, 1 bit de parada)
    tty.c_cflag &= ~PARENB;        // Sin paridad
    tty.c_cflag &= ~CSTOPB;        // 1 bit de parada
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;            // 8 bits por byte
    tty.c_cflag &= ~CRTSCTS;       // Sin control de flujo hardware
    tty.c_cflag |= CREAD | CLOCAL; // Activar lectura
    
    // Modo no canónico (sin procesamiento de líneas)
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ECHOE;
    tty.c_lflag &= ~ECHONL;
    tty.c_lflag &= ~ISIG;
    
    // Configuración de entrada
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
    
    // Configuración de salida
    tty.c_oflag &= ~OPOST;
    tty.c_oflag &= ~ONLCR;
    
    // Timeouts
    tty.c_cc[VTIME] = 10;  // 1 segundo de timeout
    tty.c_cc[VMIN] = 0;
    
    // Aplicar configuración
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error al configurar el puerto\n");
        close(fd);
        fd = -1;
        return;
    }
    
    // Limpiar el buffer
    tcflush(fd, TCIOFLUSH);
    
    // Esperar a que el Arduino se reinicie
    printf("Conectando...\n");
    sleep(2);
    
    is_connected = true;
    printf("Puerto %s abierto\n", port_name);
}

SerialSource::~SerialSource() {
    if (fd >= 0) {
        close(fd);
    }
}

bool SerialSource::readLine(char* line, int max_len) {
    int pos = 0;
    char c;
    
    while (pos < max_len - 1) {
        int n = read(fd, &c, 1);
        
        if (n <= 0) {
            // Timeout o error
            if (pos > 0) {
                line[pos] = '\0';
                return true;
            }
            return false;
        }
        
        if (c == '\n') {
            line[pos] = '\0';
            return true;
        }
        
        if (c != '\r') {  // Ignorar retorno de carro
            line[pos++] = c;
        }
    }
    
    line[pos] = '\0';
    return true;
}

int SerialSource::getNext() {
    if (!is_connected) {
        return 0;
    }
    
    char line[256];
    
    while (readLine(line, sizeof(line))) {
        // Convertir la línea a entero
        int valor = 0;
        bool es_numero = false;
        
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] >= '0' && line[i] <= '9') {
                valor = valor * 10 + (line[i] - '0');
                es_numero = true;
            } else if (line[i] != ' ' && line[i] != '\t') {
                // Carácter inválido
                break;
            }
        }
        
        if (es_numero) {
            readings_count++;
            return valor;
        }
    }
    
    // Si no se pudo leer, marcar como desconectado
    is_connected = false;
    return 0;
}

bool SerialSource::hasMoreData() {
    if (!is_connected) {
        return false;
    }
    
    // Si hay límite de lecturas, verificarlo
    if (max_readings > 0 && readings_count >= max_readings) {
        return false;
    }
    
    return true;
}
