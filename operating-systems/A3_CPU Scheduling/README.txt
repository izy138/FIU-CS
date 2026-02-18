# Student Information
- Full Name: Isabella Correa
- PID: 6043518
- Section: U01
- FIU Email: icorr014@fiu.edu

# Assignment 3

first_come_first_served.c  
-Executes processes by arrival order using a locally sorted copy so the original context is unchanged. Accounts for CPU idle time by jumping the clock to the next arrival. Completion times are written back to the global context for later turnaround/waiting calculations.

shortest_job_first.c  
-Continuously scans the arrived processes to pick the shortest burst time and then tie breaks by arrival time and PID. When no job is ready it advances time to the next arrival.

shortest_remaining_time_first.c  
- Preemptive version of shortest job first. It re-evaluates every time unit to favor the job with the least amount of work left to finish. Fast forwards to the next arrival when its idleand updates completion times when a process finishes.

round_robin.c  
- Uses a a circular ready queue. First sorted by arrival then PID, to share the CPU time quantum. Inserts processes that arrive mid quantum time before re-queueing the in progress process. Tracks remaining time, completion, and queue so nothing is skipped or redone.

priority_non_preemptive.c  
-Chooses processes based on lowest numeric priority, tie breaks with arrival time and PID. If no process is ready, it advances thetime to the next arrival. Process will run to completion and then its completion time is recorded

priority_preemptive_rr.c  
-Buildslist of highest priority processes at each time step and orders them by arrival then PID for tie break. Runs the processes using round robin, but checks every time tick for any higher priority arrivals so it can go through the process again preemptively. 

To build:
make clean, make commands

set permissions for autograding:
chmod +x autograder_scheduler.sh
chmod +x batchgrader_scheduler.sh

test:
./autograder_scheduler.sh
