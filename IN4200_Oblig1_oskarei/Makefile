# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -I./include -lm
LDFLAGS =

# Directories
SRC_DIR = src
SRC_SERIAL_DIR = $(SRC_DIR)/src_serial
SRC_SHARED_DIR = $(SRC_DIR)/src_shared
SRC_PARALLEL_DIR = $(SRC_DIR)/src_parallel
INCLUDE_DIR = include
OBJ_DIR = obj

# Source files
SERIAL_SRCS = $(wildcard $(SRC_SERIAL_DIR)/*.c)
SHARED_SRCS = $(wildcard $(SRC_SHARED_DIR)/*.c)
PARALLEL_SRCS = $(wildcard $(SRC_PARALLEL_DIR)/*.c)

# Object files
SERIAL_OBJS = $(patsubst $(SRC_SERIAL_DIR)/%.c,$(OBJ_DIR)/serial_%.o,$(SERIAL_SRCS))
SHARED_OBJS = $(patsubst $(SRC_SHARED_DIR)/%.c,$(OBJ_DIR)/shared_%.o,$(SHARED_SRCS))
PARALLEL_OBJS = $(patsubst $(SRC_PARALLEL_DIR)/%.c,$(OBJ_DIR)/parallel_%.o,$(PARALLEL_SRCS))

# Targets
TARGET = main
TARGET_OMP = main_omp

# Default target: build both regular and OpenMP versions
all: $(OBJ_DIR) $(TARGET) $(TARGET_OMP)

# Create obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Regular version
$(TARGET): $(SERIAL_OBJS) $(SHARED_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# OpenMP version
$(TARGET_OMP): $(PARALLEL_OBJS) $(SHARED_OBJS)
	$(CC) $(CFLAGS) -fopenmp $^ -o $@ $(LDFLAGS)

# Compile serial source files
$(OBJ_DIR)/serial_%.o: $(SRC_SERIAL_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile shared source files
$(OBJ_DIR)/shared_%.o: $(SRC_SHARED_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile parallel source files
$(OBJ_DIR)/parallel_%.o: $(SRC_PARALLEL_DIR)/%.c
	$(CC) $(CFLAGS) -fopenmp -c $< -o $@

# Debug target
debug: CFLAGS += -g -DDEBUG
debug: clean all

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET) $(TARGET_OMP)
	rmdir $(OBJ_DIR) 2>/dev/null || true

.PHONY: all debug clean
