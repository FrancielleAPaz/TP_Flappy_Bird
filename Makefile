# Diretórios
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
TESTS_DIR = tests
TESTS_OUT_DIR = $(TESTS_DIR)/output

# Arquivo final do jogo
TARGET = $(BIN_DIR)/flappybird

# Compilador e flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I$(INC_DIR) -I"C:/Users/moyse/Downloads/allegro-x86_64-w64-mingw32-gcc-14.2.0-posix-seh-dynamic-5.2.10.0/allegro/include"

# Bibliotecas Allegro - usando versão 5.2.0 e pasta correta
LDLIBS = -L"C:/Users/moyse/Downloads/allegro-x86_64-w64-mingw32-gcc-14.2.0-posix-seh-dynamic-5.2.10.0/allegro/lib" \
         -lallegro \
         -lallegro_image \
         -lallegro_font \
         -lallegro_ttf \
         -lallegro_primitives \
         -lallegro_audio \
         -lallegro_acodec \
         -lallegro_dialog

# Arquivos fonte do projeto
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Arquivos fonte dos testes
TEST_SOURCES = $(wildcard $(TESTS_DIR)/*.cpp)
TEST_OBJECTS = $(patsubst $(TESTS_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(TEST_SOURCES))
TEST_EXECUTABLES = $(patsubst $(TESTS_DIR)/%.cpp,$(TESTS_OUT_DIR)/%.exe,$(TEST_SOURCES))

# Regra padrão para compilar o jogo
all: $(BIN_DIR) $(OBJ_DIR) $(TARGET)

# Compilar o executável principal
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

# Compilar objetos do projeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Criar diretórios se não existirem
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(TESTS_OUT_DIR):
	mkdir -p $(TESTS_OUT_DIR)

# Regra para compilar todos os testes
tests: $(TESTS_OUT_DIR) $(TEST_EXECUTABLES)

# Compilar cada teste em executável
$(TESTS_OUT_DIR)/%.exe: $(TESTS_DIR)/%.cpp $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $< $(SOURCES) $(LDLIBS)

# Rodar todos os testes (você pode modificar para rodar teste específico)
run_tests: tests
	@for test_exec in $(TEST_EXECUTABLES); do \
		echo "Rodando $$test_exec..."; \
		"./$$test_exec"; \
	done

# Limpeza dos objetos e binários
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET) $(TESTS_OUT_DIR)/*.exe

.PHONY: all clean tests run_tests
