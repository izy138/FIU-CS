# ===============================================================================
# COP4610 Assignment 5 - Contiguous Memory Allocation Makefile
# Author: Dr. Bhargav Bhatkalkar, KFSCIS, Florida International University
# ===============================================================================

# Compiler and flags
CC = gcc
CFLAGS = -std=c17 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Werror -g -O0
TARGET = memory_allocator

# Source files
DRIVER = driver.c
STUDENT_SOURCES = first_fit.c best_fit.c worst_fit.c coalesce.c
HEADERS = memory_allocator.h

# Object files
DRIVER_OBJ = driver.o
STUDENT_OBJS = first_fit.o best_fit.o worst_fit.o coalesce.o

# Default target
all: $(TARGET)

# Link all object files to create executable
$(TARGET): $(DRIVER_OBJ) $(STUDENT_OBJS)
	@echo "Linking $(TARGET)..."
	$(CC) $(CFLAGS) -o $(TARGET) $(DRIVER_OBJ) $(STUDENT_OBJS)
	@echo "Build successful! Executable: $(TARGET)"

# Compile driver
$(DRIVER_OBJ): $(DRIVER) $(HEADERS)
	@echo "Compiling $(DRIVER)..."
	$(CC) $(CFLAGS) -c $(DRIVER)

# Compile student implementation files
first_fit.o: first_fit.c $(HEADERS)
	@echo "Compiling first_fit.c..."
	$(CC) $(CFLAGS) -c first_fit.c

best_fit.o: best_fit.c $(HEADERS)
	@echo "Compiling best_fit.c..."
	$(CC) $(CFLAGS) -c best_fit.c

worst_fit.o: worst_fit.c $(HEADERS)
	@echo "Compiling worst_fit.c..."
	$(CC) $(CFLAGS) -c worst_fit.c

coalesce.o: coalesce.c $(HEADERS)
	@echo "Compiling coalesce.c..."
	$(CC) $(CFLAGS) -c coalesce.c

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(TARGET) $(DRIVER_OBJ) $(STUDENT_OBJS)
	rm -f *.o *~ core
	@echo "Clean complete."

# Rebuild from scratch
rebuild: clean all

# Phony targets
.PHONY: all clean rebuild
