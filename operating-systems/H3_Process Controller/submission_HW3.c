/*
 * HOMEWORK-3: Process Management with Timeout Handling
 * Student Implementation Template
 *
 * INSTRUCTIONS:
 * Complete the implementation of the signal_handler() and execute_command() function.
 * Follow the specifications exactly as provided.
 * Handle all process creation, execution, and termination scenarios.
 *
 * REQUIREMENTS:
 * - Implement proper signal handling for SIGALRM with exit code 124
 * - Create a new child process for each command
 * - Use execvp() to execute commands in child processes
 * - Implement 3-second timeout using alarm()
 * - Proper process status collection and reporting
 * - Handle all edge cases (fork failure, exec failure, etc.)
 *
 * ⚠️GRADING CRITERIA:
 * - Signal handler must call exit(124) when SIGALRM is received
 * - Must register signal handler with signal(SIGALRM, signal_handler)
 * - Correct process creation and execution flow
 * - Proper exit status and signal termination reporting
 * - Correct error handling
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include "process_controller.h"

/**
 * Signal handler for SIGALRM (timeout handling)
 *
 * SPECIFICATION:
 * - This function is called when SIGALRM signal is received (after alarm() expires)
 * - Must check if the received signal is SIGALRM
 * - ⚠️Must call exit(124) to terminate child process with timeout exit code
 * - 124 is the standard Unix exit code for timeout scenarios
 * - Do NOT return from this function - must call exit(124)
 *
 * @param sig: Signal number received by the process
 */
void signal_handler(int sig)
{
    // TODO: Check if signal is SIGALRM
    // TODO: Exit with code 124 (required for timeout handling)
    if(sig == SIGALRM){
        exit(124); //check for sigalrm
    }
}

/**
 * Executes a command in a child process with timeout handling (⚠️ 3 seconds)
 *
 * SPECIFICATION:
 * - Create a new child process for each command using fork()
 * - Child process: Register signal handler, set alarm, execute command
 * - Parent process: Wait for child and report exit status
 * - Handle three termination scenarios:
 *   1. Normal exit (WIFEXITED): Report exit code
 *   2. Signal termination (WIFSIGNALED): Report signal number
 *   3. Abnormal termination: Report abnormal exit
 *
 * PROCESS FLOW:
 * 1. Print "Executing: command args..."
 * 2. Fork to create child process
 * 3. Child: Register signal handler, set 3-second alarm, execute exec()
 * 4. Parent: Print "Child <pid> started", wait for child completion
 * 5. Parent: Analyze child exit status and print appropriate message
 *
 * ERROR HANDLING:
 * - Check fork() return value for failure
 * - Handle execvp() failure in child (exit with code 127)
 * - Check waitpid() return value for errors
 *
 * @param args: NULL-terminated array of command and arguments
 */
void execute_command(char *args[])
{
    // Print the command being executed
    printf("Executing: %s", args[0]);
    for (int i = 1; args[i] != NULL; i++)
    {
        printf(" %s", args[i]);
    }
    printf("\n");
    fflush(stdout); // CRITICAL: Force output before fork

    // TODO: Create child process using fork()
    // TODO: Store return value in 'pid_t pid' variable
    pid_t pid = fork();

    // TODO: Check for fork() failure (returns -1)
    // HINT: Use perror() to print error message fot fork() failure
    if(pid == -1){
        perror("fork() failed."); //checks for failure = -1
        return;
    }

    // ============================================================
    // CHILD PROCESS EXECUTION
    // ============================================================

    // TODO: Register signal handler for SIGALRM
    // HINT: Use signal(SIGALRM, signal_handler)
    // REQUIREMENT: This exact pattern must be present for grading
    if(pid == 0) {
        signal(SIGALRM,signal_handler);
        // TODO: Set 3-second timeout
        // HINT: Call alarm() to set 3-second timer
        alarm(3);

        // TODO: Execute the command using execvp()
        // HINT: execvp(args[0], args) replaces current process with command
        // HINT: execvp() only returns if it fails
        execvp(args[0], args);
        // TODO: Check if execvp failed. Exit with code 127 (standard for command not found)
        exit(127);
    }
    // ============================================================
    // PARENT PROCESS - WAIT AND REPORT
    // ============================================================

    // TODO: Print child started message
    // FORMAT: "Child <%d> started\n" where %d is the child PID
    printf("Child <%d> started\n", pid);

    fflush(stdout); // CRITICAL: Force output before child runs

    // TODO: Wait for child process and collect exit status
    // HINT: Use waitpid(pid, &status, 0) to wait for specific child
    // HINT: Use perror() to print error message fot waitpid() failure
    // HINT: Declare 'int status' variable to store exit information
    int status;
    if(waitpid(pid,&status,0) == -1){
        perror("waitpid failed.");
        return;
    }

    // ============================================================
    // PARENT ANALYZE CHILD EXIT STATUS AND REPORT
    // ============================================================

    // TODO: Check if child exited normally using WIFEXITED(status). If yes,
    // 1) TODO: Extract exit code using WEXITSTATUS(status)
    // 2) TODO: Print: "Child <%d> exited with status %d\n"
    // NOTE: Exit status comes from the executed command itself (e.g., echo=0, true=0, false=1)
    if(WIFEXITED(status)){
        int exit_code = WEXITSTATUS(status);
        printf("Child <%d> exited with status %d\n", pid, exit_code);
    }
    // TODO: Check if child was terminated by  SIGALRM signal using WIFSIGNALED(status). If yes,
    // 1) TODO: Extract signal number using WTERMSIG(status)
    // 2) TODO: Print: "Child <%d> terminated by SIGALRM [%d] - execution exceeded 3 seconds\n"
    // 3) HINT: Note that SIGALRM signal number is 14
    else if(WIFSIGNALED(status)){
        int signal_num = WTERMSIG(status);

        if(signal_num == SIGALRM){
            printf("Child <%d> terminated by SIGALRM [14] - execution exceeded 3 seconds\n", pid);
        }
    }

    // If child did not exit normally and was not terminated by SIGALRM:
    //   1) Handle other abnormal termination cases
    //   2) Print: "Child <%d> exited abnormally\n"
    else{
        printf("Child <%d> exited abnormally\n", pid);
    }

    fflush(stdout); // Force final output
}

/*
 * TESTING SCENARIOS:
 * - Short commands (should complete normally): echo, true, false
 * - Long commands (should timeout): sleep 10, sleep 50
 * - Invalid commands (should fail): nonexistent_command
 *
 * EXPECTED OUTPUT PATTERNS:
 * Normal exit: "Child <123> exited with status 0"
 * Timeout: "Child <123> terminated by SIGALRM [14] - execution exceeded 3 seconds"
 * Command not found: "Child <123> exited with status 127"
 *
 * EXIT STATUS EXPLANATION:
 * - Status 0: Successful command execution (echo, true, sleep 1)
 * - Status 1: Command executed but failed (false command)
 * - Status 127: Command not found or execvp() failed
 * - SIGALRM termination: Command exceeded 3-second timeout
 */

 /*
 * ⚠️ NOTE:
 * In the examples above, <123> represents the actual process ID (PID),
 * which will be a real number (e.g., 7615, 7616).
 * The autograder normalizes all PIDs to <PID> for comparison.
 * For example, your output:
 *   "Child <7615> exited with status 0"
 * will match the expected pattern:
 *   "Child <PID> exited with status 0"
 */