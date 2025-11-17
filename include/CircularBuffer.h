/**
 * @file CircularBuffer.h
 * @brief Lista circular doblemente enlazada para buffer de datos
 */

#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

/**
 * @struct Nodo
 * @brief Nodo de la lista circular doblemente enlazada
 */
struct Nodo {
    int dato;           // Valor almacenado
    Nodo* siguiente;    // Puntero al siguiente nodo
    Nodo* previo;       // Puntero al nodo anterior
    
    /**
     * @brief Constructor del nodo
     * @param valor Valor a almacenar
     */
    Nodo(int valor) : dato(valor), siguiente(nullptr), previo(nullptr) {}
};

/**
 * @class CircularBuffer
 * @brief Buffer circular de tamaño fijo implementado con lista doblemente enlazada
 * 
 * Este buffer almacena datos en una lista circular y cuando se llena,
 * permite ordenar su contenido y volcarlo a un archivo.
 */
class CircularBuffer {
private:
    Nodo* cabeza;           // Puntero a la cabeza de la lista
    int capacidad;          // Capacidad máxima del buffer
    int tamano_actual;      // Número de elementos actuales
    
    /**
     * @brief Ordena los datos del buffer usando Insertion Sort
     */
    void ordenarInternamente();
    
    /**
     * @brief Intercambia los datos de dos nodos
     * @param a Primer nodo
     * @param b Segundo nodo
     */
    void intercambiar(Nodo* a, Nodo* b);
    
public:
    /**
     * @brief Constructor que inicializa el buffer con capacidad fija
     * @param cap Capacidad del buffer
     */
    CircularBuffer(int cap);
    
    /**
     * @brief Destructor que libera toda la memoria
     */
    ~CircularBuffer();
    
    /**
     * @brief Inserta un dato en el buffer
     * @param valor Valor a insertar
     * @return true si se insertó correctamente, false si está lleno
     */
    bool insertar(int valor);
    
    /**
     * @brief Verifica si el buffer está lleno
     * @return true si está lleno
     */
    bool estaLleno() const { return tamano_actual >= capacidad; }
    
    /**
     * @brief Verifica si el buffer está vacío
     * @return true si está vacío
     */
    bool estaVacio() const { return tamano_actual == 0; }
    
    /**
     * @brief Obtiene el tamaño actual del buffer
     * @return Número de elementos en el buffer
     */
    int getTamano() const { return tamano_actual; }
    
    /**
     * @brief Ordena el buffer y escribe su contenido en un archivo
     * @param nombre_archivo Nombre del archivo donde escribir
     * @return true si se escribió correctamente
     */
    bool ordenarYVolcar(const char* nombre_archivo);
    
    /**
     * @brief Vacía completamente el buffer
     */
    void vaciar();
    
    /**
     * @brief Muestra el contenido del buffer (para debug)
     */
    void mostrar() const;
};

#endif // CIRCULARBUFFER_H
