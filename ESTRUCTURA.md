# Estructura del Proyecto E-Sort

```
e-sort/
├── arduino/
│   └── test.ino                 # Sketch para Arduino
├── include/
│   ├── DataSource.h             # Clase base abstracta
│   ├── SerialSource.h           # Lee del puerto serial
│   ├── FileSource.h             # Lee de archivos
│   └── CircularBuffer.h         # Lista circular
├── src/
│   ├── main.cpp                 # Programa principal
│   ├── SerialSource.cpp         # Implementación serial
│   ├── FileSource.cpp           # Implementación archivo
│   └── CircularBuffer.cpp       # Implementación buffer
├── build/
│   └── esort                    # Ejecutable (después de compilar)
├── CMakeLists.txt               # Configuración CMake
├── Makefile                     # Makefile alternativo
├── README.md                    # Documentación completa
├── QUICKSTART.md                # Guía rápida
└── run.sh                       # Script de ejecución
```

## Archivos Clave

### 🔧 Clases Principales

- **DataSource.h**: Interfaz abstracta con `getNext()` y `hasMoreData()`
- **SerialSource**: Lee enteros del Arduino por puerto serial
- **FileSource**: Lee enteros de archivos `.tmp`
- **CircularBuffer**: Lista circular de tamaño fijo con ordenamiento

### 📱 Arduino

- **test.ino**: Genera números aleatorios cada 50ms

### 🚀 Ejecución

- **run.sh**: Script interactivo con menú de configuración
- **esort**: Ejecutable principal

## Compilación

### Opción 1: CMake (Recomendado)

```bash
cd build
cmake ..
make
```

### Opción 2: Makefile

```bash
make
```

## Uso

### Modo Directo

```bash
./esort [puerto] [buffer_size] [max_lecturas]
```

## Salidas

- `chunk_X.tmp` → Archivos temporales ordenados
- `output.sorted.txt` → **Resultado final ordenado**
