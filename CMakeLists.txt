cmake_minimum_required(VERSION 3.10)
project(E-Sort VERSION 1.0)

# Configurar C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Directorios de inclusión
include_directories(${PROJECT_SOURCE_DIR}/include)

# Archivos fuente
set(SOURCES
    src/main.cpp
    src/SerialSource.cpp
    src/FileSource.cpp
    src/CircularBuffer.cpp
)

# Ejecutable
add_executable(esort ${SOURCES})

# Mensaje de ayuda
message(STATUS "")
message(STATUS "========================================")
message(STATUS "  Sistema E-Sort - Configuración CMake")
message(STATUS "========================================")
message(STATUS "")
message(STATUS "Para compilar:")
message(STATUS "  cd build")
message(STATUS "  cmake ..")
message(STATUS "  make")
message(STATUS "")
message(STATUS "Para ejecutar:")
message(STATUS "  ./esort [puerto] [buffer_size] [max_lecturas]")
message(STATUS "")
message(STATUS "Ejemplo:")
message(STATUS "  ./esort /dev/ttyACM0 100 500")
message(STATUS "")
message(STATUS "========================================")
message(STATUS "")
