/*
 * ===============================================================================
 * SHORTEST JOB FIRST (SJF) - NON-PREEMPTIVE SCHEDULING ALGORITHM - SKELETON
 * ===============================================================================
 * 
 * ALGORITHM OVERVIEW:
 * -------------------
 * Shortest Job First (SJF) is a non-preemptive scheduling algorithm that selects
 * the process with the smallest burst time from the ready queue. It minimizes
 * average waiting time but can cause starvation for longer processes.
 * 
 * KEY CHARACTERISTICS:
 * - Non-preemptive: Once a process starts, it runs to completion
 * - Optimal for minimizing average waiting time
 * - Can cause starvation for processes with longer burst times
 * - Requires knowledge of burst time in advance
 * 
 * TIE-BREAKING RULE:
 * ------------------
 * When multiple processes have the same burst time:
 * 1. Burst Time (primary) - select shortest
 * 2. If burst times are equal, Arrival Time (secondary) - select earliest arrival
 * 3. If both are equal, Process ID (tertiary) - select smallest PID
 * 
 * IMPORTANT FORMULAS (as per lecture slides):
 * -------------------------------------------
 * - Completion Time = Time when process finishes execution
 * - Turnaround Time (TAT) = Completion Time - Arrival Time
 * - Waiting Time (WT) = Turnaround Time - Burst Time
 * 
 * ALGORITHM STEPS:
 * ----------------
 * 1. Reset all process states
 * 2. Initialize current_time = 0, completed = 0
 * 3. Use a tracking array to mark completed processes
 * 4. Main scheduling loop (while completed < num_processes):
 *    a. Find the process with shortest burst time among arrived processes
 *       - Apply tie-breaking rules: burst_time → arrival_time → pid
 *    b. If no process is ready:
 *       - CPU is idle
 *       - Jump to next arrival time
 *    c. If process found:
 *       - Execute it to completion (non-preemptive)
 *       - Update current_time
 *       - Record completion_time
 *       - Mark as completed
 *       - Increment completed counter
 * 5. Call display_results with algorithm name "SJF"
 * 
 * IMPLEMENTATION HINTS:
 * ---------------------
 * - Use a loop to search for the process with minimum burst time
 * - Check if process has arrived (arrival_time <= current_time)
 * - Check if process is not already completed
 * - Apply tie-breaking carefully when burst times are equal
 * - Handle CPU idle time when no process is ready
 * 
 * COMMON PITFALLS TO AVOID:
 * -------------------------
 * - Don't sort the entire array - select process dynamically each time
 * - Don't forget to apply all three tie-breaking criteria
 * - Remember this is non-preemptive (no interruption)
 * - Don't calculate waiting_time directly (let display_results do it)
 * - Handle CPU idle periods correctly
 * 
 * ===============================================================================
 */

#include "CPU_scheduler.h"

/* ========================================================================================*/
/**
 * @brief Shortest Job First (SJF) - Non-Preemptive Scheduling Algorithm
 * 
 * @param ctx Pointer to the scheduler context containing all process information
 * 
 * This function implements the SJF scheduling algorithm. At each scheduling decision,
 * it selects the process with the shortest burst time among all arrived processes.
 * The selected process runs to completion without preemption.
 */
void shortest_job_first(SchedulerContext *ctx)
{
    // Step 1: Reset all process states
    reset_process_states(ctx);

    // Step 2: Initialize variables
    int completed = 0;                           // Count of completed processes
    int current_time = 0;                        // Current time in the simulation
    bool is_completed[MAX_PROCESSES] = {false};  // Track which processes are completed

    // Step 3: Main scheduling loop
    while (completed < ctx->num_processes)
    {
        int best_idx = -1;              // Index of the selected process
        int min_burst_time = INT_MAX;   // Minimum burst time found so far

        // Step 4: Find the process with shortest burst time that has arrived
        // TODO: Loop through all processes
        for (int i = 0; i < ctx->num_processes; i++)
        {
            // TODO: Check if process has arrived and is not completed
            //process arrived and is not completed
            if (ctx->processes[i].arrival_time <= current_time && !is_completed[i])
            {
                // TODO: Apply tie-breaking logic to select the best process
                // TIE-BREAKING ORDER: burst_time → arrival_time → pid
                //picks the process with the shortest burst time
                if (ctx->processes[i].burst_time < min_burst_time)
                {
                    // found a new shortest burst time among the arrived processes
                    min_burst_time = ctx->processes[i].burst_time;
                    best_idx = i;
                }
                //if the burst time is the same, tie breaks by arrival time and then pid
                else if (ctx->processes[i].burst_time == min_burst_time)
                {
                    //tie breaks by earliestarrival time and then by smallest pid
                    if (ctx->processes[i].arrival_time < ctx->processes[best_idx].arrival_time ||
                        (ctx->processes[i].arrival_time == ctx->processes[best_idx].arrival_time &&
                         ctx->processes[i].pid < ctx->processes[best_idx].pid))
                    {
                        
                        best_idx = i;
                    }
                }
            }
        }

        // Step 5: Handle the selected process or CPU idle time
        if (best_idx == -1)
        {
            // No process is ready - CPU is idle
            // TODO: Find the next arrival time among incomplete processes
            // HINT: Loop through all processes to find minimum arrival_time > current_time
            int next_arrival = INT_MAX;

            // TODO: Jump current_time to next_arrival
            for (int i = 0; i < ctx->num_processes; i++)
            {
                //only checks the processes that have not completed and have arrived after the current time
                if (!is_completed[i] && ctx->processes[i].arrival_time > current_time)
                {
                    if (ctx->processes[i].arrival_time < next_arrival)
                    {
                        //tracks the soonest future arrival to resume scheduling
                        next_arrival = ctx->processes[i].arrival_time;
                    }
                }
            }
            //if no new arrival time is found, break the loop
            if (next_arrival == INT_MAX)
            {
                break;
            }
            //sets the current time to the next arrival time
            current_time = next_arrival;
        }
        else
        {
            // Process found - execute it to completion
            // TODO: Update current_time by adding burst_time of selected process
            //if the current time is before the arrival time of the selected process
            if (current_time < ctx->processes[best_idx].arrival_time)
            {
                //if CPU was idle before this process arrived, fast-forward time
                current_time = ctx->processes[best_idx].arrival_time;
            }

            // adds the burst time to simulate running the process to completion
            current_time += ctx->processes[best_idx].burst_time;

            // TODO: Record the completion_time for this process
            // HINT: ctx->processes[best_idx].completion_time = current_time;
            // store when the process is finished
            ctx->processes[best_idx].completion_time = current_time;

            // TODO: Mark this process as completed
            // sets the process as completed so its not used in the future
            is_completed[best_idx] = true;

            // TODO: Increment the completed counter
            //count the number of processes finished
            completed++;
        }
    }

    // Step 6: Display results
    display_results(ctx, "Shortest-Job-First (SJF)");
}

/* ========================================================================================*/
/*
 * TESTING YOUR IMPLEMENTATION:
 * ----------------------------
 * 1. Compile: make
 * 2. Run: ./scheduler < Testing/Testcases/input1.txt
 * 3. Compare output with Testing/Expected_output/output1.txt
 *
 * TIE-BREAKING EXAMPLE:
 * ---------------------
 * If two processes have the same burst time:
 *   P1: Arrival=0, Burst=5
 *   P2: Arrival=2, Burst=5
 *   P3: Arrival=2, Burst=5, PID=3
 *   
 * At time 0: Select P1 (only arrived process)
 * At time 5: Both P2 and P3 have burst=5
 *            Select P2 (same burst, same arrival, smaller PID)
 * 
 * DEBUGGING TIPS:
 * ---------------
 * - Print which process is selected at each scheduling decision
 * - Verify that tie-breaking follows: burst_time → arrival_time → pid
 * - Check that only arrived and incomplete processes are considered
 * - Ensure CPU idle time is handled when no process is ready
 * - Trace the execution timeline to match expected output
 * - Verify completion times are calculated correctly
 * 
 * COMMON MISTAKES:
 * ----------------
 * - Forgetting to check if process has arrived before selecting
 * - Not applying all three tie-breaking criteria in correct order
 * - Sorting the entire array instead of dynamic selection
 * - Calculating waiting_time directly instead of using completion_time
 */