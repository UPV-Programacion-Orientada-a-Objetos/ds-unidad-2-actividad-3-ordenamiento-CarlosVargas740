/**
 * @file FileSource.h
 * @brief Implementación de DataSource para archivos
 */

#ifndef FILESOURCE_H
#define FILESOURCE_H

#include "DataSource.h"
#include <cstdio>

/**
 * @class FileSource
 * @brief Lee datos enteros desde un archivo
 */
class FileSource : public DataSource {
private:
    FILE* file;           // Puntero al archivo
    bool has_more;        // Indica si hay más datos
    int next_value;       // Siguiente valor pre-leído
    
    /**
     * @brief Lee el siguiente valor del archivo
     * @return true si se leyó correctamente
     */
    bool readNextValue();
    
public:
    /**
     * @brief Constructor que abre el archivo
     * @param filename Nombre del archivo a abrir
     */
    FileSource(const char* filename);
    
    /**
     * @brief Destructor que cierra el archivo
     */
    ~FileSource();
    
    /**
     * @brief Obtiene el siguiente entero del archivo
     * @return Entero leído
     */
    int getNext();
    
    /**
     * @brief Verifica si hay más datos en el archivo
     * @return true si hay más datos
     */
    bool hasMoreData();
    
    /**
     * @brief Verifica si el archivo se abrió correctamente
     * @return true si está abierto
     */
    bool isOpen() const { return file != nullptr; }
};

#endif // FILESOURCE_H
