CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -I./src -I./test
LDFLAGS =

# Source files
SRC_DIR = src
TEST_DIR = test
BUILD_DIR = build

# Object files
SRC_OBJ = $(BUILD_DIR)/child_lock_system.o
TEST_OBJ = $(BUILD_DIR)/test_child_lock_system.o

# Executables
TEST_EXE = $(BUILD_DIR)/test_runner

# Default target
all: $(TEST_EXE)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile source files
$(BUILD_DIR)/child_lock_system.o: $(SRC_DIR)/child_lock_system.c $(SRC_DIR)/child_lock_system.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/test_child_lock_system.o: $(TEST_DIR)/test_child_lock_system.c $(SRC_DIR)/child_lock_system.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link test executable
$(TEST_EXE): $(SRC_OBJ) $(TEST_OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

# Run tests
test: $(TEST_EXE)
	./$(TEST_EXE)

# Clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all test clean