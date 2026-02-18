/**
 * server_skeleton.c - Multi-threaded TCP Server
 * 
 * STUDENT ASSIGNMENT: Complete the TODO sections to implement a working server
 * 
 * This skeleton provides a complete framework for implementing a multi-threaded
 * TCP server that executes shell commands and returns results to clients.
 * 
 * ⚠️rename server_skeleton.c file to server.c
 */

// Feature test macros 
#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE
#include "shell_server.h"

// Global variables for cleanup - DO NOT MODIFY
int tcp_server_socket = -1;
volatile int server_running = 1;

/**
 * TODO:1 SETUP TCP SERVER SOCKET
 * 
 * Implement the setup_tcp_server function to:
 * 1. Create a TCP socket using socket()
 * 2. Set SO_REUSEADDR socket option to prevent "Address already in use" errors
 * 3. Configure server address structure (sockaddr_in)
 *    - Set family to AF_INET
 *    - Set address to SERVER_IP using inet_addr(). Note: SERVER_IP which is "127.0.0.1" is defined in the header file
 *    - Set port to SERVER_PORT using htons(). Note: SERVER_PORT which is 8888 is defined in the header file
 * 4. Bind the socket to the configured address
 * 5. Set the socket to listen mode with MAX_CLIENTS backlog
 * 6. Display success message and return the socket descriptor
 * 7. Handle all errors appropriately with informative messages
 * 
 * REQUIRED VARIABLES:
 * - int server_socket;
 * - struct sockaddr_in server_addr;
 * - int opt = 1;
 * 
 * 🕵️ HINTS:
 * - Use socket(AF_INET, SOCK_STREAM, 0) for TCP
 * - Use setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))
 * - Use memset() to zero out server_addr before configuring
 * - Use bind() with properly configured sockaddr_in structure
 * - Use listen() with MAX_CLIENTS parameter
 * - Return -1 on any error, socket descriptor on success
 * - Close socket before returning -1 on errors
 * 
 * EXAMPLE OUTPUT:
 * [Server] TCP server listening on 127.0.0.1:8888
 * [Server] Ready to accept client connections...
 */
int setup_tcp_server(int port) {

    /* YOUR CODE HERE - Implement TCP server setup */
    
    // Declare required variables:
    /* 
       🕵️HINT:
        int server_socket;
        struct sockaddr_in server_addr;
        int opt = 1;
    */
    int server_socket; 
    struct sockaddr_in server_addr;
    int opt = 1;
    
    // Step 1: Create TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        return -1;
    }
    
    // Step 2: Set SO_REUSEADDR socket option
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Setsockopt failed");
        close(server_socket);
        return -1;
    }
    
    // Step 3: Configure server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(port);
    
    // Step 4: Bind socket to address
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        return -1;
    }
    
    // Step 5: Set socket to listen mode
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        close(server_socket);
        return -1;
    }
    
    // Step 6: Print success messages
    printf("[Server] TCP server listening on %s:%d\n", SERVER_IP, SERVER_PORT);
    printf("[Server] Ready to accept client connections...\n");
    
    // This return statement should be replaced with your socket descriptor
    return server_socket;
}

/**
 * TODO 2: HANDLE CLIENT CONNECTION
 * 
 * Implement the handle_client_connection function to:
 * 1. Extract client socket from parameter and free allocated memory
 * 2. Get and display client IP address and port using getpeername()
 * 3. Send welcome message to client with RESPONSE_END_MARKER
 * 4. Enter main command processing loop:
 *    - Receive commands from client using recv()
 *    - Handle connection errors and client disconnection
 *    - Remove trailing newline/carriage return from commands
 *    - Handle empty commands gracefully
 *    - Process user-defined commands (EXIT, HELP) by calling handle_user_command()
 *    - Execute shell commands using execute_shell_command()
 *    - Break loop on EXIT command
 * 5. Clean up and close socket when client disconnects
 * 
 * REQUIRED VARIABLES:
 * - int client_socket = *(int*)socket_ptr;
 * - char buffer[BUFFER_SIZE];
 * - char client_ip[INET_ADDRSTRLEN];
 * - int client_port;
 * - struct sockaddr_in client_addr;
 * - socklen_t addr_len = sizeof(client_addr);
 * 
 * 🕵️ HINTS:
 * - Cast socket_ptr to int* and dereference: int client_socket = *(int*)socket_ptr;
 * - Call free(socket_ptr) to free the allocated memory
 * - Use getpeername() to get client address information
 * - Use inet_ntoa() and ntohs() to convert address to readable format
 * - Use recv() to receive commands, check return value for disconnection (<=0)
 * - Use strchr() to find and remove '\n' and '\r' characters
 * - Use send() to send responses back to client
 * - Send welcome message: "Welcome to Shell Server! Type 'HELP' for commands.\n" + RESPONSE_END_MARKER + "\n"
 * - Handle "EXIT" and "HELP" commands by calling handle_user_command()
 * - Call execute_shell_command() for all other commands
 * - Use strcmp() to compare commands
 * 
 * EXAMPLE OUTPUT:
 * [Thread 140234567890124] Client connected: 127.0.0.1:54321
 * [Thread 140234567890124] Command from 127.0.0.1:54321: 'ls -la'
 * [Thread 140234567890124] Client 127.0.0.1:54321 disconnected gracefully
 */
void* handle_client_connection(void* socket_ptr) {
    
    /* YOUR CODE HERE - Implement client connection handling */
    
    // Step 1: Extract client socket and free memory
    int client_socket = *(int*)socket_ptr;
    free(socket_ptr);
    
    // Step 2: Declare required variables
    char buffer[BUFFER_SIZE];
    char client_ip[INET_ADDRSTRLEN];
    int client_port;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    
    // Step 3: Get client address information using getpeername()
    if (getpeername(client_socket, (struct sockaddr*)&client_addr, &addr_len) == 0) {
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        client_port = ntohs(client_addr.sin_port);
        printf("[Thread %lu] Client connected: %s:%d\n", (unsigned long)pthread_self(), client_ip, client_port);
    } else {
        strcpy(client_ip, "unknown");
        client_port = 0;
        printf("[Thread %lu] Client connected: address unknown\n", (unsigned long)pthread_self());
    }
    
    // Step 4: Send welcome message to client
       /* 
         🕵️HINT:
          char welcome_msg[256];
          snprintf(welcome_msg, sizeof(welcome_msg), 
          "[SERVER] Hello %s:%d! Type 'HELP' for commands.\n" RESPONSE_END_MARKER "\n",
          client_ip, client_port);           
       */ 
    char welcome_msg[256];
    snprintf(welcome_msg, sizeof(welcome_msg), 
             "[SERVER] Hello %s:%d! Type 'HELP' for commands.\n" RESPONSE_END_MARKER "\n",
             client_ip, client_port);
    send(client_socket, welcome_msg, strlen(welcome_msg), 0);
    
    // Step 5: Main command processing loop
      /* 
        🕵️HINT:
         while (1) {
            Receive command from client
            Check for disconnection or errors
            Process command (remove newlines, handle empty commands)
            Call appropriate handler function
            Break on EXIT command
         }
     */
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        
        if (bytes_received <= 0) {
            if (bytes_received == 0) {
                printf("[Thread %lu] Client %s:%d disconnected gracefully\n", 
                       (unsigned long)pthread_self(), client_ip, client_port);
            } else {
                printf("[Thread %lu] Client %s:%d disconnected with error\n", 
                       (unsigned long)pthread_self(), client_ip, client_port);
            }
            break;
        }
        
        buffer[bytes_received] = '\0';
        
        // Remove trailing newline and carriage return
        char* newline = strchr(buffer, '\n');
        if (newline) *newline = '\0';
        char* carriage = strchr(buffer, '\r');
        if (carriage) *carriage = '\0';
        
        // Skip empty commands
        if (strlen(buffer) == 0) {
            continue;
        }
        
        printf("[Thread %lu] Command from %s:%d: '%s'\n", 
               (unsigned long)pthread_self(), client_ip, client_port, buffer);
        
        // Handle commands
        if (strcmp(buffer, "EXIT") == 0 || strcmp(buffer, "HELP") == 0) {
            handle_user_command(buffer, client_socket);
            if (strcmp(buffer, "EXIT") == 0) {
                break;
            }
        } else {
            execute_shell_command(buffer, client_socket);
        }
    }
    
    // Step 6: Cleanup and close socket
    close(client_socket);
    printf("[Thread %lu] Connection closed\n", (unsigned long)pthread_self());
    
    return NULL;
}

/**
 * TODO 3: EXECUTE SHELL COMMANDS WITH TIMEOUT
 * 
 * Implement the execute_shell_command function to:
 * 1. Fork a child process using fork()
 * 2. In child process:
 *    - Create unique temporary file for output (/tmp/shell_output_<PID>.txt)
 *      Use snprintf() to create filename: snprintf(temp_file, sizeof(temp_file), "/tmp/shell_output_%d.txt", getpid());
 *    - Open temporary file with proper permissions (O_WRONLY | O_CREAT | O_TRUNC, 0644)
 *    - Redirect stdout and stderr to the temporary file using dup2()
 *    - Execute the command using execlp("/bin/bash", "bash", "-c", command, NULL)
 * 3. In parent process:
 *    - Wait for child process with COMMAND_TIMEOUT seconds using sleep(COMMAND_TIMEOUT)
 *    - Use waitpid() with WNOHANG to check if child completed
 *    - If timeout occurs, kill child process with SIGKILL and send timeout message
 *    - If completed, call send_command_output() to send results
 *    - Clean up zombie process with waitpid()
 * 
 * REQUIRED VARIABLES:
 * - pid_t child_pid;
 * - int status;
 * - char temp_file[128];
 * 
 * 🕵️ HINTS:
 * - Use fork() to create child process
 * - Use snprintf() to create unique filename with getpid() *      
 * - In child: use open(), dup2() for output redirection to temp file
 * - If dup2() fails, close the temp file descriptor before exit 
        if (dup2(fd, STDOUT_FILENO) < 0) { perror("dup2 stdout"); close(fd); exit(EXIT_FAILURE); }
 * - Use execlp() to execute bash with -c option
 * - In parent: use sleep(COMMAND_TIMEOUT) and waitpid() with WNOHANG
 * - Use kill(child_pid, SIGKILL) for timeout
 * - Send timeout message: "Command Timeout (exceeded 3 seconds)\n" + RESPONSE_END_MARKER + "\n"
 * - Call send_command_output() when command completes normally
 * 
 * EXAMPLE OUTPUT:
 * [Thread 140234567890124] Executing shell command: 'ls -la'
 * [Thread 140234567890124] Forked child process 12347 for command execution
 * [Thread 140234567890124] Command completed
 */
void execute_shell_command(char* command, int client_socket) {
   
    /* YOUR CODE HERE - Implement shell command execution with timeout */
    
    // Step 1: Declare required variables
    pid_t child_pid;
    int status;
    char temp_file[128];
    
    // Step 2: Print command being executed
    printf("[Thread %lu] Executing shell command: '%s'\n", (unsigned long)pthread_self(), command);
    
    // Step 3: Fork child process
    child_pid = fork();
    
    // Step 4: Child process implementation
       /*
        🕵️HINT:
          if (child_pid == 0) {
            Create temp file name
            Open temp file for writing
            Redirect stdout and stderr using dup2()
            Execute command with execlp()
            Handle errors and exit
          }
    */
    if (child_pid == 0) {
        // Create temp file name
        snprintf(temp_file, sizeof(temp_file), "/tmp/shell_output_%d.txt", getpid());
        
        // Open temp file for writing
        int fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            perror("open temp file");
            exit(EXIT_FAILURE);
        }
        
        // Redirect stdout and stderr using dup2()
        if (dup2(fd, STDOUT_FILENO) < 0) {
            perror("dup2 stdout");
            close(fd);
            exit(EXIT_FAILURE);
        }
        if (dup2(fd, STDERR_FILENO) < 0) {
            perror("dup2 stderr");
            close(fd);
            exit(EXIT_FAILURE);
        }
        
        close(fd);
        
        // Execute command with execlp()
        execlp("/bin/bash", "bash", "-c", command, NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }
    
    // Step 5: Parent process implementation
    //   else if (child_pid > 0) {
    //     // Print forked message
    //     // Sleep for timeout period
    //     // Check if child completed with waitpid()
    //     // Handle timeout or normal completion
    //   }
    else if (child_pid > 0) {
        // Print forked message
        printf("[Thread %lu] Forked child process %d for command execution\n", 
               (unsigned long)pthread_self(), child_pid);
        
        // Sleep for timeout period
        sleep(COMMAND_TIMEOUT);
        
        // Check if child completed with waitpid()
        if (waitpid(child_pid, &status, WNOHANG) == 0) {
            // Child still running - timeout occurred
            printf("[Thread %lu] Command timed out, killing child process %d\n", 
                   (unsigned long)pthread_self(), child_pid);
            kill(child_pid, SIGKILL);
            waitpid(child_pid, &status, 0); // Clean up zombie
            
            // Send timeout message
            char timeout_msg[256];
            snprintf(timeout_msg, sizeof(timeout_msg), 
                     "Command Timeout (exceeded %d seconds)\n" RESPONSE_END_MARKER "\n", 
                     COMMAND_TIMEOUT);
            send(client_socket, timeout_msg, strlen(timeout_msg), 0);
        } else {
            // Command completed normally
            printf("[Thread %lu] Command completed\n", (unsigned long)pthread_self());
            send_command_output(client_socket, child_pid);
        }
    }
    
    // Step 6: Handle fork error
    else {
        perror("fork");
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), 
                 "Fork failed: %s\n" RESPONSE_END_MARKER "\n", strerror(errno));
        send(client_socket, error_msg, strlen(error_msg), 0);
    }
}

/**
 * TODO 4: SEND COMMAND OUTPUT TO CLIENT
 * 
 * Implement the send_command_output function to:
 * 1. Create temporary filename using child PID
 * 2. Open the temporary file for reading
 * 3. Read the file contents line by line using fgets()
 * 4. Send each line to the client via TCP socket using send()
 * 5. Send end-of-response marker: RESPONSE_END_MARKER + "\n"
 * 6. Close file and delete temporary file with unlink()
 * 7. Handle file access errors gracefully
 * 
 * REQUIRED VARIABLES:
 * - char temp_file[128];
 * - char buffer[BUFFER_SIZE];
 * - FILE* file;
 * 
 * 🕵️ HINTS:
 * - Use fopen() to open the same file (💡line 152 above) created earlier with uniquue child process PID for reading ("r" mode)
 *   --> file = fopen(temp_file, "r");
 * - Use fgets() to read line by line
 * - Use send() to transmit each line to client
 * - Send RESPONSE_END_MARKER + "\n" to indicate end of output
 * - Use unlink() to delete temporary file
 * - Handle file access errors by sending error message
 */
void send_command_output(int client_socket, pid_t child_pid) {

    /* YOUR CODE HERE - Implement sending command output to client */
    
    // Step 1: Declare required variables
    char temp_file[128];
    char buffer[BUFFER_SIZE];
    FILE* file;
    
    // Step 2: Create temporary filename
    snprintf(temp_file, sizeof(temp_file), "/tmp/shell_output_%d.txt", child_pid);
    
    // Step 3: Open file for reading
    file = fopen(temp_file, "r");
    if (file == NULL) {
        perror("fopen temp file for reading");
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), 
                 "Error reading command output: %s\n" RESPONSE_END_MARKER "\n", 
                 strerror(errno));
        send(client_socket, error_msg, strlen(error_msg), 0);
        return;
    }
    
    // Step 4: Read and send file contents line by line
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        send(client_socket, buffer, strlen(buffer), 0);
    }
    
    // Step 5: Send end-of-response marker
    char end_marker[64];
    snprintf(end_marker, sizeof(end_marker), RESPONSE_END_MARKER "\n");
    send(client_socket, end_marker, strlen(end_marker), 0);
    
    // Step 6: Close file and cleanup
    fclose(file);
    unlink(temp_file);
}

/**
 * TODO 5: HANDLE USER-DEFINED COMMANDS
 * 
 * Implement the handle_user_command function to:
 * 1. Check if command is "EXIT" - send goodbye message and return
 * 2. Check if command is "HELP" - send comprehensive help information
 * 3. For any other command, call execute_shell_command() to treat as shell command
 * 4. All responses must include RESPONSE_END_MARKER + "\n"
 * 
 * 🕵️ HINTS:
 * - Use strcmp() to compare command strings
 * - For EXIT: send "Goodbye! Connection closing...\n" + RESPONSE_END_MARKER + "\n"
 * - For HELP: send multi-line help message with available commands and examples
 * - Include RESPONSE_END_MARKER + "\n" at end of each response
 * - For unknown commands, call execute_shell_command()
 * 
 * HELP MESSAGE SHOULD INCLUDE:
 * - Available Commands section
 * - Examples section
 * - Usage instructions
 */
void handle_user_command(char* command, int client_socket) {
    
    /* YOUR CODE HERE - Implement user command handling */
    
    // Step 1: Print debug message
    printf("[Thread %lu] Handling user command: '%s'\n", (unsigned long)pthread_self(), command);
    
    // Step 2: Check for EXIT command
    if (strcmp(command, "EXIT") == 0) {
        char goodbye_msg[128];
        snprintf(goodbye_msg, sizeof(goodbye_msg), 
                 "Goodbye! Connection closing...\n" RESPONSE_END_MARKER "\n");
        send(client_socket, goodbye_msg, strlen(goodbye_msg), 0);
        return;
    }
    
    // Step 3: Check for HELP command and send the following multi-line help message 
            /*  
               🕵️HINT:
                  Available Commands:
                    Shell Commands:
                    ls, pwd, date, echo, sleep, cat, etc. - Any valid shell command
                    ./program - Execute custom programs
                  Client Commands:
                    HELP - Show available commands
                    EXIT - Disconnect and quit client
                  Examples:
                    ls -la
                    pwd
                    sleep 5
                    echo 'Hello World'
                    cat /etc/hostname
            */
    if (strcmp(command, "HELP") == 0) {
        char help_msg[1024];
        snprintf(help_msg, sizeof(help_msg),
                 "Available Commands:\n"
                 "  Shell Commands:\n"
                 "  ls, pwd, date, echo, sleep, cat, etc. - Any valid shell command\n"
                 "  ./program - Execute custom programs\n"
                 "Client Commands:\n"
                 "  HELP - Show available commands\n"
                 "  EXIT - Disconnect and quit client\n"
                 "Examples:\n"
                 "  ls -la\n"
                 "  pwd\n"
                 "  sleep 5\n"
                 "  echo 'Hello World'\n"
                 "  cat /etc/hostname\n"
                 RESPONSE_END_MARKER "\n");
        send(client_socket, help_msg, strlen(help_msg), 0);
        return;
    }
    
    // Step 4: Handle unknown commands as shell commands
    execute_shell_command(command, client_socket);
}

/**
 * TODO 6: SIGNAL HANDLER FOR GRACEFUL SHUTDOWN
 * 
 * Implement the cleanup_and_exit function to:
 * 1. Print shutdown message with signal number
 * 2. Set server_running flag to 0
 * 3. Close the TCP server socket if valid
 * 4. Clean up any temporary files using system()
 * 5. Print cleanup completion messages
 * 6. Exit the program with exit(0)
 * 
 * 🕵️ HINTS:
 * - This function is called when user presses Ctrl+C (SIGINT)
 * - Print: "[Main Thread] Received signal %d, shutting down gracefully...\n"
 * - Check if tcp_server_socket >= 0 before closing
 * - Use system("rm -f /tmp/shell_output_*.txt") to remove temporary files
 * - Print progress messages for each cleanup step
 * - Use exit(0) to terminate program
 */
void cleanup_and_exit(int sig) {

    /* YOUR CODE HERE - Implement cleanup and exit */
    
    // Step 1: Print shutdown message
    printf("\n[Main Thread] Received signal %d, shutting down gracefully...\n", sig);
    
    // Step 2: Set server_running to 0
    server_running = 0;
    
    // Step 3: Close server socket if valid
    if (tcp_server_socket >= 0) {
        printf("[Main Thread] Closing server socket...\n");
        close(tcp_server_socket);
        tcp_server_socket = -1;
    }
    
    // Step 4: Clean up temporary files
    printf("[Main Thread] Cleaning up temporary files...\n");
    system("rm -f /tmp/shell_output_*.txt");
    
    // Step 5: Print completion message and exit
    printf("[Main Thread] Server shutdown complete\n");
    exit(0);
}


/* ################################### DO NOT MODIFY MAIN() FUNCTION ####################################### */


/**
 * Print welcome banner
 */
void print_welcome_banner() {
    printf("=================================================================\n");
    printf("        Socket-Based Multi-Client Shell Server Starting         \n");
    printf("=================================================================\n");
    printf("Server PID: %d\n", getpid());
    printf("Main Thread ID: %lu\n",  (unsigned long)pthread_self());
}


/** 
 * COMPLETE IMPLEMENTATION: Main sever function
 *
 * Main() function is already implemented. It:
 * 1. Sets up signal handlers
 * 2. Creates TCP server socket  
 * 3. Accepts client connections in a loop
 * 4. Creates new thread for each client
 * 
 * You should not need to modify this function.
 */
int main() {
    pthread_t client_thread;
    
    printf("=================================================================\n");
    printf("        Socket-Based Multi-Client Shell Server Starting         \n");
    printf("=================================================================\n");
    printf("Server PID: %d\n", getpid());
    printf("Main Thread ID: %lu\n", (unsigned long)pthread_self());
    
    // Setup signal handler for graceful shutdown
    signal(SIGINT, cleanup_and_exit);
    signal(SIGPIPE, SIG_IGN);  // Ignore broken pipe signals

    // Setup TCP server socket
    tcp_server_socket = setup_tcp_server(SERVER_PORT);
    if (tcp_server_socket < 0) {
        fprintf(stderr, "Failed to setup TCP server\n");
        exit(EXIT_FAILURE);
    }

    printf("[Server] Waiting for client connections...\n");
    printf("[Server] Press Ctrl+C to shutdown gracefully\n\n");

    // Main server loop - accept and handle client connections
    while (server_running) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        
        // Accept incoming client connection
        int client_socket = accept(tcp_server_socket, 
                                  (struct sockaddr*)&client_addr, 
                                  &client_addr_len);
        
        if (client_socket < 0) {
            if (server_running) {
                perror("Accept failed");
            }
            continue;
        }

        printf("[Main Thread %lu] Accepted connection from %s:%d\n",    
               (unsigned long)pthread_self(),
               inet_ntoa(client_addr.sin_addr), 
               ntohs(client_addr.sin_port));

        // Allocate memory for passing socket to thread
        int* client_socket_ptr = malloc(sizeof(int));
        if (client_socket_ptr == NULL) {
            perror("Memory allocation failed for client socket");
            close(client_socket);
            continue;
        }
        *client_socket_ptr = client_socket;

        // Create new thread to handle this client
        if (pthread_create(&client_thread, NULL, handle_client_connection, client_socket_ptr) != 0) {
            perror("Failed to create client thread");
            free(client_socket_ptr);
            close(client_socket);
            continue;
        }

        // Detach thread for automatic cleanup
        pthread_detach(client_thread);

        printf("[Main Thread %lu] Created thread %lu for client connection\n", 
               (unsigned long)pthread_self(), (unsigned long)client_thread);
    }

    printf("[Main Thread] Server main loop exiting\n");
    cleanup_and_exit(0);
    
    return 0;
}