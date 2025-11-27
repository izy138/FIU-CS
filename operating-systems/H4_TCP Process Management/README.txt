# Student Information
- Full Name: Isabella Correa
- PID: 6043518
- Section: U01
- FIU Email: icorr014@fiu.edu

# Homework: TCP Command Server
[Brief description of your implementation approach]
I implemented the TCP server that listens on port 8080 and accepts client connections. The server receives commands from the client and processes them in a loop until the client disconnects.

[Mention how you implemented the fork-per-command architecture]
For each command received from the client, the server forks a new child process. The child process executes the command using system(), and checks the return value to determine success or failure. The server sends the status messages back to the client, and then exits. The parent process waits for each child process to finish using waitpid() before it goes and processes the next command.

[Describe any challenges faced and how you solved them]
The main challenge was ensuring proper socket handling across parent and child processes. I solved this by having the child close the socket after sending the status message and calling exit() to prevent returning. 

Another challenge wasformatting the status messages to match the expected output exactly, which I addressed by using snprintf().