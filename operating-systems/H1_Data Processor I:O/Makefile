# Makefile for Student Data Processor
# Compiles student_submission.c to executable named 'run'

CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Werror -pedantic -g -O0
TARGET = run
SOURCE = student_submission.c

# Default target
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Clean target - removes executable and any object files  
clean:
	rm -f $(TARGET) *.o

# Phony targets
.PHONY: clean


