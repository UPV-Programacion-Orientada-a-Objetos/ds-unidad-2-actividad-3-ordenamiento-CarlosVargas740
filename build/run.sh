#!/bin/bash
# Script para ejecutar E-Sort con configuración recomendada

# Colores
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  E-Sort - Script de Ejecución${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

# Verificar si existe el ejecutable
if [ ! -f "./esort" ]; then
    echo -e "${RED}Error: No se encontró el ejecutable 'esort'${NC}"
    echo "Primero compila el proyecto:"
    echo "  cd build && cmake .. && make"
    exit 1
fi

# Detectar puerto Arduino
PUERTO=""
if [ -e "/dev/ttyACM0" ]; then
    PUERTO="/dev/ttyACM0"
elif [ -e "/dev/ttyACM1" ]; then
    PUERTO="/dev/ttyACM1"
elif [ -e "/dev/ttyUSB0" ]; then
    PUERTO="/dev/ttyUSB0"
elif [ -e "/dev/ttyUSB1" ]; then
    PUERTO="/dev/ttyUSB1"
else
    echo -e "${RED}No se detectó Arduino conectado${NC}"
    echo "Puertos disponibles:"
    ls /dev/tty* 2>/dev/null | grep -E "(ACM|USB)" || echo "  Ninguno"
    echo ""
    read -p "Ingresa el puerto manualmente (ej: /dev/ttyACM0): " PUERTO
fi

echo -e "${GREEN}Puerto detectado: $PUERTO${NC}"
echo ""

# Preguntar configuración
echo "Configuración:"
echo "1. Prueba rápida (buffer=10, lecturas=50)"
echo "2. Prueba media (buffer=50, lecturas=200)"
echo "3. Prueba completa (buffer=100, lecturas=500)"
echo "4. Modo continuo (buffer=100, lecturas=infinito)"
echo "5. Personalizado"
echo ""
read -p "Selecciona opción [1-5]: " OPCION

case $OPCION in
    1)
        BUFFER=10
        LECTURAS=50
        ;;
    2)
        BUFFER=50
        LECTURAS=200
        ;;
    3)
        BUFFER=100
        LECTURAS=500
        ;;
    4)
        BUFFER=100
        LECTURAS=0
        ;;
    5)
        read -p "Tamaño del buffer: " BUFFER
        read -p "Máximo de lecturas (0=infinito): " LECTURAS
        ;;
    *)
        echo -e "${RED}Opción inválida${NC}"
        exit 1
        ;;
esac

echo ""
echo -e "${GREEN}Iniciando E-Sort...${NC}"
echo "  Puerto: $PUERTO"
echo "  Buffer: $BUFFER elementos"
echo "  Lecturas: $([ $LECTURAS -eq 0 ] && echo 'infinito' || echo $LECTURAS)"
echo ""
echo -e "${BLUE}Presiona Ctrl+C para detener${NC}"
echo ""

# Ejecutar
./esort "$PUERTO" "$BUFFER" "$LECTURAS"

# Mostrar resultados
if [ -f "output.sorted.txt" ]; then
    echo ""
    echo -e "${GREEN}========================================${NC}"
    echo -e "${GREEN}  Resultado${NC}"
    echo -e "${GREEN}========================================${NC}"
    echo ""
    echo "Archivo generado: output.sorted.txt"
    echo "Elementos ordenados: $(wc -l < output.sorted.txt)"
    echo ""
    echo "Primeros 10 valores:"
    head -10 output.sorted.txt
    echo ""
    echo "Últimos 10 valores:"
    tail -10 output.sorted.txt
fi
