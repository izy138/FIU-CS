/*
 * ===============================================================================
 * FIRST-COME, FIRST-SERVED (FCFS) SCHEDULING ALGORITHM - SKELETON
 * ===============================================================================
 * 
 * ALGORITHM OVERVIEW:
 * -------------------
 * First-Come, First-Served (FCFS) is a non-preemptive scheduling algorithm 
 * where processes are executed in the order they arrive in the ready queue.
 * 
 * KEY CHARACTERISTICS:
 * - Non-preemptive: Once a process starts execution, it runs to completion
 * - Simple to implement and understand
 * - May cause the "convoy effect" (short processes waiting for long ones)
 * 
 * TIE-BREAKING RULE:
 * ------------------
 * When multiple processes have the same arrival time:
 * 1. Sort by Arrival Time (primary)
 * 2. If arrival times are equal, sort by Process ID (secondary)
 * 
 * IMPORTANT FORMULAS (as per lecture slides):
 * -------------------------------------------
 * - Completion Time = Time when process finishes execution
 * - Turnaround Time (TAT) = Completion Time - Arrival Time
 * - Waiting Time (WT) = Turnaround Time - Burst Time
 * 
 * ALGORITHM STEPS:
 * ----------------
 * 1. Reset all process states (use reset_process_states function)
 * 2. Sort processes by arrival time (apply tie-breaking rules)
 *    HINT: Create a LOCAL copy of processes array for sorting
 *          DO NOT modify the global ctx->processes array order
 * 3. Initialize current_time = 0
 * 4. For each process in sorted order:
 *    a. If CPU is idle (current_time < arrival_time), jump to arrival_time
 *    b. Execute the process: current_time += burst_time
 *    c. Record completion_time for this process
 *    d. Update current_time
 * 
 * IMPLEMENTATION HINTS:
 * ---------------------
 * - Use bubble sort or selection sort for sorting processes
 * - Remember to handle CPU idle time between process arrivals
 * - Store completion_time in the ORIGINAL ctx->processes array
 * - The display_results function will calculate turnaround and waiting times
 * 
 * COMMON PITFALLS TO AVOID:
 * -------------------------
 * - Don't modify    the global processes array order (use local copy)
 * - Don't calculate waiting_time or turnaround_time directly
 * - Don't forget to handle CPU idle periods
 * - Ensure tie-breaking follows: arrival_time → pid
 * 
 * ===============================================================================
 */

#include "CPU_scheduler.h"

/* ========================================================================================*/
/**
 * @brief First-Come, First-Served (FCFS) Scheduling Algorithm
 * 
 * @param ctx Pointer to the scheduler context containing all process information
 * 
 * This function implements the FCFS scheduling algorithm. Processes are executed
 * in the order of their arrival time. When arrival times are equal, the process
 * with the smaller PID is executed first.
 */
void first_come_first_served(SchedulerContext *ctx)
{
    // Step 1: Reset all process states to initial values
    reset_process_states(ctx);

    // Step 2: Create a LOCAL copy of processes for sorting
    // IMPORTANT: Do NOT modify ctx->processes array order directly
    Process local_processes[MAX_PROCESSES];
    // TODO: Copy all processes from ctx->processes to local_processes
    for (int i = 0; i < ctx->num_processes; i++)
    {
        // copy each process into the local array so sorting does not affect the original order
        local_processes[i] = ctx->processes[i];
    }

    // Step 3: Sort LOCAL copy by arrival time, then by PID for tie-breaking
    // TODO: Implement sorting logic
    // HINT: Use nested loops for bubble sort or selection sort  
    for (int i = 0; i < ctx->num_processes - 1; i++)
    {
        for (int j = 0; j < ctx->num_processes - i - 1; j++)
        {
            bool swap = false;

            if (local_processes[j].arrival_time > local_processes[j + 1].arrival_time)
            {
                // swap if the current process arrives later than the next process
                swap = true;
            }
            else if (local_processes[j].arrival_time == local_processes[j + 1].arrival_time &&
                     local_processes[j].pid > local_processes[j + 1].pid)
            {
                // tie breaks by picking the smaller PID first
                swap = true;
            }

            if (swap)
            {
                // swap to keep the sorted order
                Process temp = local_processes[j];
                local_processes[j] = local_processes[j + 1];
                local_processes[j + 1] = temp;
            }
        }
    }

    // Step 4: Execute processes in FCFS order
    int current_time = 0;

    for (int i = 0; i < ctx->num_processes; i++)
    {
        // TODO: Handle CPU idle time if needed        
        if (current_time < local_processes[i].arrival_time)
        {
            // sets the clock to when the next process arrives
            current_time = local_processes[i].arrival_time;
        }

        // TODO: Calculate completion time for this process       
        current_time += local_processes[i].burst_time;
        //completion time is the moment the CPU finishes running the process. adds the time to the current time
        local_processes[i].completion_time = current_time;

        // TODO: Find the corresponding process in the GLOBAL array (ctx->processes)
        //       and update its completion_time
        // HINT: Search by PID to find the correct process
        for (int j = 0; j < ctx->num_processes; j++)
        {
            if (ctx->processes[j].pid == local_processes[i].pid)
            {
                // update the global copy so later calculations use the correct completion time
                ctx->processes[j].completion_time = local_processes[i].completion_time;
                break;
            }
        }

        // TODO: Move current_time forward
        //         current_time += local_processes[i].burst_time; done above

    }

    // Step 5: Display results (this function is already implemented)
    display_results(ctx, "First-Come-First-Served (FCFS)");
}

/* ========================================================================================*/
/*
 * TESTING YOUR IMPLEMENTATION:
 * ----------------------------
 * 1. Compile: make
 * 2. Run: ./scheduler < Testing/Testcases/input1.txt
 * 3. Compare output with Testing/Expected_output/output1.txt
 * 
 * DEBUGGING TIPS:
 * ---------------
 * - Print the sorted order to verify correct sorting
 * - Print current_time and completion_time for each process
 * - Verify that completion times match expected values
 * - Check if CPU idle time is handled correctly
 */