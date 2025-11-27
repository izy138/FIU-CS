# Student Information
- Full Name: Isabella Correa
- PID: 6043518
- Section: U01
- FIU Email: icorr014@fiu.edu

# Assignment 2 
I implemented a multi threaded TCP server, which listens on port 8888 and accepts multiple clients sending requests to the server. The client connections work by created a thread for each client. When a command is sent by a client, the server will fork a child process and execute it. The parent process monitors the child process and after executing, the server reads the output and send it back to the client and signals the processes completeion.