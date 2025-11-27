# Student Information
- Full Name: Isabella Correa
- PID: 6043518
- Section: U01
- FIU Email: icorr014@fiu.edu

# Homework: Round-Robin CPU Scheduler

[Brief description of your implementation approach]
I implemented the Round Robin scheduler algorithm using circular queues using FIFO. The algorithm schedules the processes by using a fixed time quantum that has to execute before it moves it to the back of the queue if the process is incomplete. There are 3 arrays to track the remaining time, completed tasks, and ready tasks in queue. These help to manage the processes execution. it sorts through the processes by arrival time and PID.

[Explain how you implemented the Round-Robin scheduling algorithm]
-The input is validated and the state is reset, then three arrays keep track of remaining time, completed tasks, and ready tasks in queue for the processes.
-Then the processes go through bubble sort sorting by their arrival time and then the tie breaker is the PID. The main loop dequeus the process from queue, exectutes it and checks for the process completion status. Then it enqueues new arrived processes and re-enqueues the current process if its not finished executing. 
-It checks if the ready queues is empty and increase the time for the next process by finding the minimum arrival time for the correct incomplete processes. It continues until all the processes finish and displays.

[Describe how you managed the ready queue and time  quantum]
- The ready queue is a circular queue where the rear points to front, and processes advance through the queue. For time quantym, the execution time is calculated by using the minumu value between the processes remaining time and the set time quantum which determines how long the process runs for. -After executing, if the remaining time reaches 0 then the process is marked as complete and the time is recorded into the completion time array.

[Explain how you handled process arrivals during execution]
- The proccesses are looped to find those with the arrival time greater than start time and current time, which means they arrived during execution. Each process is checked for completion, and if its already in queue. If its not the current executing process it enqueues it. All new arrived processes are enqueues first, and then it checks the current process and its remaining tima, and the re enqueues it as last in the queue.

[Describe how you handled tie-breaking when processes have same arrival time]
During bubble sort for the processes are ordered first by comparing their arrival times in ascending order. If any processes have the same arrival time, they are tie broken by comparing PID in ascending order. The processes are swapped before the main loop so they are in order for execution.

