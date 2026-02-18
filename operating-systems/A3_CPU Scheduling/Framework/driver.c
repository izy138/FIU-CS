/**
 * ===============================================================================
 * CPU SCHEDULER ASSIGNMENT- DRIVER PROGRAM
 * ===============================================================================
 * @file driver.c
 * @brief Driver program for CPU scheduling assignment
 * @author Dr. Bhargav Bhatkalkar, KFSCIS, Florida International University
 * 
 * This driver reads process data from stdin and runs the RR scheduler with a TimeQuantum = 2.
 * The autograder redirects input from test case files.
 * ===============================================================================
 */

#include "CPU_scheduler.h"

/* ========================================================================================*/
/* CORE UTILITY FUNCTIONS */
/* ========================================================================================*/

void init_scheduler_context(SchedulerContext *ctx)
{
    ctx->num_processes = 0;
    memset(ctx->processes, 0, sizeof(ctx->processes));
}

/* ========================================================================================*/

void reset_process_states(SchedulerContext *ctx)
{
    for (int i = 0; i < ctx->num_processes; i++)
    {
        ctx->processes[i].remaining_time = ctx->processes[i].burst_time;
        ctx->processes[i].waiting_time = 0;
        ctx->processes[i].turnaround_time = 0;
        ctx->processes[i].completion_time = 0;
        ctx->processes[i].is_completed = false;
    }
}

/* ========================================================================================*/

bool validate_input_data(const SchedulerContext *ctx)
{
    if (ctx->num_processes <= 0 || ctx->num_processes > MAX_PROCESSES)
    {
        return false;
    }

    for (int i = 0; i < ctx->num_processes; i++)
    {
        if (ctx->processes[i].burst_time <= 0)
        {
            return false;
        }
        if (ctx->processes[i].arrival_time < 0)
        {
            return false;
        }
        if (ctx->processes[i].priority < 0)
        {
            return false;
        }
    }

    // Check for duplicate PIDs
    for (int i = 0; i < ctx->num_processes - 1; i++)
    {
        for (int j = i + 1; j < ctx->num_processes; j++)
        {
            if (ctx->processes[i].pid == ctx->processes[j].pid)
            {
                return false;
            }
        }
    }

    return true;
}

/* ========================================================================================*/

void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/* ========================================================================================*/

int min_value(int a, int b)
{
    return (a < b) ? a : b;
}

/* ========================================================================================*/

bool read_processes_from_stdin(SchedulerContext *ctx)
{
    char line[256];
    int line_number = 0;

    // Skip the first two lines (header and separator)
    while (line_number < 2 && fgets(line, sizeof(line), stdin))
    {
        line_number++;
    }

    // Read process data from stdin
    while (fgets(line, sizeof(line), stdin) && ctx->num_processes < MAX_PROCESSES)
    {
        // Skip empty lines, whitespace-only lines, and separator lines
        bool skip_line = true;
        for (int i = 0; line[i] != '\0'; i++)
        {
            if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\r' &&
                line[i] != '=' && line[i] != '-')
            {
                skip_line = false;
                break;
            }
        }

        if (skip_line)
        {
            continue;
        }

        char pid_str[10];
        int pid, priority, burst_time, arrival_time;

        // Read the process ID and other values
        if (sscanf(line, "%s %d %d %d", pid_str, &burst_time, &priority, &arrival_time) == 4)
        {
            // Extract numeric part from 'P1' or read directly if it's just '1'
            if (sscanf(pid_str, "P%d", &pid) != 1)
            {
                if (sscanf(pid_str, "%d", &pid) != 1)
                {
                    continue;
                }
            }

            // Store the process data
            ctx->processes[ctx->num_processes].pid = pid;
            ctx->processes[ctx->num_processes].priority = priority;
            ctx->processes[ctx->num_processes].burst_time = burst_time;
            ctx->processes[ctx->num_processes].arrival_time = arrival_time;
            ctx->processes[ctx->num_processes].remaining_time = burst_time;
            ctx->processes[ctx->num_processes].waiting_time = 0;
            ctx->num_processes++;
        }
    }

    if (ctx->num_processes == 0)
    {
        return false;
    }

    return validate_input_data(ctx);
}

/* ========================================================================================*/
/**
 * Computing Turnaround Time and Waiting Time for all processes
 * 
 * FORMULAS (as per lecture slides):
 * - Turnaround Time (TAT) = Completion Time - Arrival Time
 * - Waiting Time (WT) = Turnaround Time - CPU Burst Time
 */
void calculate_turnaround_times(SchedulerContext *ctx)
{
    for (int i = 0; i < ctx->num_processes; i++)
    {
        // Calculate Turnaround Time: TAT = Completion Time - Arrival Time
        ctx->processes[i].turnaround_time = 
            ctx->processes[i].completion_time - ctx->processes[i].arrival_time;
        
        // Calculate Waiting Time: WT = TAT - Burst Time
        ctx->processes[i].waiting_time = 
            ctx->processes[i].turnaround_time - ctx->processes[i].burst_time;
    }
}

/* ========================================================================================*/

void calculate_average_times(const SchedulerContext *ctx)
{
    int total_waiting_time = 0, total_turnaround_time = 0;

    for (int i = 0; i < ctx->num_processes; i++)
    {
        total_waiting_time += ctx->processes[i].waiting_time;
        total_turnaround_time += ctx->processes[i].turnaround_time;
    }
   
    printf("Average Turnaround Time: %.2f\nAverage Waiting Time: %.2f\n", (float)total_turnaround_time / ctx->num_processes,
           (float)total_waiting_time / ctx->num_processes);
}

/* ========================================================================================*/

void display_results(const SchedulerContext *ctx, const char *algorithm_name)
{
    calculate_turnaround_times((SchedulerContext *)ctx);

    printf("%s\n", algorithm_name);
    printf("PID      Turnaround_Time      Waiting_Time\n");
    for (int i = 0; i < ctx->num_processes; i++)
    {
        printf("%-9d%-21d%d\n",
               ctx->processes[i].pid,
               ctx->processes[i].turnaround_time,
               ctx->processes[i].waiting_time);
    }

    calculate_average_times(ctx);
}

/* ========================================================================================*/
/* MAIN DRIVER PROGRAM */
/* ========================================================================================*/

int main(void)
{
    // Initialize scheduler context
    SchedulerContext ctx;
    init_scheduler_context(&ctx);

    // Read process data from stdin (autograder redirects from test files)
    if (!read_processes_from_stdin(&ctx))
    {
        fprintf(stderr, "Error: Invalid input data format.\n");
        return EXIT_FAILURE;
    }
    
    //💡Run all scheduling algorithms in sequence

    printf("============================================\n");
    // 1. First-Come, First-Served (FCFS)
    first_come_first_served(&ctx);
    printf("============================================\n");
    // 2. Shortest Job First (SJF) - Non-preemptive
    shortest_job_first(&ctx);
    printf("============================================\n");
    // 3. Shortest Remaining Time (SRT) - Preemptive
    shortest_remaining_time_first(&ctx);
    printf("============================================\n");
    // 4. Round Robin (RR) with quantum = 3    
    round_robin(&ctx, 3);
    printf("============================================\n");
    // 5. Priority - non-preemptive
    priority_non_preemptive(&ctx); 
    printf("============================================\n");
    // 6. Priority - preemptive with RR (quantum = 3)
    priority_preemptive_rr(&ctx, 3); 
    printf("============================================\n");
    
    return EXIT_SUCCESS;
}