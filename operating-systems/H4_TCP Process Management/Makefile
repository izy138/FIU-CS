# ============================================================================
# Compiler settings
# ============================================================================
CC = gcc
CFLAGS = -std=c17 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Werror -pedantic -g -O0
LDFLAGS = 

# ============================================================================
# Project settings - TCP Command Server
# ============================================================================
SERVER_TARGET = server
CLIENT_TARGET = client
SERVER_SOURCES = driver.c submission_HW4.c
CLIENT_SOURCES = tcp_client.c
HEADERS = tcp_server.h

# ============================================================================
# Build Rules
# ============================================================================

# Default target - builds both server and client
all: $(SERVER_TARGET) $(CLIENT_TARGET)

# Server build rule
$(SERVER_TARGET): $(SERVER_SOURCES) $(HEADERS)
	@echo "Checking TCP server implementation..."
	@if ! grep -v "^[[:space:]]*//.*run_tcp_server" submission_HW4.c | grep -q "run_tcp_server" ; then \
		echo "ERROR: run_tcp_server() function not implemented in submission_HW4.c"; \
		echo "You must implement the main server function"; \
		echo "COMPILATION FAILED - implement required functions"; \
		exit 1; \
	fi
	@if ! grep -v "^[[:space:]]*//.*socket" submission_HW4.c | grep -q "socket[[:space:]]*(" ; then \
		echo "ERROR: socket() function call not found in submission_HW4.c"; \
		echo "You must create TCP socket in run_tcp_server()"; \
		echo "COMPILATION FAILED - implement socket programming"; \
		exit 1; \
	fi
	@if ! grep -v "^[[:space:]]*//.*bind" submission_HW4.c | grep -q "bind[[:space:]]*(" ; then \
		echo "ERROR: bind() function call not found in submission_HW4.c"; \
		echo "You must bind socket to port 8080"; \
		echo "COMPILATION FAILED - implement socket programming"; \
		exit 1; \
	fi
	@if ! grep -v "^[[:space:]]*//.*accept" submission_HW4.c | grep -q "accept[[:space:]]*(" ; then \
		echo "ERROR: accept() function call not found in submission_HW4.c"; \
		echo "You must accept client connections"; \
		echo "COMPILATION FAILED - implement socket programming"; \
		exit 1; \
	fi
	@if ! grep -v "^[[:space:]]*//.*fork" submission_HW4.c | grep -q "fork[[:space:]]*(" ; then \
		echo "ERROR: fork() function call not found in submission_HW4.c"; \
		echo "You must fork child processes for each command"; \
		echo "COMPILATION FAILED - implement process management"; \
		exit 1; \
	fi
	@if ! grep -v "^[[:space:]]*//.*system" submission_HW4.c | grep -q "system[[:space:]]*(" ; then \
		echo "ERROR: system() function call not found in submission_HW4.c"; \
		echo "You must use system() to execute commands"; \
		echo "COMPILATION FAILED - implement command execution"; \
		exit 1; \
	fi
	@echo "TCP server implementation checks passed"
	@echo "Building $(SERVER_TARGET)..."
	$(CC) $(CFLAGS) $(SERVER_SOURCES) -o $(SERVER_TARGET) $(LDFLAGS)
	@echo "Server build successful!"

# Client build rule  
$(CLIENT_TARGET): $(CLIENT_SOURCES)
	@echo "Building $(CLIENT_TARGET)..."
	$(CC) $(CFLAGS) $(CLIENT_SOURCES) -o $(CLIENT_TARGET) $(LDFLAGS)
	@echo "Client build successful!"

# Clean up any running servers
killserver:
	@echo "Killing any existing server processes..."
	@killall server 2>/dev/null || true
	@sleep 1

# Clean up generated files
clean:
	@echo "Cleaning up..."
	rm -f $(SERVER_TARGET) $(CLIENT_TARGET) *.o *.gch
	@echo "Cleanup complete."

# Rebuild everything from scratch
rebuild: killserver clean all

# Declare phony targets
.PHONY: all killserver clean rebuild