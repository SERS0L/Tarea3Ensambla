# === COMPILER AND FLAGS ===
CXX = g++
ASM = nasm
ASMFLAGS = -f elf64
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lcurl

# === FOLDERS ===
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
ASM_DIR = asm
WHISPER_DIR = whisper
WHISPER_BIN = $(WHISPER_DIR)/whisper-cli
WHISPER_SRC = ../whisper.cpp/build/bin/whisper-cli
MODEL_DIR = $(WHISPER_DIR)/models
MODEL_FILE = $(MODEL_DIR)/ggml-base.en.bin
MODEL_URL = https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-base.en.bin

# === FILES ===
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
ASM_SOURCES = $(wildcard $(ASM_DIR)/*.asm)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
ASM_OBJECTS = $(patsubst $(ASM_DIR)/%.asm, $(OBJ_DIR)/%.o, $(ASM_SOURCES))
TARGET = $(BIN_DIR)/app

# === DEFAULT TARGET ===
all: check-whisper check-model $(TARGET)

# === BUILD TARGET ===
$(TARGET): $(OBJECTS) $(ASM_OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJECTS) $(ASM_OBJECTS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(ASM_DIR)/%.asm
	@mkdir -p $(OBJ_DIR)
	$(ASM) $(ASMFLAGS) $< -o $@

# === CLEAN ===
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# === RUN ===
run: all
	./$(TARGET)

run-full: all
	@echo "Levantando backend..."
	cd ollama && node dist/index.js &
	sleep 1
	@echo "Ejecutando GUI..."
	./$(TARGET)


# === COPY whisper-cli IF MISSING ===
check-whisper:
	@if [ ! -f $(WHISPER_BIN) ]; then \
		echo "Downloading whisper-cli..."; \
		mkdir -p $(WHISPER_DIR); \
		cp $(WHISPER_SRC) $(WHISPER_BIN); \
		chmod +x $(WHISPER_BIN); \
	else \
		echo "✔ whisper-cli already installed."; \
	fi

# === DOWNLOAD MODEL IF MISSING ===
check-model:
	@if [ ! -f $(MODEL_FILE) ]; then \
		echo "Downloading model ggml-base.en.bin..."; \
		mkdir -p $(MODEL_DIR); \
		wget -O $(MODEL_FILE) $(MODEL_URL); \
	else \
		echo "✔ Model ggml-base.en.bin already exists."; \
	fi