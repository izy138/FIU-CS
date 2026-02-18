# ============================================================================
# Compiler settings
CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Werror -g -O0
LDFLAGS =

# ============================================================================
# Project settings - FCFS Scheduling Algorithm Homework
TARGET = fcfs
SOURCES = driver.c submission_HW5.c 
HEADERS = fcfs_scheduler.h

# ============================================================================
# Build Rules
# ============================================================================

# Default target - builds the data structures program
all: $(TARGET)

# Main build rule - creates the executable
$(TARGET): $(SOURCES) $(HEADERS)
	@echo "Building $(TARGET)..."
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)
	@echo "Build successful! Run with: ./$(TARGET)"

# Run the program with test cases
run: $(TARGET)
	./$(TARGET)

# Clean up generated files
clean:
	@echo "Cleaning up..."
	rm -f $(TARGET) *.o STUDENT_OUTPUT.txt
	@echo "Cleanup complete."

# Rebuild everything from scratch
rebuild: clean all

# Show available commands
help:
	@echo "Available commands:"
	@echo "  make       - Build the program"
	@echo "  make run   - Build and run with TESTCASES.txt"
	@echo "  make clean - Remove generated files"
	@echo "  make rebuild - Clean and build from scratch"

# Declare phony targets
.PHONY: all run clean rebuild help