#ifndef PROCESS_CONTROLLER_H
#define PROCESS_CONTROLLER_H

/**
 * Execute a command by creating a child process
 * @param args: NULL-terminated array of command and arguments
 *              Example: char *args[] = {"echo", "hello", NULL};
 * 
 * Expected output format:
 *   Executing: [command]
 *   Child <pid> started
 *   Child <pid> exited with status <exit_code>
 */
void execute_command(char *args[]);

#endif