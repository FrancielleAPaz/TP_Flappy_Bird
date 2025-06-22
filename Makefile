CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude -I"C:/allegro-5.0.10-mingw-4.7.0/include"
LDFLAGS = -L"C:/allegro-5.0.10-mingw-4.7.0/lib" \
          -lallegro-5.0.10-mt \
          -lallegro_image-5.0.10-mt \
          -lallegro_font-5.0.10-mt \
          -lallegro_ttf-5.0.10-mt \
          -lallegro_primitives-5.0.10-mt \
          -lallegro_audio-5.0.10-mt \
          -lallegro_acodec-5.0.10-mt \
          -lallegro_dialog-5.0.10-mt

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = tests

# Arquivos principais do jogo
SRCS := $(wildcard $(SRC_DIR)/*.cpp) main.cpp
OBJS := $(SRCS:.cpp=.o)
OBJS := $(patsubst %.o, $(OBJ_DIR)/%.o, $(notdir $(OBJS)))
TARGET = $(BIN_DIR)/flappybird

# Regra padrão
all: $(TARGET)

# Linkagem final do jogo
$(TARGET): $(OBJS)
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compilação dos arquivos .cpp para .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Testes
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
TEST_TARGET = $(BIN_DIR)/testes

test: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRCS)
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

run_test: test
	$(BIN_DIR)/testes

clean_test:
	del /F /Q $(TEST_TARGET) 2>NUL

# Limpeza geral
clean:
	del /F /Q $(OBJ_DIR)\*.o $(BIN_DIR)\flappybird $(TEST_TARGET) 2>NUL

# Executar o jogo
run: all
	$(BIN_DIR)/flappybird

# Compilar apenas o main.cpp da pasta src
compile_main:
	@if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o $(OBJ_DIR)/main.o

# Criar executável apenas do main
main: $(OBJ_DIR)/main.o
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"
	$(CXX) $(CXXFLAGS) $< -o $(BIN_DIR)/main $(LDFLAGS)

# Executar apenas o main
run_main: main
	$(BIN_DIR)/main

.PHONY: all clean run test run_test clean_test compile_main main run_main