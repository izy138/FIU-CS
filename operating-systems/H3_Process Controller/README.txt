# Student Information
- Full Name: Isabella Correa
- PID: 6043518
- Section: U01
- FIU Email: icorr014@fiu.edu

# Homework: Process Controller

I used the professors resources and followed the instructions of the lab closely.
signal_handler - checks if the sign is the alarm signal SIGALRM and exits with 124.
pid_t pid = fork(); - duplicates the process into a child process and returns 0, and -1 if failed.
signal(SIGALRM,signal_handler) - sets the timeout mechanism, it occurs in the child process before a command executes
alarm(3); - sets a 3 second timer for the SIGALRM to be sent
execvp(args[0], args); - loads the progress and replaces the childs process with the command target, only returns when failed.
waitpid() - blocks the parent process until the child process is done
WIFEXITED(status) - checks the status of child to make sure it exited
WIFSIGNALED(status) - checks that the child process was terminated by the signal

child sequence:
signal(SIGALRM,signal_handler)
alarm(3)
execvp(args[0], args)
exit(127)

parent sequence:
pid_t pid = fork();
-child process starts-
waitpid()
WIFEXITED(status)
WIFSIGNALED(status)

Info for this lab was taken from the professors slides:
Process Creation: slides 37-56
Signal Handling: slides 60-65