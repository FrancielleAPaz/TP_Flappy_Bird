# Diretórios
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Arquivo final
TARGET = $(BIN_DIR)/flappybird

# Compilador e flags
CXX = g++
CXXFLAGS = -Wall -std=c++17 -I$(INC_DIR)

# Bibliotecas Allegro
LDLIBS = -lallegro -lallegro_image -lallegro_font -lallegro_ttf -lallegro_primitives -lallegro_audio -lallegro_acodec -lallegro_dialog

# Todos os arquivos fonte
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Arquivos objeto correspondentes
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Regra padrão
all: $(BIN_DIR) $(OBJ_DIR) $(TARGET)

# Compilar executável
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

# Compilar objetos
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Criar pastas se não existirem
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Limpeza
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

.PHONY: all clean
