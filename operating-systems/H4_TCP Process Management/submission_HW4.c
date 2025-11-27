/*
 * HOMEWORK-4: TCP Command Server with Process Management
 * Student Implementation Template. 
 *
 * INSTRUCTIONS:
 * Complete the implementation of all three functions below.
 * Follow the specifications exactly as provided.
 * Handle all socket programming, TCP server logic, and process management.
 *
 * REQUIREMENTS:
 * - Create TCP server socket and bind to port 8080
 * - Accept one client connection and handle commands in a loop
 * - Fork a new child process for EACH command received
 * - Execute commands using system() in child processes
 * - Send command status back to client
 * - Proper process status collection and cleanup
 * - Handle all edge cases (socket errors, fork failure, client disconnection)
 *
 * ⚠️ GRADING CRITERIA:
 * - Correct TCP socket creation, binding, and listening
 * - Proper client connection acceptance and handling
 * - Fork child process for each command execution
 * - Use system() to execute commands in child processes
 * - Send appropriate status messages back to client
 * - Proper process cleanup and socket closure
 * - Correct error handling for all operations
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "tcp_server.h"

#define PORT 8080
#define BUFFER_SIZE 1024

/**
 * Executes a command in child process and sends status to client
 *
 * SPECIFICATION:
 * - Execute the received command using system() function
 * - Create appropriate status message based on command execution result
 * - Send status message back to client through socket
 * - ⚠️ Must close socket and exit child process (DO NOT return)
 * - Handle system() return values properly
 *
 * system() RETURN VALUES:
 * - 0: Command executed successfully
 * - Non-zero: Command failed or returned error code
 * - -1: system() call failed
 *
 * @param client_socket: Socket descriptor for client communication
 * @param command: Command string to execute
 */
void execute_command_in_child(int client_socket, const char *command)
{
    printf("Child %d: Executing command: %s\n", getpid(), command);
    fflush(stdout);

    // TODO: Execute command using system()
    // TODO: Create status message based on system() result
    // TODO: Check if system() succeeded (result == 0)
    // SUCCESS: Format message as "Command '%s' completed successfully" where %s is the command
    // FAILURE: Format message as "Command '%s' failed" where %s is the command
    int result = system(command);


    // TODO: Send status message to client
    // TODO: Check if send() failed and print error message

    // TODO: Close socket and exit child process
    // ⚠️HINT: exit(0) - Child must exit, never return to parent code

    char status_message[BUFFER_SIZE];
    if (result == 0) {
        // command executed successfully 
        snprintf(status_message, sizeof(status_message), 
                "Command '%s' completed successfully\n", command);
    } else {
        // command failed
        snprintf(status_message, sizeof(status_message), 
                "Command '%s' failed\n", command);
    }

     // this sends the status message to the client
     ssize_t bytes_sent = send(client_socket, status_message, strlen(status_message), 0);
     if (bytes_sent < 0) {
         perror("send to client failed");
     }
 
     // closes the socket and exits the child process
     close(client_socket);
     exit(0);  // Child must exit, never return to parent code
}

/**
 * Handles client requests by forking for each command
 *
 * SPECIFICATION:
 * - Convert client address to readable format and print connection info
 * - Loop continuously to receive commands from client
 * - For each command: fork child process, execute command, wait for completion
 * - Handle client disconnection gracefully
 * - Proper process cleanup and error handling
 *
 * ERROR HANDLING:
 * - Check recv() return value for disconnection (0) or error (-1)
 * - Check fork() return value for failure
 * - Handle all socket and process errors appropriately
 *
 * @param client_socket: Socket descriptor for client communication
 * @param client_addr: Client address structure containing IP and port
 */
void handle_client_request(int client_socket, struct sockaddr_in client_addr)
{
    char client_ip[INET_ADDRSTRLEN];
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;  // num of bytes received from client
    pid_t pid;   

    // this converts the client IP into a string
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    int client_port = ntohs(client_addr.sin_port);

    printf("Server: Connected to client %s:%d\n", client_ip, client_port);
    fflush(stdout);

    // Loop to handle multiple commands from the same client
    while (1) 
    {
        // Clear buffer
        memset(buffer, 0, BUFFER_SIZE);

        // TODO: Receive command from client
        // TODO: Check if recv() failed or client disconnected
        // HINT: recv() returns 0 when client closes connection (disconnected)
        // HINT: recv() returns -1 when error occurs
        // ⚠️ HINT: if (bytes_received <= 0), break from loop to terminate server

        //this makes sure the client is connected and receives the command
        bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            if (bytes_received == 0) {
                // client disconnected 
                break;
            } else {
                // recv() error
                perror("recv from client failed");
                break;
            }
        }
        // TODO: Null-terminate the received string and remove newline if present
        // HINT: buffer[bytes_received] = '\0';
        // HINT: Check if last character is '\n' and replace with '\0'

        // this makes sure the command is received and null terminated
        buffer[bytes_received] = '\0';

        // the command is null terminated and removes the newline if present
        char *newline = strchr(buffer, '\n');
        if (newline) {
            *newline = '\0';
        }

        printf("Server: Received command: %s\n", buffer);
        fflush(stdout);

        // ============================================================
        // FORK CHILD PROCESS FOR COMMAND EXECUTION
        // ============================================================

        // TODO: Fork a new child process for this command
        // TODO: Check for fork() failure (returns -1)

        // child process is forked and checks for failure
        pid = fork();
        if (pid < 0) {
            perror("fork failed");
            continue; 
        }

        // ============================================================
        // CHILD PROCESS - EXECUTE COMMAND
        // ============================================================

        // TODO: Check if this is child process (pid == 0)
        // TODO: Call execute_command_in_child() with socket and command

        // child process is executed
        if (pid == 0)  
        {
            execute_command_in_child(client_socket, buffer);
            
        }

        // ============================================================
        // PARENT PROCESS
        // ============================================================

        else
        {
            printf("Server: Child %d started for command: %s\n", pid, buffer);
            fflush(stdout);

            // TODO: Wait for child to complete using waitpid()

            // the child process is completed and catches any waitpid failures
            int status;
            if (waitpid(pid, &status, 0) == -1) {
                perror("waitpid failed");
            }
            
            printf("Server: Child %d completed\n", pid);
            fflush(stdout);
        }
    }

    printf("Server: Client %s:%d disconnected\n", client_ip, client_port);
    close(client_socket);
}

/**
 * Main TCP server implementation - Complete socket programming implementation
 *
 * SPECIFICATION:
 * - Create TCP socket using socket() system call
 * - Set socket options for address reuse to avoid "Address already in use" errors
 * - Configure server address structure for port 8080
 * - Bind socket to the configured address
 * - Listen for incoming connections
 * - Accept one client connection and handle requests
 * - Proper cleanup of all resources
 *
 * ERROR HANDLING:
 * - Check return values of all socket operations
 * - Use perror() for system call errors
 * - Return appropriate error codes
 * - Ensure proper resource cleanup
 *
 * @return: 0 on success, -1 on failure
 */
int run_tcp_server(void)
{
    // creates the socket and binds it to the port 8080
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);
    int opt=1;
    
    // TODO: Create TCP socket
    // creates the socket and checks for failure
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket creation failed");
        return -1;
    }
    // TODO: Set socket options for address reuse (SO_REUSEADDR)
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt for address reuse failed");
        close(server_fd);
        return -1;
    }
    // TODO: Configure server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // TODO: Bind socket to port 8080

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind socket failed");
        close(server_fd);
        return -1;
    }
    // TODO: Listen for connections

    if (listen(server_fd, 1) < 0) {
        perror("listen for connections failed");
        close(server_fd);
        return -1;
    }
    printf("Server: Listening on port %d...\n", PORT);
    printf("Server: Waiting for client connection...\n");
    fflush(stdout);

    // TODO: Accept client connection (single client for this homework)

    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addrlen);
    if (client_fd < 0) {
        perror("accept failed");
        close(server_fd); 
        return -1;
    }
    // Handle client requests
    handle_client_request(client_fd, client_addr);

    // Clean up server socket
    close(server_fd);

    return 0;
}

/*
 * EXPECTED OUTPUT PATTERNS:
 * Server startup: "Server: Listening on port 8080..."
 * Client connection: "Server: Connected to client 127.0.0.1:12345"
 * Command execution: "Server: Child 1234 started for command: echo hello"
 * Command completion: "Server: Child 1234 completed"
 * Client disconnection: "Server: Client 127.0.0.1:12345 disconnected"
 *
 * COMMAND STATUS MESSAGES (sent to client):
 * - Success: "Command 'echo hello' completed successfully"
 * - Failure: "Command 'invalidcommand' failed"
 */

/*
 * ⚠️ NOTE:
 * In the examples above, actual PIDs and port numbers will vary.
 * The autograder normalizes these values for comparison.
 * Ensure your output format matches exactly for proper grading.
 * 💡Test the instructor provided sample executables
 */