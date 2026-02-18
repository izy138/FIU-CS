/**
 * @file driver.c
 * @brief driver for TCP command server
 * @author Dr. Bhargav Bhatkalkar, KFSCIS, Florida International University
 * 
 * This is a framework file - DO NOT MODIFY
 * 
 * This driver handles only signal setup and calls the student's server implementation.
 * Students implement ALL socket programming and server logic in submission_HW4.c
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include "tcp_server.h"

// Reap terminated children to avoid zombie processes
void sigchld_handler(int signo) {
    (void)signo; // Suppress unused parameter warning
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(void) {
    printf("Starting TCP Command Server...\n");

    // Install SIGCHLD handler using sigaction()
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; // Auto-restart interrupted syscalls
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction failed");
        exit(EXIT_FAILURE);
    }

    // Call student's server implementation
    int result = run_tcp_server();
    
    if (result != 0) {
        printf("Server exited with error code: %d\n", result);
        return EXIT_FAILURE;
    }
    
    printf("Server: Shutting down\n");
    return EXIT_SUCCESS;
}