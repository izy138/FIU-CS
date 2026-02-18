/**
 * @file shell_server.h
 * @brief Header file for Multi-Threaded TCP Shell Server Assignment
 * 
 * STUDENT INSTRUCTIONS:
 * - Do NOT modify this header file
 * - Do NOT add any printf statements in your implementations (the driver handles all output)
 * - The testing framework handles all input/output
 */

#ifndef SHELL_SERVER_H
#define SHELL_SERVER_H

// Feature test macros MUST come before any #include statements
#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE
/*
 * Standard library includes
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>

/*
 * Network configuration constants
 */
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888
/*
 * Communication constants
 */
#define BUFFER_SIZE 1024
#define RESPONSE_END_MARKER "<END_OF_RESPONSE>"

/*
 * Server configuration constants
 */
#define MAX_CLIENTS 50
#define COMMAND_TIMEOUT 3

/*
 * Server function prototypes
 */
int setup_tcp_server(int port);
void* handle_client_connection(void* socket_ptr);
void execute_shell_command(char* command, int client_socket);
void send_command_output(int client_socket, pid_t child_pid);
void handle_user_command(char* command, int client_socket);
void cleanup_and_exit(int sig);

/*
 * Client function prototypes
 */
int connect_to_server(const char* ip, int port);
void send_command(int socket, const char* command);
void receive_response(int socket);
void trim_whitespace(char* str);
void print_welcome_banner(void);
void print_help(void);

/*
 * Note: Global variables are defined separately in each source file:
 * - server.c: tcp_server_socket, server_running
 * - client.c: tcp_socket, client_running
 */

#endif /* SHELL_SERVER_H */