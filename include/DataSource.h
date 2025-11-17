/**
 * @file DataSource.h
 * @brief Clase base abstracta para fuentes de datos
 * 
 * Define la interfaz que deben implementar todas las fuentes de datos
 * (serial, archivos, etc.) para el sistema E-Sort.
 */

#ifndef DATASOURCE_H
#define DATASOURCE_H

/**
 * @class DataSource
 * @brief Clase abstracta que representa una fuente de datos
 * 
 * Esta clase define la interfaz para leer datos de diferentes fuentes
 * como puertos seriales o archivos.
 */
class DataSource {
public:
    /**
     * @brief Destructor virtual para permitir polimorfismo
     */
    virtual ~DataSource() {}
    
    /**
     * @brief Obtiene el siguiente entero de la fuente
     * @return El siguiente entero disponible
     */
    virtual int getNext() = 0;
    
    /**
     * @brief Verifica si hay más datos disponibles
     * @return true si hay más datos, false en caso contrario
     */
    virtual bool hasMoreData() = 0;
};

#endif // DATASOURCE_H
