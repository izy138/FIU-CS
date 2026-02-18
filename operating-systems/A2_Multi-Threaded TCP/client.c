/**
 * client_skeleton.c - TCP Client Template
 * 
 * STUDENT ASSIGNMENT: Complete the TODO sections to implement a working client
 * 
 * This skeleton provides a complete framework for implementing a TCP client
 * that connects to the Multi-threaded TCP server and exchanges commands and responses.
 * 
 * ⚠️ rename client_skeleton.c file to client.c
 */

// Feature test macros 
#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE

#include "shell_server.h"

// Global variables for cleanup - DO NOT MODIFY
int tcp_socket = -1;
volatile int client_running = 1;

/**
 * TODO:1 CONNECT TO SERVER 
 * 
 * Implement the connect_to_server function to:
 * 1. Create a TCP socket using socket()
 * 2. Configure server address structure with IP and port
 *    - Set family to AF_INET
 *    - Convert IP string to binary using inet_pton()
 *    - Set port to 8888 using htons(). Note: SERVER PORT is defined in the header file
 * 3. Connect to the server using connect()
 * 4. Handle connection errors with informative messages
 * 5. Print connection status messages
 * 6. Return socket descriptor on success, -1 on failure
 * 
 * REQUIRED VARIABLES:
 * - int sock;
 * - struct sockaddr_in server_addr;
 * 
 * 🕵️ HINTS:
 * - Use socket(AF_INET, SOCK_STREAM, 0) for TCP
 * - Use memset() to zero out server_addr before configuring
 * - Use inet_pton() to convert IP string to binary format
 * - Configure sockaddr_in structure properly
 * - Use connect() to establish connection
 * - Close socket on error before returning -1
 * - Print success/failure messages for user feedback
 * 
 * EXAMPLE OUTPUT:
 * [Connection] Connecting to server 127.0.0.1:8888...
 * [Connection] Successfully connected to 127.0.0.1:8888
 */
int connect_to_server(const char* ip, int port) {
    
    /* YOUR CODE HERE - Implement TCP connection to server */
    
    // Step 1: Declare required variables
    int sock;
    struct sockaddr_in server_addr;
    
    // Step 2: Create TCP socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return -1;
    }
    
    // Step 3: Configure server address structure
       /*
        🕵️HINT:   
            - Use memset() to zero out structure
            - Set address family to AF_INET
            - Set port using htons()
            - Convert IP using inet_pton()
       */
    //zero out to make sure theres no garbage values
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; //sets the address family to AF_INET
    server_addr.sin_port = htons(port); //convert port to network byte order
    //convert ip to binary format
    //returns 1 on success, 0 on failure, -1 on error
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
        printf("Invalid address/ Address not supported\n");
        close(sock);
        return -1;
    }
    
    // Step 4: Print connection attempt message
    printf("[Connection] Connecting to server %s:%d...\n", ip, port);
    
    // Step 5: Connect to server using connect()
    //establish tcp connection to the server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        return -1;
    }
    
    // Step 6: Handle connection result
    //   - Print success message and return socket on success
    //   - Print error message, close socket, and return -1 on failure
    printf("[Connection] Successfully connected to %s:%d\n", ip, port);
    
    /* END YOUR CODE */
    
    return sock; // Replace this with your socket descriptor
}

/**
 * TODO:2 SEND COMMAND TO SERVER 
 * 
 * Implement the send_command function to:
 * 1. Send the complete command string to server via TCP
 * 2. Handle partial sends to ensure complete transmission
 * 3. Send newline terminator after command
 * 4. Handle send errors appropriately with error messages
 * 
 * REQUIRED VARIABLES:
 * - size_t total_sent = 0;
 * - size_t command_len = strlen(command);
 * - const char* data = command;
 * 
 * 🕵️ HINTS:
 * - Use send() to transmit data
 * - Use a while loop to handle partial sends
 * - Check return value of send() for errors (< 0)
 * - Send newline character ("\n") after command
 * - Handle SIGPIPE errors gracefully
 * - Continue sending until all bytes are transmitted
 */
void send_command(int socket, const char* command) {
    
    /* YOUR CODE HERE - Implement command transmission */
    
    // Step 1: Declare required variables
    size_t total_sent = 0; //num of bytes sent successfully
    size_t command_len = strlen(command); //length of the command
    const char* data = command;     //points tot he command string
    
    // Step 2: Send complete command using loop for partial sends
    /* 
      🕵️HINT:
        while (total_sent < command_len) {
            Use send() to transmit remaining data
            Check for errors
            Update total_sent counter
        }
    */
    // tcp doeesnt always send all data in one go, so it sends it in chunks
    while (total_sent < command_len) {
        ssize_t bytes_sent = send(socket, data + total_sent, command_len - total_sent, 0);
        if (bytes_sent < 0) {
            perror("Send failed");
            return;
        }
        total_sent += bytes_sent;
    }
    
    // Step 3: Send newline terminator
    //server expects a newline after the command
    if (send(socket, "\n", 1, 0) < 0) {
        perror("Send newline failed");
        return;
    }
    
    /* END YOUR CODE */
}

/**
 * TODO:3 RECEIVE AND DISPLAY SERVER RESPONSE
 * 
 * Implement the receive_response function to:
 * 1. Receive data from server using recv() in a loop
 * 2. Accumulate all received data in a buffer
 * 3. Continue receiving until RESPONSE_END_MARKER is found
 * 4. Remove the end marker from output before displaying
 * 5. Handle server disconnection gracefully
 * 6. Display the complete response to user
 * 
 * REQUIRED VARIABLES:
 * - char buffer[BUFFER_SIZE];
 * - char all_data[BUFFER_SIZE * 5] = {0};
 * - ssize_t bytes_received;
 * - int response_done = 0;
 * 
 * 🕵️ HINTS:
 * - Use recv() in a loop to get all response data
 * - Check for RESPONSE_END_MARKER string using strstr()
 * - Use strcat() to accumulate data in all_data buffer
 * - Check recv() return value: 0 = disconnection, <0 = error
 * - Remove RESPONSE_END_MARKER before displaying output
 * - Set client_running = 0 on disconnection
 * - Use sleep(1) for brief delay after processing
 * 
 * EXAMPLE FLOW:
 * 1. Loop while !response_done && client_running
 * 2. Receive data chunk with recv()
 * 3. Append to all_data buffer
 * 4. Check if RESPONSE_END_MARKER is present
 * 5. If found, remove marker and display result
 */
void receive_response(int socket) {
    
    /* YOUR CODE HERE - Implement response reception and display */
    
    // Step 1: Declare required variables
    char buffer[BUFFER_SIZE]; //buffer to store the received data
    char all_data[BUFFER_SIZE * 5] = {0}; //buffer to store the accumulated data
    ssize_t bytes_received; //number of bytes received
    int response_done = 0; // indicate if the response is complete
    
    // Step 2: Main reception loop
        /*
          🕵️HINT:  
            while (!response_done && client_running) {
                Receive data chunk
                Handle disconnection and errors
                Null-terminate received data
                Append to all_data buffer
                Check for RESPONSE_END_MARKER
            }
        */
    //continues until the response is complete  
    while (!response_done && client_running) {
        memset(buffer, 0, BUFFER_SIZE); //zero out the buffer to make sure no garbage values    
        bytes_received = recv(socket, buffer, BUFFER_SIZE - 1, 0); //receive data from the server returns the number of bytes received
        if (bytes_received <= 0) { //if the server disconnected or an error occurred
            if (bytes_received == 0) { 
                printf("[Client] Server disconnected\n");
            } else { 
                perror("Receive failed");
            }
            client_running = 0;
            break; 
        }
        buffer[bytes_received] = '\0'; //null terminates the received data
        strcat(all_data, buffer); //append the received data to the accumulated data
        // Check for RESPONSE_END_MARKER if the response is complete
        if (strstr(all_data, RESPONSE_END_MARKER) != NULL) {
            response_done = 1;
        } 
    }
    
    // Step 3: Process complete response
        /*
          🕵️HINT:  
            if (response_done) {
                Find and remove RESPONSE_END_MARKER
                Display the response
            }
        */
        //display the response
    if (response_done) {
        char* marker_pos = strstr(all_data, RESPONSE_END_MARKER); //find the position of the response end marker
        if (marker_pos != NULL) {
            *marker_pos = '\0'; // remove the marker
        }
        printf("%s", all_data); //display to user
    }

    // Step 4: Brief delay before continuing
    // delay to allow the server to process the response
    sleep(1);
    
    /* END YOUR CODE */
}

/**
 * TODO:4 SIGNAL HANDLER FOR CLIENT CLEANUP
 * 
 * Implement the cleanup_and_exit function to:
 * 1. Print shutdown message with signal number
 * 2. Set client_running flag to 0
 * 3. Send EXIT command to server if connected
 * 4. Add brief delay for server to process EXIT
 * 5. Close the TCP socket if valid
 * 6. Display cleanup completion message
 * 7. Exit the program with exit(0)
 * 
 * 🕵️ HINTS:
 * - This function is called when user presses Ctrl+C (SIGINT)
 * - Print: "[Main Thread] Received signal %d, shutting down...\n"
 * - Check if tcp_socket >= 0 before using it
 * - Use send_command() to send "EXIT" to server
 * - Use sleep(1) for brief delay after EXIT
 * - Close socket with close()
 * - Use exit(0) to terminate program
 */
void cleanup_and_exit(int sig) {
    
    /* YOUR CODE HERE - Implement cleanup and exit */
    
    // Step 1: Print shutdown message with signal number
    printf("\n[Main Thread] Received signal %d, shutting down...\n", sig);
    
    // Step 2: Set client_running flag to 0
    client_running = 0; 
    
    // Step 3: Handle server disconnection if connected
        /*
          🕵️HINT:  
            if (tcp_socket >= 0) {
                Send EXIT command using send_command()
                Add delay for server processing
                Close socket
                Print closure message
            }
        */
    
    if (tcp_socket >= 0) {
        //send exit command to the server
        printf("[Main Thread] Sending EXIT command to server...\n");
        send_command(tcp_socket, "EXIT");
        //delay to allow the server to process the exit command
        sleep(1);
        //close the socket
        close(tcp_socket);
       // indicate that the socket has been closed
        printf("[Main Thread] Socket closed\n");
    }
    
    // Step 4: Print completion message and exit
    printf("[Main Thread] Client shutdown complete\n");
    exit(0);
    
    /* END YOUR CODE */
}


/* ⚠️⚠️⚠️################# DO NOT MODIFY THE BELOW FUNCTIONS #################⚠️⚠️⚠️ */

/**
 * UTILITY FUNCTIONS - Already implemented (You need not implement these!)
 */

/**
 * Trim leading and trailing whitespace from a string
 */
void trim_whitespace(char* str) {
    char* start = str;
    char* end;

    // Skip leading whitespace
    while (isspace((unsigned char)*start)) {
        start++;
    }

    if (*start == 0) {
        *str = '\0';
        return;
    }

    // Trim trailing whitespace
    end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }
    *(end + 1) = '\0';

    // Move trimmed content to beginning
    if (start != str) {
        memmove(str, start, end - start + 2);
    }
}

/**
 * Print welcome banner
 */
void print_welcome_banner() {
    printf("\n=================================================================\n");
    printf("     Socket-Based Shell Client                                  \n");
    printf("=================================================================\n");
    printf("Client PID: %d\n", getpid());
    printf("Connecting to server at %s:%d\n", SERVER_IP, SERVER_PORT);
    printf("=================================================================\n\n");
}

/**
 * COMPLETE IMPLEMENTATION: Main client function
 */
int main() {
    char command[BUFFER_SIZE];
    
    // Display startup information
    print_welcome_banner();

    // Setup signal handlers for graceful shutdown
    signal(SIGINT, cleanup_and_exit);
    signal(SIGTERM, cleanup_and_exit);
    signal(SIGPIPE, SIG_IGN);

    // Connect to server
    tcp_socket = connect_to_server(SERVER_IP, SERVER_PORT);
    if (tcp_socket < 0) {
        printf("Failed to connect to server. Exiting.\n");
        cleanup_and_exit(0);
    }

    // Receive welcome message from server
    printf("\n[Connection] Connected! Receiving welcome message...\n");
    receive_response(tcp_socket);

    // Main command processing loop
    printf("\nReady for commands. Type 'EXIT' to quit.\n");
    while (client_running) {
        // Display command prompt
        printf("\nshell> ");
        fflush(stdout);

        // Read user command
        if (fgets(command, sizeof(command), stdin) == NULL) {
            if (feof(stdin)) {
                printf("\n[Main Thread] EOF detected, exiting...\n");
            }
            break;
        }

        // Process command input
        trim_whitespace(command);

        // Skip empty commands
        if (strlen(command) == 0) {
            continue;
        }
       

        // Send command to server
        printf("[Command] Sending: %s\n", command);
        send_command(tcp_socket, command);

        // Receive and display response
        printf("[Response] Server output:\n");
        printf("----------------------------------------\n");
        receive_response(tcp_socket);
        printf("----------------------------------------\n");

        // Check for EXIT command
        if (strcmp(command, "EXIT") == 0) {
            printf("[Main Thread] EXIT command sent, disconnecting...\n");
            break;
        }

        sleep(1); // Brief delay between commands
    }

    // Cleanup and exit
    cleanup_and_exit(0);
    return 0;
}