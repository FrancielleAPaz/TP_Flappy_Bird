# Compilador e flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude
LDFLAGS = -lallegro -lallegro_image -lallegro_font -lallegro_ttf \
          -lallegro_primitives -lallegro_audio -lallegro_acodec -lallegro_dialog

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

# Arquivos de teste
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
TEST_TARGET = $(BIN_DIR)/testes

# Regra padrão
all: $(TARGET)

# Linkagem final do jogo
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compilação dos arquivos .cpp para .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Testes
test: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRCS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

run_test: test
	./$(TEST_TARGET)

clean_test:
	rm -f $(TEST_TARGET)

# Limpeza geral
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/flappybird $(TEST_TARGET)

# Executar o jogo
run: all
	./$(TARGET)

.PHONY: all clean run test run_test clean_test