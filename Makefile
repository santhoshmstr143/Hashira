# Makefile for DNA Pattern Matching Suite

CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99 -Iinclude
LDFLAGS = -lm

# Directories
SRC_DIR = src
ALGO_DIR = $(SRC_DIR)/algorithms
UTILS_DIR = $(SRC_DIR)/utils
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SRCS = $(SRC_DIR)/main.c \
       $(ALGO_DIR)/kmp_algorithm.c \
       $(ALGO_DIR)/naive_algorithm.c \
       $(ALGO_DIR)/boyer_moore_algorithm.c \
       $(ALGO_DIR)/suffix_tree.c \
       $(ALGO_DIR)/shift_or_algorithm.c \
       $(ALGO_DIR)/levenshtein_algorithm.c \
       $(ALGO_DIR)/rabin_karp_algorithm.c \
       $(ALGO_DIR)/z_algorithm.c \
       $(ALGO_DIR)/aho_corasick_algorithm.c \
       $(UTILS_DIR)/dna_sequence_handler.c \
       $(UTILS_DIR)/utils.c

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Executable name
TARGET = $(BIN_DIR)/dna_pattern_matching

# Default target
all: directories $(TARGET)

# Create directories
directories:
	@mkdir -p $(OBJ_DIR)/algorithms
	@mkdir -p $(OBJ_DIR)/utils
	@mkdir -p $(BIN_DIR)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)
	@echo "Build successful! Run with: ./$(TARGET)"

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Clean complete!"

# Run the program
run: $(TARGET)
	./$(TARGET)

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: clean $(TARGET)

# Create sample FASTA file
sample:
	@mkdir -p data
	@echo ">Sample DNA Sequence" > data/sample.fasta
	@echo "ACGTACGTACGTTAGCTAGCTAGCTACGTACGTACGT" >> data/sample.fasta
	@echo "ACGTACGTTAGCTAGCTAGCTTACGTACGTACGTACG" >> data/sample.fasta
	@echo "Sample FASTA file created: data/sample.fasta"

# Help
help:
	@echo "DNA Pattern Matching Suite - Makefile Help"
	@echo ""
	@echo "Available targets:"
	@echo "  make        - Build the project"
	@echo "  make run    - Build and run the program"
	@echo "  make clean  - Remove build files"
	@echo "  make debug  - Build with debug symbols"
	@echo "  make sample - Create a sample FASTA file"
	@echo "  make help   - Show this help message"

.PHONY: all clean run debug sample help