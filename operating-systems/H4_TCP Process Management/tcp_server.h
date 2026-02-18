#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>

/**
 * Main TCP server implementation (student implements this)
 * @return 0 on success, non-zero on error
 * 
 * Expected behavior:
 *   - Create and configure TCP socket on port 8080
 *   - Set socket options (SO_REUSEADDR)
 *   - Bind socket and start listening for connections
 *   - Accept one client connection at a time
 *   - Call handle_client_request() to process client
 *   - Clean up all resources before returning
 *   - Return 0 for success, 1 for error
 */
int run_tcp_server(void);

/**
 * Handles client requests by forking for each command (student implements this)
 * @param client_socket: Socket descriptor for accepted client
 * @param client_addr: Client address structure for IP/port information
 * 
 * Expected behavior:
 *   - Print client connection information (IP:port)
 *   - Loop to receive commands from client one by one
 *   - For each command: fork new child process
 *   - Child calls execute_command_in_child()
 *   - Parent waits for child completion with waitpid()
 *   - Continue until client disconnects (recv returns <= 0)
 *   - Close client socket when done
 */
void handle_client_request(int client_socket, struct sockaddr_in client_addr);

/**
 * Executes a command in child process and sends status to client (student implements this)
 * @param client_socket: Socket descriptor for client communication
 * @param command: Command string to execute
 * 
 * Expected behavior:
 *   - Print child PID and command being executed
 *   - Execute command using system()
 *   - Check system() return value (0=success, non-zero=failure)
 *   - Send appropriate status message to client via send()
 *   - Close client socket
 *   - Exit child process with exit(0)
 */
void execute_command_in_child(int client_socket, const char *command);

#endif