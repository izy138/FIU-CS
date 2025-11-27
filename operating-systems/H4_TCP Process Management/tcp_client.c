/**
 * @file test_client.c
 * @brief Framework client program for testing TCP command server
 * @author Dr. Bhargav Bhatkalkar, KFSCIS, Florida International University
 * 
 * This is a framework file - DO NOT MODIFY
 * 
 * This client reads commands from an input file and sends them to the server,
 * then receives and displays the responses from the server.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_LINE_LENGTH 256

static char *xstrdup(const char *s)
{
    size_t len = strlen(s) + 1;
    char *copy = malloc(len);
    if (!copy) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    memcpy(copy, s, len);
    return copy;
}

int main(int argc, char *argv[])
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    FILE *input_file;
    char **lines;
    int line_count = 0;
    int line_capacity = 10;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Open input file
    input_file = fopen(argv[1], "r");
    if (!input_file) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    // Read all lines into memory
    lines = malloc(line_capacity * sizeof(char *));
    if (!lines) {
        perror("malloc failed");
        fclose(input_file);
        return EXIT_FAILURE;
    }

    char file_buffer[MAX_LINE_LENGTH];
    while (fgets(file_buffer, sizeof(file_buffer), input_file)) {
        if (line_count >= line_capacity) {
            line_capacity *= 2;
            char **tmp = realloc(lines, line_capacity * sizeof(char *));
            if (!tmp) {
                perror("realloc failed");
                fclose(input_file);
                return EXIT_FAILURE;
            }
            lines = tmp;
        }
        lines[line_count] = xstrdup(file_buffer);
        line_count++;
    }
    fclose(input_file);

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Client: Connected to server\n");
    fflush(stdout);

    // Send commands and receive responses
    for (int i = 0; i < line_count; i++) {
        char *line = lines[i];
        
        // Skip empty lines and comments
        char *trimmed = line;
        while (*trimmed == ' ' || *trimmed == '\t') trimmed++;
        if (*trimmed == '\0' || *trimmed == '\n' || *trimmed == '#') {
            continue;
        }

        // Remove newline if present
        trimmed[strcspn(trimmed, "\n")] = '\0';
        
        if (strlen(trimmed) == 0) continue;

        printf("Client: Sending command: %s\n", trimmed);
        fflush(stdout);

        // Send command to server
        if (send(sock, trimmed, strlen(trimmed), 0) < 0) {
            perror("send failed");
            break;
        }

        // Receive response from server
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf("Client: Received: %s", buffer);
            if (buffer[strlen(buffer) - 1] != '\n') {
                printf("\n");
            }
            fflush(stdout);
        } else if (bytes_received == 0) {
            printf("Client: Server closed connection\n");
            break;
        } else {
            perror("recv failed");
            break;
        }

        // Small delay between commands
        sleep(1); // 1 second
    }

    // Free stored lines
    for (int i = 0; i < line_count; i++) {
        free(lines[i]);
    }
    free(lines);

    close(sock);
    printf("Client: Disconnected from server\n");
    return 0;
}