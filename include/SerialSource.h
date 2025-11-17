/**
 * @file SerialSource.h
 * @brief Implementación de DataSource para puerto serial
 */

#ifndef SERIALSOURCE_H
#define SERIALSOURCE_H

#include "DataSource.h"

/**
 * @class SerialSource
 * @brief Lee datos enteros desde un puerto serial (Arduino)
 */
class SerialSource : public DataSource {
private:
    int fd;                    // File descriptor del puerto serial
    char buffer[256];          // Buffer para lectura de líneas
    int buffer_pos;            // Posición actual en el buffer
    bool is_connected;         // Estado de conexión
    int max_readings;          // Número máximo de lecturas (0 = infinito)
    int readings_count;        // Contador de lecturas realizadas
    
    /**
     * @brief Lee una línea completa del puerto serial
     * @param line Buffer donde almacenar la línea
     * @param max_len Tamaño máximo del buffer
     * @return true si se leyó una línea, false si hay error
     */
    bool readLine(char* line, int max_len);
    
public:
    /**
     * @brief Constructor que abre y configura el puerto serial
     * @param port_name Nombre del puerto (ej: "/dev/ttyACM0")
     * @param max_reads Número máximo de lecturas (0 = infinito)
     */
    SerialSource(const char* port_name, int max_reads = 0);
    
    /**
     * @brief Destructor que cierra el puerto serial
     */
    ~SerialSource();
    
    /**
     * @brief Obtiene el siguiente entero del serial
     * @return Entero leído del Arduino
     */
    int getNext();
    
    /**
     * @brief Verifica si hay más datos disponibles
     * @return true si el puerto está conectado y no se alcanzó el límite
     */
    bool hasMoreData();
    
    /**
     * @brief Verifica si la conexión está activa
     * @return true si está conectado
     */
    bool isConnected() const { return is_connected; }
};

#endif // SERIALSOURCE_H
