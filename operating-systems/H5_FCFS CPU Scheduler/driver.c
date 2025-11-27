/**
 * ===============================================================================
 * FCFS SCHEDULER - DRIVER PROGRAM
 * ===============================================================================
 * @file driver.c
 * @brief Driver program for FCFS scheduling homework
 * @author Dr. Bhargav Bhatkalkar, KFSCIS, Florida International University
 * 
 * This driver reads process data from stdin and runs the FCFS scheduler.
 * The autograder redirects input from test case files.
 * ===============================================================================
 */

#include "fcfs_scheduler.h"

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

    printf("AWT:%.2f\nATT:%.2f\n", (float)total_waiting_time / ctx->num_processes,
           (float)total_turnaround_time / ctx->num_processes);
}

/* ========================================================================================*/

void display_results(const SchedulerContext *ctx, const char *algorithm_name)
{
    calculate_turnaround_times((SchedulerContext *)ctx);

    printf("%s\n", algorithm_name);
    for (int i = 0; i < ctx->num_processes; i++)
    {
        printf("%-4d %-12d %-15d\n",
               ctx->processes[i].pid,
               ctx->processes[i].waiting_time,
               ctx->processes[i].turnaround_time);
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

    // Run First-Come, First-Served (FCFS) scheduling
    fcfs_scheduling(&ctx);

    return EXIT_SUCCESS;
}