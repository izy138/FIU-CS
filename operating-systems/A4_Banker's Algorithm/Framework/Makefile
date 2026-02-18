# CPU Scheduling Algorithms - Makefile
# ❎ Do not modify this file contents!
# ===============================================================================

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c17

# Target executable
TARGET = banker

# Source files
SOURCES = banker_driver.c need.c available.c safety.c request.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Header files
HEADERS = banker_header.h

# Default target - build and show help
help: all cleanObj
	@echo ""
	@echo "Build completed successfully!"
	@echo ""
	@echo "Available targets:"
	@echo "  all      - Build the scheduler"
	@echo "  clean    - Remove build files"
	@echo "  rebuild  - Clean and build"
	@echo "  test     - Run with INPUT.txt if it exists"
	@echo "  install  - Install to /usr/local/bin"
	@echo "  help     - Show this help message"
	@echo ""
	@echo "Application Usage: ./banker"
	@echo ""

# Build target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files to object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean object files
cleanObj:
	rm -f $(OBJECTS)

# Clean executable file
clean:
	rm -f $(TARGET)

# Rebuild everything
rebuild: clean all cleanObj

# Test with sample input (if INPUT.txt exists)
test: $(TARGET)
	@if [ -f INPUT.txt ]; then \
		echo "Running scheduler with INPUT.txt:"; \
		./$(TARGET) INPUT.txt; \
	else \
		echo "Error: INPUT.txt not found. Please create an input file."; \
	fi

.PHONY: all clean rebuild test help