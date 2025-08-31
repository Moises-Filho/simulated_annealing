# Makefile para o TSP com Simulated Annealing

# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

# Diretórios
SRC_DIR = ../src
INCLUDE_DIR = ../include
OUTPUT_DIR = ../output

# Arquivos
SRC = main.cpp \
      $(SRC_DIR)/SAReaquecimento.cpp \
      $(SRC_DIR)/SimulatedAnnealing.cpp \
      $(SRC_DIR)/FuncoesAuxiliares.cpp \
      $(SRC_DIR)/FuncoesMain.cpp \
      $(SRC_DIR)/SA.cpp

OBJ = $(SRC:.cpp=.o)

TARGET = $(OUTPUT_DIR)/tsp_solver

# Regra padrão (build)
all: $(TARGET)

# Como compilar o executável
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $^ -o $@

# Limpeza de arquivos .o e executável
clean:
	rm -f $(OBJ) $(TARGET)

# Executar o programa
run: $(TARGET)
	./$(TARGET)
