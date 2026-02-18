/*
 * ===============================================================================
 * PRIORITY PREEMPTIVE WITH ROUND ROBIN TIE-BREAKING - SKELETON
 * ===============================================================================
 * 
 * ALGORITHM OVERVIEW:
 * -------------------
 * This is a complex scheduling algorithm that combines priority-based preemptive
 * scheduling with Round Robin for tie-breaking. The process with the highest
 * priority (lowest priority number) executes. When multiple processes share the
 * same highest priority, Round Robin with time quantum is used among them.
 * 
 * KEY CHARACTERISTICS:
 * - Preemptive: Higher priority arrival causes IMMEDIATE preemption
 * - Priority-based: Lower priority number = Higher priority
 * - Round Robin for ties: Processes with same priority use RR scheduling
 * - Time quantum: Used only when multiple processes have same priority
 * 
 * PRIORITY CONVENTION:
 * --------------------
 * IMPORTANT: Lower priority number = Higher priority
 * Example: Priority 1 > Priority 2 > Priority 3
 * 
 * BEHAVIOR (as per lecture slides):
 * ---------------------------------
 * 1. Always execute the process(es) with highest priority (lowest number)
 * 2. If multiple processes have the same highest priority, use Round Robin among them
 * 3. When a higher priority process arrives, immediately preempt current process
 * 4. When preempted process's priority becomes highest again, it continues execution
 * 
 * TIE-BREAKING RULE:
 * ------------------
 * 1. Priority Value (primary) - select lowest priority number
 * 2. If same priority, use Round Robin with time quantum (secondary)
 * 3. Process ID for ordering within same priority level (tertiary)
 * 
 * IMPORTANT FORMULAS:
 * -------------------
 * - Completion Time = Time when process finishes execution
 * - Turnaround Time (TAT) = Completion Time - Arrival Time
 * - Waiting Time (WT) = Turnaround Time - Burst Time
 * 
 * ALGORITHM STEPS:
 * ----------------
 * 1. Reset all process states
 * 2. Initialize current_time = 0, completed = 0
 * 3. Main scheduling loop (while completed < num_processes):
 *    a. Find the highest priority (minimum priority number) among arrived processes
 *    b. Collect all processes with that highest priority into ready_processes array
 *    c. If no processes ready:
 *       - CPU is idle, jump to next arrival
 *    d. If processes ready:
 *       - Apply Round Robin among processes with same highest priority
 *       - For each process in the RR cycle:
 *         * Execute for min(time_quantum, remaining_time)
 *         * Check for higher priority arrivals during execution (preemption)
 *         * If process completes, record completion_time
 *         * If higher priority process arrives, break RR cycle and restart
 * 4. Call display_results with algorithm name "PRIORITY_PREEMPTIVE_RR"
 * 
 * PREEMPTION RULES:
 * -----------------
 * - Preemption occurs when a process with HIGHER priority (lower number) arrives
 * - Preemption is checked during execution (time unit by time unit)
 * - When preempted, the current process is suspended until its priority is highest again
 * 
 * IMPLEMENTATION HINTS:
 * ---------------------
 * - Execute time unit by time unit to check for preemptions
 * - Maintain a list of processes at the current highest priority level
 * - Use Round Robin rotation only among processes with same priority
 * - After executing one process, re-evaluate priorities (new arrivals may change highest)
 * - Use min_value() helper function for calculating execution time
 * 
 * COMMON PITFALLS TO AVOID:
 * -------------------------
 * - Don't forget priority convention (lower number = higher priority)
 * - Check for higher priority arrivals DURING execution, not just before
 * - Round Robin applies only among processes with SAME priority
 * - Re-evaluate priorities after each process execution
 * - Don't execute entire time quantum if higher priority process arrives
 * 
 * ===============================================================================
 */

#include "CPU_scheduler.h" 

/* ========================================================================================*/
/**
 * @brief Priority Preemptive with Round Robin Tie-Breaking
 * 
 * @param ctx Pointer to the scheduler context containing all process information
 * @param time_quantum Time slice for Round Robin among same-priority processes
 * 
 * This function implements priority-based preemptive scheduling with Round Robin
 * used for tie-breaking when multiple processes have the same priority.
 */
void priority_preemptive_rr(SchedulerContext *ctx, int time_quantum)
{
    // Input validation
    if (time_quantum <= 0) {
        fprintf(stderr, "Error: Time quantum must be positive\n");
        return;
    }

    // Step 1: Reset process states
    reset_process_states(ctx);

    // Step 2: Initialize variables
    int current_time = 0;   // Current time in simulation
    int completed = 0;      // Count of completed processes

    // Step 3: Main scheduling loop
    while (completed < ctx->num_processes)
    {
        // Step 4: Find highest priority (minimum priority number) among arrived processes
        int highest_priority = INT_MAX;  // Will store the lowest priority number (highest priority)
        int ready_processes[MAX_PROCESSES];  // Array to store indices of ready processes
        int ready_count = 0;  // Count of processes at highest priority level

        // TODO: Find the highest priority (minimum priority number) among all arrived processes
        // HINT: Loop through all processes to determine the process with highest priority
        for (int i = 0; i < ctx->num_processes; i++)
        {
            if (ctx->processes[i].arrival_time <= current_time &&
                ctx->processes[i].remaining_time > 0)
            {
                if (ctx->processes[i].priority < highest_priority)
                {
                    //new highest priority process first and then reset the ready processlist
                    highest_priority = ctx->processes[i].priority;
                    ready_count = 0;
                    ready_processes[ready_count++] = i;
                }
                else if (ctx->processes[i].priority == highest_priority)
                {
                    // collect additional processes sharing the same priority
                    ready_processes[ready_count++] = i;
                }
            }
        }

        // if there are multiple processes with the same highest priority
        if (ready_count > 1)
        {
            for (int i = 0; i < ready_count - 1; i++)
            {
                for (int j = 0; j < ready_count - i - 1; j++)
                {
                    int idx_a = ready_processes[j];
                    int idx_b = ready_processes[j + 1];
                    bool swap = false;

                    if (ctx->processes[idx_a].arrival_time > ctx->processes[idx_b].arrival_time)
                    {
                        // earliest arrival gets prio
                        swap = true;
                    }
                    else if (ctx->processes[idx_a].arrival_time == ctx->processes[idx_b].arrival_time &&
                             ctx->processes[idx_a].pid > ctx->processes[idx_b].pid)
                    {
                        //if arrival ties, rely on PID order
                        swap = true;
                    }

                    if (swap)
                    {
                        // swap the indices for process order
                        int temp = ready_processes[j];
                        ready_processes[j] = ready_processes[j + 1];
                        ready_processes[j + 1] = temp;
                    }
                }
            }
        }

        // Step 5: If no process ready, jump to next arrival
        if (ready_count == 0)
        {
            // TODO: Find next arrival time
            
            //set the next arrival time to the maximum value
            int next_arrival = INT_MAX;

            // loop through all processes to find the next arrival time that is greater than the current
            for (int i = 0; i < ctx->num_processes; i++)
            {
                if (ctx->processes[i].remaining_time > 0 &&
                    ctx->processes[i].arrival_time > current_time)
                {
                    if (ctx->processes[i].arrival_time < next_arrival)
                    {
                        //track the nearest arrival 
                        next_arrival = ctx->processes[i].arrival_time;
                    }
                }
            }

            // TODO: Jump to next arrival time
            if (next_arrival == INT_MAX) break;
            
            current_time = next_arrival;
            continue;
        }

        // Step 6: Round Robin scheduling for processes with same highest priority
        // Execute each ready process for its time quantum (or remaining time)
        for (int i = 0; i < ready_count && completed < ctx->num_processes; i++)
        {
            int idx = ready_processes[i];
            
            // TODO: Skip if process already completed
            if (ctx->processes[idx].remaining_time <= 0)
            {
                // gnore processes that finished previously
                continue;
            }
          
            // TODO: Calculate time to execute for this process
            // HINT: Execute for minimum of (time_quantum, remaining_time)
            // find how long the process runs during round robin
            int time_to_execute = min_value(time_quantum, ctx->processes[idx].remaining_time);

            // Execute the process time unit by time unit (for preemption check)
            // TODO: Loop for time_to_execute iterations
            bool preempted = false;


            for (int t = 0; t < time_to_execute; t++)
            {
                // TODO: Advance time by 1 unit
                current_time += 1;

                // TODO: Decrease remaining_time by 1
                // reduce amount of work left for process
                ctx->processes[idx].remaining_time -= 1;

                // TODO: Check if a higher priority process has arrived (preemption check)
                // HINT: Loop through all processes
                //       If (arrival_time == current_time && remaining_time > 0 && priority < highest_priority)
                //           Set preempted flag and break
                for (int j = 0; j < ctx->num_processes; j++)
                {
                    //check if a higher priority process has arrived
                    if (ctx->processes[j].arrival_time <= current_time &&
                        ctx->processes[j].remaining_time > 0 &&
                        ctx->processes[j].priority < highest_priority)
                    {
                        preempted = true;
                        break;
                    }
                }

                // TODO: If preempted, break out of execution loop
                if (preempted)
                {
                    break;
                }
            }

            // TODO: Check if process completed
            // HINT: If remaining_time == 0, record completion_time and increment completed
            if (ctx->processes[idx].remaining_time == 0)
            {
                //record completion time and mark as finished
                ctx->processes[idx].completion_time = current_time;
                ctx->processes[idx].is_completed = true;
                completed++;
            }
            
            // TODO: After executing one process, check if higher priority arrived
            // If so, break out of RR loop to re-evaluate priorities in next iteration
            bool higher_priority_arrived = false;
            if (preempted)
            {
                // preemption means a higher priority process is waiting
                higher_priority_arrived = true;
            }
            else
            {
                // loop through all processes to check if a higher priority process has arrived
                for (int j = 0; j < ctx->num_processes; j++)
                {
                    if (ctx->processes[j].arrival_time <= current_time &&
                        ctx->processes[j].remaining_time > 0 &&
                        ctx->processes[j].priority < highest_priority)
                    {
                        higher_priority_arrived = true;
                        break;
                    }
                }
            }
            
            // if a higher priority process has arrived, break out of the round robin 
            if (higher_priority_arrived)
                break;  // Exit RR loop, will re-evaluate priorities in next iteration of main loop
        }
    }

    // Step 7: Display results
    display_results(ctx, "PRIORITY_PREEMPTIVE_WITH_RR");
}

/* ========================================================================================*/
/*
 * TESTING YOUR IMPLEMENTATION:
 * ----------------------------
 * 1. Compile: make
 * 2. Run: ./scheduler < Testing/Testcases/input1.txt
 * 3. Compare output with Testing/Expected_output/output1.txt 
 * 
 * Key Points:
 * - At time 1, P2 (priority=1) preempts P1 (priority=2)
 * - At time 2, P3 (priority=1) joins, P2 and P3 use RR
 * - P2 executes for quantum=3, then P3 gets its turn
 * - After P3 completes, P2 finishes its remaining 1 unit
 * - P1 resumes only after all priority=1 processes complete
 * 
 * PREEMPTION EXAMPLE:
 * -------------------
 * Time 0: P1 (priority=3) starts
 * Time 2: P2 (priority=1) arrives → IMMEDIATELY preempts P1
 * Time 5: P3 (priority=2) arrives → Does NOT preempt P2 (1 < 2)
 * Time 7: P2 completes → P3 executes (priority=2 > P1's priority=3)
 * Time 10: P3 completes → P1 resumes
 * 
 * ROUND ROBIN WITHIN SAME PRIORITY:
 * ----------------------------------
 * If at time 5, we have:
 *   P1: Priority=2, Remaining=4
 *   P2: Priority=2, Remaining=6
 *   P3: Priority=2, Remaining=3
 * 
 * With time_quantum=3:
 * - P1 executes for 3 (remaining=1)
 * - P2 executes for 3 (remaining=3)
 * - P3 executes for 3 (completes)
 * - P1 executes for 1 (completes)
 * - P2 executes for 3 (completes)
 * 
 * DEBUGGING TIPS:
 * ---------------
 * - Print which process executes at each time unit
 * - Print current highest priority at each scheduling decision
 * - Track which processes are in the ready list for RR
 * - Verify preemptions occur when higher priority process arrives
 * - Check that RR rotation happens only among same-priority processes
 * - Trace timeline to ensure it matches expected execution order
 * 
 * COMMON MISTAKES:
 * ----------------
 * - Not checking for preemption during execution (only before)
 * - Applying RR to all processes instead of only same-priority ones
 * - Not re-evaluating priorities after each process execution
 * - Forgetting priority convention (lower number = higher priority)
 * - Executing entire quantum even when higher priority arrives
 * - Not breaking out of RR loop when priorities change
 */