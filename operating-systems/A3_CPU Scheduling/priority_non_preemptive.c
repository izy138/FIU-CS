/*
 * ===============================================================================
 * PRIORITY SCHEDULING - NON-PREEMPTIVE ALGORITHM - SKELETON
 * ===============================================================================
 * 
 * ALGORITHM OVERVIEW:
 * -------------------
 * Priority Scheduling is a scheduling algorithm where each process is assigned
 * a priority value. The process with the highest priority (lowest priority number)
 * is selected for execution. This is the non-preemptive version - once a process
 * starts execution, it runs to completion.
 * 
 * KEY CHARACTERISTICS:
 * - Non-preemptive: Once a process starts, it runs to completion
 * - Priority-based selection: Lower priority number = Higher priority
 * - Can cause starvation for low-priority processes
 * - Widely used in real-time systems and operating systems
 * 
 * PRIORITY CONVENTION:
 * --------------------
 * IMPORTANT: Lower priority number = Higher priority
 * Example: Priority 1 > Priority 2 > Priority 3 > Priority 5
 * 
 * TIE-BREAKING RULE:
 * ------------------
 * When multiple processes have the same priority value:
 * 1. Priority Value (primary) - select lowest priority number (highest priority)
 * 2. If priorities are equal, Arrival Time (secondary) - select earliest arrival
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
 *    a. Find the process with highest priority (lowest priority number) among arrived processes
 *       - Apply tie-breaking rules: priority → arrival_time → pid
 *    b. If no process is ready:
 *       - CPU is idle
 *       - Jump to next arrival time
 *    c. If process found:
 *       - Execute it to completion (non-preemptive)
 *       - Update current_time
 *       - Record completion_time
 *       - Mark as completed
 *       - Increment completed counter
 * 5. Call display_results with algorithm name "PRIORITY_NP"
 * 
 * IMPLEMENTATION HINTS:
 * ---------------------
 * - Use a loop to search for the process with minimum priority number
 * - Remember: Lower number = Higher priority (Priority 1 > Priority 5)
 * - Check if process has arrived and is not completed
 * - Apply tie-breaking carefully when priorities are equal
 * - Handle CPU idle time when no process is ready
 * 
 * COMMON PITFALLS TO AVOID:
 * -------------------------
 * - Don't confuse priority convention (lower number = higher priority)
 * - Don't forget to apply all three tie-breaking criteria
 * - Remember this is non-preemptive (no interruption)
 * - Don't calculate waiting_time directly
 * - Handle CPU idle periods correctly
 * 
 * ===============================================================================
 */

#include "CPU_scheduler.h" 

/* ========================================================================================*/
/**
 * @brief Priority Scheduling - Non-Preemptive Algorithm
 * 
 * @param ctx Pointer to the scheduler context containing all process information
 * 
 * This function implements the non-preemptive priority scheduling algorithm.
 * At each scheduling decision, it selects the process with the highest priority
 * (lowest priority number) among all arrived processes. The selected process
 * runs to completion without preemption.
 */
void priority_non_preemptive(SchedulerContext *ctx)
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
        int best_idx = -1;  // Index of the process with highest priority (lowest priority number)

        // Step 4: Select the process with highest priority (lowest priority number) that has arrived
        // TODO: Loop through all processes
        int best_priority = INT_MAX;

        for (int i = 0; i < ctx->num_processes; i++)
        {
            // TODO: Check if process has arrived and is not completed   

            if (ctx->processes[i].arrival_time <= current_time && !is_completed[i])
            {
                // TODO: Apply tie-breaking logic to select the best process
                // TIE-BREAKING ORDER: priority (lower is better) → arrival_time → pid
                //sorts by priority first
                if (ctx->processes[i].priority < best_priority)
                {
                    // found process with a higher priority which should be the smallest number
                    best_priority = ctx->processes[i].priority;
                    best_idx = i;
                }
                // if the priority is the same, sort by arrival time
                else if (ctx->processes[i].priority == best_priority && best_idx != -1)
                {
                    if (ctx->processes[i].arrival_time < ctx->processes[best_idx].arrival_time ||
                        (ctx->processes[i].arrival_time == ctx->processes[best_idx].arrival_time &&
                         ctx->processes[i].pid < ctx->processes[best_idx].pid))
                    {
                        // resolve tie break using arrival time and PID ordering
                        best_idx = i;
                    }
                }
                else if (best_idx == -1)
                {
                    //update the best index and priority if found
                    best_idx = i;
                    best_priority = ctx->processes[i].priority;
                }
            }
        }

        // Step 5: Handle the selected process or CPU idle time
        if (best_idx == -1)
        {
            // No process is ready - CPU idle, jump to next arrival
            // TODO: Find the next arrival time among incomplete processes
            // HINT: Loop through all processes to find minimum arrival_time > current_time
            int next_arrival = INT_MAX;

            // TODO: Jump current_time to next_arrival
            for (int i = 0; i < ctx->num_processes; i++)
            {
                if (!is_completed[i] && ctx->processes[i].arrival_time > current_time)
                {
                    if (ctx->processes[i].arrival_time < next_arrival)
                    {
                        //choose the earliest next arrival to resume the scheduling
                        next_arrival = ctx->processes[i].arrival_time;
                    }
                }
            }

            if (next_arrival == INT_MAX)
            {
                break;
            }

            //set the current time to the next arrival time
            current_time = next_arrival;
        }
        else
        {
            // Execute the selected process to completion (non-preemptive)
            // TODO: Update current_time by adding burst_time of selected process
            if (current_time < ctx->processes[best_idx].arrival_time)
            {
                
                //set the current time to the arrival time of the process
                current_time = ctx->processes[best_idx].arrival_time;
            }

            // run process for full CPU burst
            current_time += ctx->processes[best_idx].burst_time;

            // TODO: Record the completion_time for this process
            // HINT: ctx->processes[best_idx].completion_time = current_time;
            // store completion time for later 
            ctx->processes[best_idx].completion_time = current_time;

            // TODO: Mark this process as completed
            // prevent the process from being considered again
            is_completed[best_idx] = true;

            // TODO: Increment the completed counter
            //track number of processes finished
            completed++;
        }
    }

    // Step 6: Display results
    display_results(ctx, "PRIORITY_NON_PREEMPTIVE");
}

/* ========================================================================================*/
/*
 * TESTING YOUR IMPLEMENTATION:
 * ----------------------------
 * 1. Compile: make
 * 2. Run: ./scheduler < Testing/Testcases/input1.txt
 * 3. Compare output with Testing/Expected_output/output1.txt
 *  
 * PRIORITY CONVENTION EXAMPLE:
 * ----------------------------
 * Priority values and their meaning:
 *   Priority 1 = HIGHEST priority (executes first)
 *   Priority 2 = High priority
 *   Priority 3 = Medium priority
 *   Priority 4 = Low priority
 *   Priority 5 = LOWEST priority (executes last)
 * 
 * TIE-BREAKING EXAMPLE:
 * ---------------------
 * If at time 10, three processes have the same priority:
 *   P1: Arrival=2, Priority=3
 *   P2: Arrival=5, Priority=3
 *   P3: Arrival=2, Priority=3, PID=3
 *   
 * Selection order:
 * 1. All have priority=3 (tie)
 * 2. P1 and P3 arrived at time 2, P2 arrived at time 5
 *    → P2 is eliminated
 * 3. Between P1 and P3: same priority, same arrival
 *    → Select P1 (PID 1 < PID 3)
 * 
 * DEBUGGING TIPS:
 * ---------------
 * - Print which process is selected at each scheduling decision
 * - Print the priority value being compared
 * - Remember: Lower priority number = Higher priority
 * - Verify tie-breaking follows: priority → arrival_time → pid
 * - Check that only arrived and incomplete processes are considered
 * - Trace execution timeline to match expected output
 * - Verify completion times are calculated correctly
 * 
 * COMMON MISTAKES:
 * ----------------
 * - Confusing priority convention (selecting higher number instead of lower)
 * - Not applying all three tie-breaking criteria in correct order
 * - Forgetting to check if process has arrived before selecting
 * - Calculating waiting_time directly instead of using completion_time
 * - Not handling CPU idle time when no process is ready
 * 
 * STARVATION EXAMPLE:
 * -------------------
 * Consider this scenario:
 *   P1: Arrival=0, Burst=100, Priority=5 (LOW)
 *   P2: Arrival=10, Burst=1, Priority=1 (HIGH)
 *   P3: Arrival=20, Burst=1, Priority=1 (HIGH)
 *   
 * If this were preemptive, P1 would never complete (starvation).
 * But since this is NON-PREEMPTIVE, P1 completes first, then P2, then P3.
 * Non-preemptive priority scheduling reduces (but doesn't eliminate) starvation risk.
 */