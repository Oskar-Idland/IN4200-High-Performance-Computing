# Compiler settings
CC = gcc
CC_PARALLEL = mpicc
CFLAGS = -Wall -Wextra -I./include -lm
LDFLAGS = 

# Directories (keeping as requested)
SRC_SERIAL_DIR = serial/src
INCLUDE_SERIAL_DIR = serial/include
OBJ_DIR_SERIAL = serial/obj

SRC_PARALLEL_DIR = parallel/src
INCLUDE_PARALLEL_DIR = parallel/include
OBJ_DIR_PARALLEL = parallel/obj

# Source files
SERIAL_SRCS = $(wildcard $(SRC_SERIAL_DIR)/*.c)
PARALLEL_SRCS = $(wildcard $(SRC_PARALLEL_DIR)/*.c)

# Object files
SERIAL_OBJS = $(patsubst $(SRC_SERIAL_DIR)/%.c,$(OBJ_DIR_SERIAL)/%.o,$(SERIAL_SRCS))
PARALLEL_OBJS = $(patsubst $(SRC_PARALLEL_DIR)/%.c,$(OBJ_DIR_PARALLEL)/%.o,$(PARALLEL_SRCS))

# Targets
TARGET_SERIAL = main_serial
TARGET_PARALLEL = main_parallel

# Default target: build both versions
all: serial parallel

# Serial version
serial: $(OBJ_DIR_SERIAL) $(SERIAL_OBJS)
	$(CC) $(CFLAGS) $(SERIAL_OBJS) -o $(TARGET_SERIAL) $(LDFLAGS)

# Parallel version
parallel: $(OBJ_DIR_PARALLEL) $(PARALLEL_OBJS)
	$(CC_PARALLEL) $(CFLAGS) -fopenmp $(PARALLEL_OBJS) -o $(TARGET_PARALLEL) $(LDFLAGS)

# Create object directories
$(OBJ_DIR_SERIAL):
	mkdir -p $(OBJ_DIR_SERIAL)

$(OBJ_DIR_PARALLEL):
	mkdir -p $(OBJ_DIR_PARALLEL)

# Compile serial source files
$(OBJ_DIR_SERIAL)/%.o: $(SRC_SERIAL_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE_SERIAL_DIR) -c $< -o $@

# Compile parallel source files
$(OBJ_DIR_PARALLEL)/%.o: $(SRC_PARALLEL_DIR)/%.c
	$(CC_PARALLEL) $(CFLAGS) -I$(INCLUDE_PARALLEL_DIR) -fopenmp -c $< -o $@

# Debug target
debug: CFLAGS += -g -DDEBUG
debug: clean all

clean:
	rm -rf $(OBJ_DIR_SERIAL)
	rm -rf $(OBJ_DIR_PARALLEL)
	rm -f $(TARGET_SERIAL) $(TARGET_PARALLEL)

.PHONY: all serial parallel debug clean
