# Student Information
- Full Name: Isabella Correa
- PID: 6043518
- Section: U01
- FIU Email: icorr014@fiu.edu

# Homework: FCFS CPU Scheduler
[Brief description of your implementation approach]
I implemented the schedule using arrays. The program creates a new local copy of the processes array to sort the processes without affecting the original process array. The sorting is done using bubble sort and sorts by arrival time first and then uses the PID as the tie breaker if the processes happen to arrive at the same time. 
Then the sceduler executes the processes in sequence using the sorting algorithms results, and it tracks the CPU time to calculate the time it takes to complete each process.

[Explain how you implemented the FCFS scheduling algorithm]
The scheduling algortihm is first come first serve using the arrival time and pid to sort them in order. It tracts the time the process arrives, and its burst time which is how long the process needs to run. then it calculates when the process is finished executing and this is saved in order to display the time info in a table. When a new process arrives the process starts again, saving the time it takes for each process to run and finish. 

[Describe how you handled tie-breaking when processes have same arrival time]
The processes are sorted by arrival time but in the case they both arrive at the same time then it needs to be sorted by the PID in ascending order.
