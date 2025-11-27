/* 
 * HOMEWORK-5: FCFS CPU Scheduler
 * Student Implementation Template
 *
 * INSTRUCTIONS:
 * Complete the implementation of the fcfs_scheduling() function below.
 * Follow the FCFS algorithm specifications exactly as provided.
 *
 * REQUIREMENTS:
 * - Implement First-Come, First-Served (FCFS) scheduling algorithm
 * - Sort processes by arrival time (use PID for tie-breaking)
 * - Calculate waiting time for each process
 * - Handle CPU idle time when no process is ready
 * - Do NOT modify the global ctx->processes[] array order
 * - Create a local copy for sorting if needed
 * - Update waiting times in the original global array
 *  
 * 💡TIE-BREAKING RULE:
 * - Primary: Arrival Time (ascending - earlier arrivals first)
 * - Secondary: Process ID (ascending - lower PID first)
 * - Example: If P3 and P1 both arrive at time 0, schedule P1 first
 *
 * 👍FORMULAS (as per lecture slides):
 * - Turnaround Time (TAT) = Completion Time - Arrival Time
 * - Waiting Time (WT) = Turnaround Time - CPU Burst Time
 * - Average WT = Sum of all WT / Number of Processes
 * - Average TAT = Sum of all TAT / Number of Processes
 *
 * NOTE: You only need to calculate and store completion_time for each process.
 *       The framework will automatically calculate TAT and WT using the formulas above.
 *
 * ⚠️ GRADING CRITERIA:
 * - Correct FCFS implementation with proper tie-breaking
 * - Accurate completion time calculation for all processes
 * - Proper handling of CPU idle time
 * - Do not modify global process array order
 * - Exact output format matching expected results
 * - Code must compile without errors or warnings
 */

#include "fcfs_scheduler.h"

/**
 * ===============================================================================
 * FCFS Scheduling Algorithm Implementation
 * ===============================================================================
 *
 * @brief Implements the First-Come, First-Served scheduling algorithm
 *
 * @param ctx Pointer to SchedulerContext containing process information
 *
 * ALGORITHM STEPS:
 * 1. Reset all process states using reset_process_states(ctx)
 * 2. Create a local copy of processes for sorting
 * 3. Sort local copy by arrival time, then by PID (do NOT modify global array)
 * 4. Simulate FCFS scheduling *    
 * 5. Call display_results(ctx, "FCFS") to output results
 *
 * IMPORTANT NOTES:
 * - Do NOT sort or modify the global ctx->processes[] array
 * - Only update the completion_time field in the global array
 * - Use a local copy for determining execution order
 * - The framework calculates TAT and WT from completion_time using:
 *   TAT = Completion Time - Arrival Time
 *   WT = TAT - Burst Time
 * - Ensure proper tie-breaking: Arrival Time → Process ID
 *
 * ===============================================================================
 */
void fcfs_scheduling(SchedulerContext *ctx)
{
    // Step1️⃣: Reset process states (clears previous scheduling data)
    reset_process_states(ctx);
    
    // Step2️⃣: Create a local copy of processes for sorting   
    // TODO: Declare a local Process array to hold a copy of processes
    // HINT: Process local_processes[MAX_PROCESSES];    
    // TODO: Copy all processes from ctx->processes[] to local_processes[]  
    
    //this is used to sort the processes by arrival time 
    Process local_processes[MAX_PROCESSES];
    
    // copies each process from the global array to the local process array
    for (int i = 0; i < ctx->num_processes; i++) {
        local_processes[i] = ctx->processes[i];
    }
   
    // Step3️⃣: Sort the LOCAL copy by arrival time, then by PID  
    // TODO: Implement bubble sort or selection sort on local_processes[]
    // SORTING LOGIC:
    // - Primary sort key: arrival_time (ascending order)
    // - Secondary sort key: pid (ascending order for tie-breaking)
    // HINT: Use nested loops to compare and swap processes
    
    // bubble sort to sort the processes by arrival time
    for (int i = 0; i < ctx->num_processes - 1; i++) {
        for (int j = 0; j < ctx->num_processes - i - 1; j++) {
            // compare arrival times first, then PIDs for tie-breaking
            bool swap = false;
            
            //if the currennt process has a later arrival time than the next process, swap them
            if (local_processes[j].arrival_time > local_processes[j + 1].arrival_time) {
                swap = true;

                // if the arrival times are the same, compares the PIDs
            } else if (local_processes[j].arrival_time == local_processes[j + 1].arrival_time) {
                // use PID in ascending order for tie-breaking
                if (local_processes[j].pid > local_processes[j + 1].pid) {
                    swap = true;
                }
            }
            
            //swaps the processes if they need to be swapped
            if (swap) {
                Process temp = local_processes[j];
                local_processes[j] = local_processes[j + 1];
                local_processes[j + 1] = temp;
            }
        }
    }
    
    // Step4️⃣: Simulate FCFS scheduling and calculate completion times    
    // TODO: Initialize current_time to 0
    // HINT: int current_time = 0;

    // tracks the cpu time
    int current_time = 0;
    
    // TODO: Loop through each process in the sorted LOCAL array
    for (int i = 0; i < ctx->num_processes; i++) {
        // Step 4a: Handle CPU idle time        
        // TODO: If current_time is less than the process's arrival_time,
        //       update current_time to match the arrival_time (CPU waits)

        // if the process has not arrived yet, the cpu waits until it does
        if (current_time < local_processes[i].arrival_time) {
            current_time = local_processes[i].arrival_time;
        }
        
        // Step 4b: Calculate completion time for this process      
        // TODO: Calculate when this process will complete

        //current time is when the process starts running
        //and burst time is how long the process takes to finish
        int completion_time = current_time + local_processes[i].burst_time;
        
        // Step 4c: Update completion_time in the GLOBAL array        
        // TODO: Find the matching process in ctx->processes[] by PID
        //       and update its completion_time field
        // HINT: Use a loop to search for the process with matching PID

        // find the process in the global array by PID and update its completion time
        for (int j = 0; j < ctx->num_processes; j++) {
            if (ctx->processes[j].pid == local_processes[i].pid) {
                ctx->processes[j].completion_time = completion_time;
                break;
            }
        }
        
        // Step 4d: Update current_time to the completion_time
        // TODO: Move current_time forward to when this process completes
        // HINT: current_time = completion_time;
        current_time = completion_time;
    }
   
    // Step5️⃣: Display results
    display_results(ctx, "FCFS");
}


/*
 *
 * Common Mistakes to Avoid:
 * -------------------------
 * ❌ Modifying the global array order (only update completion_time field!)
 * ❌ Forgetting to handle CPU idle time
 * ❌ Incorrect tie-breaking (should use PID as secondary key)
 * ❌ Updating wrong process in global array (match by PID, not index!)
 * ❌ Not resetting process states at the beginning
 * ❌ Forgetting to call display_results() at the end
 * 
 */