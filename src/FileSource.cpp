/**
 * @file FileSource.cpp
 * @brief Implementación de la clase FileSource
 */

#include "FileSource.h"
#include <cstdio>

FileSource::FileSource(const char* filename) : file(nullptr), has_more(false), next_value(0) {
    file = fopen(filename, "r");
    
    if (file == nullptr) {
        printf("Error: No se pudo abrir el archivo %s\n", filename);
        return;
    }
    
    // Pre-leer el primer valor
    has_more = readNextValue();
}

FileSource::~FileSource() {
    if (file != nullptr) {
        fclose(file);
    }
}

bool FileSource::readNextValue() {
    if (file == nullptr) {
        return false;
    }
    
    int resultado = fscanf(file, "%d", &next_value);
    
    if (resultado == 1) {
        return true;
    }
    
    return false;
}

int FileSource::getNext() {
    int valor_actual = next_value;
    has_more = readNextValue();
    return valor_actual;
}

bool FileSource::hasMoreData() {
    return has_more;
}
