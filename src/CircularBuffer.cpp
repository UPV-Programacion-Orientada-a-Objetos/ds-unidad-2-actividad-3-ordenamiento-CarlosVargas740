/**
 * @file CircularBuffer.cpp
 * @brief Implementación de la clase CircularBuffer
 */

#include "CircularBuffer.h"
#include <cstdio>

CircularBuffer::CircularBuffer(int cap) 
    : cabeza(nullptr), capacidad(cap), tamano_actual(0) {
}

CircularBuffer::~CircularBuffer() {
    vaciar();
}

bool CircularBuffer::insertar(int valor) {
    if (estaLleno()) {
        return false;
    }
    
    Nodo* nuevo = new Nodo(valor);
    
    if (cabeza == nullptr) {
        // Primer nodo: apunta a sí mismo
        cabeza = nuevo;
        nuevo->siguiente = nuevo;
        nuevo->previo = nuevo;
    } else {
        // Insertar al final (antes de la cabeza)
        Nodo* cola = cabeza->previo;
        
        nuevo->siguiente = cabeza;
        nuevo->previo = cola;
        cola->siguiente = nuevo;
        cabeza->previo = nuevo;
    }
    
    tamano_actual++;
    return true;
}

void CircularBuffer::intercambiar(Nodo* a, Nodo* b) {
    int temp = a->dato;
    a->dato = b->dato;
    b->dato = temp;
}

void CircularBuffer::ordenarInternamente() {
    if (cabeza == nullptr || tamano_actual <= 1) {
        return;
    }
    
    // Insertion Sort sobre la lista circular
    Nodo* actual = cabeza->siguiente;
    
    for (int i = 1; i < tamano_actual; i++) {
        Nodo* clave = actual;
        int valor_clave = clave->dato;
        Nodo* j = clave->previo;
        
        // Mover elementos mayores hacia adelante
        int comparaciones = i;
        while (comparaciones > 0 && j->dato > valor_clave) {
            j->siguiente->dato = j->dato;
            j = j->previo;
            comparaciones--;
        }
        
        j->siguiente->dato = valor_clave;
        actual = actual->siguiente;
    }
}

bool CircularBuffer::ordenarYVolcar(const char* nombre_archivo) {
    if (estaVacio()) {
        return false;
    }
    
    // Ordenar el contenido
    ordenarInternamente();
    
    // Escribir al archivo
    FILE* archivo = fopen(nombre_archivo, "w");
    if (archivo == nullptr) {
        printf("Error: No se pudo crear el archivo %s\n", nombre_archivo);
        return false;
    }
    
    Nodo* actual = cabeza;
    for (int i = 0; i < tamano_actual; i++) {
        fprintf(archivo, "%d\n", actual->dato);
        actual = actual->siguiente;
    }
    
    fclose(archivo);
    printf("Guardado: %s\n", nombre_archivo);
    
    return true;
}

void CircularBuffer::vaciar() {
    if (cabeza == nullptr) {
        return;
    }
    
    // Romper el círculo
    Nodo* cola = cabeza->previo;
    cola->siguiente = nullptr;
    
    // Eliminar todos los nodos
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        Nodo* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
    
    cabeza = nullptr;
    tamano_actual = 0;
}

void CircularBuffer::mostrar() const {
    if (cabeza == nullptr) {
        printf("Buffer vacío\n");
        return;
    }
    
    printf("Buffer [%d/%d]: ", tamano_actual, capacidad);
    Nodo* actual = cabeza;
    for (int i = 0; i < tamano_actual; i++) {
        printf("%d ", actual->dato);
        actual = actual->siguiente;
    }
    printf("\n");
}
