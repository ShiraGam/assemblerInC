CC = gcc
CFLAGS = -g -Wall -fsanitize=address -pedantic -ansi -lm

# Paths to header files
INC_DIR = ../project/headers

# Paths to source files
SRC_DIR = ../project/sources

# Directory for object files
OBJ_DIR = ../project/objects

# List of source files
SRCS = $(SRC_DIR)/Arrays.c \
       $(SRC_DIR)/Second_pass.c \
       $(SRC_DIR)/First_pass.c \
       $(SRC_DIR)/Symbols.c \
       $(SRC_DIR)/Errors.c \
       $(SRC_DIR)/Global_fun.c \
       $(SRC_DIR)/Macro.c \
       $(SRC_DIR)/Main_program.c \
       $(SRC_DIR)/Opcodes_table.c \
       $(SRC_DIR)/Pre_proccesor.c \
       $(SRC_DIR)/Reserved_words.c

# List of object files with paths relative to OBJ_DIR
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Default target
all: assembler

# Create object directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $(CFLAGS) -I$(INC_DIR) $< -o $@

# Link object files into executable
assembler: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Clean target
clean:
	rm -f $(OBJS) run
