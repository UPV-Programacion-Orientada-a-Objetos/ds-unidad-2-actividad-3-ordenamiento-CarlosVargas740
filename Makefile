# Makefile simple para E-Sort (alternativa a CMake)

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude
TARGET = esort
SRC_DIR = src
OBJ_DIR = build

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(OBJ_DIR) $(TARGET)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(OBJ_DIR)/$(TARGET)
	@echo ""
	@echo "✅ Compilación exitosa!"
	@echo "Ejecutable: $(OBJ_DIR)/$(TARGET)"
	@echo ""
	@echo "Uso: ./$(OBJ_DIR)/$(TARGET) [puerto] [buffer_size] [max_lecturas]"
	@echo "Ejemplo: ./$(OBJ_DIR)/$(TARGET) /dev/ttyACM0 100 500"
	@echo ""

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o $(OBJ_DIR)/$(TARGET) chunk_*.tmp output.sorted.txt
	@echo "Limpieza completada"

.PHONY: all clean
