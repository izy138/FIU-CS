# ============================================================================
# Compiler settings
# ============================================================================
CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Werror -pedantic -g -O0
LDFLAGS = 

# ============================================================================
# Project settings - Process Controller
# ============================================================================
TARGET = run
SOURCES = driver.c submission_HW3.c
HEADERS = process_controller.h

# ============================================================================
# Build Rules
# ============================================================================

# Default target - builds the grade management system
all: $(TARGET)

# Main build rule - creates the executable
$(TARGET): $(SOURCES) $(HEADERS)
	@echo "Checking signal handler implementation..."
	@if ! grep -v "^[[:space:]]*//.*exit" submission_HW3.c | grep -q "exit[[:space:]]*([[:space:]]*124[[:space:]]*)" ; then \
		echo "⚠️ ERROR: exit(124) not found in submission_HW3.c"; \
		echo "⚠️ Signal handler must call exit(124) when SIGALRM is received"; \
		echo "⚠️ COMPILATION FAILED - add exit(124) in the signal handler to properly exit child upon timeout"; \
		exit 1; \
	fi
	@if ! grep -v "^[[:space:]]*//.*signal" submission_HW3.c | grep -q "signal[[:space:]]*([[:space:]]*SIGALRM[[:space:]]*,[[:space:]]*signal_handler[[:space:]]*)" ; then \
		echo "⚠️ ERROR: signal(SIGALRM, signal_handler) not found in submission_HW3.c"; \
		echo "⚠️ You must handle the signal by registering the signal handler with signal(SIGALRM, signal_handler)"; \
		echo "⚠️ COMPILATION FAILED - add signal registration in child process"; \
		exit 1; \
	fi
	@echo "Signal handler checks passed - both exit(124) and signal(SIGALRM, signal_handler) found"
	@echo "Building $(TARGET)..."
	$(CC) $(CFLAGS) $(SOURCES) $(LIBRARY) -o $(TARGET) $(LDFLAGS)
	@echo "Build successful! Run with: ./$(TARGET)"

# Clean up generated files
clean:
	@echo "Cleaning up..."
	rm -f $(TARGET) *.o *.gch
	@echo "Cleanup complete."

# Rebuild everything from scratch
rebuild: clean all

# Declare phony targets
.PHONY: all clean