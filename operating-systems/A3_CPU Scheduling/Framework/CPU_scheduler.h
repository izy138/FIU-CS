/*
 * ===============================================================================
 * CPU SCHEDULING ALGORITHMS HEADER FILE
 * ❎ Do not modify this file contents!
 * ===============================================================================
 * 
 * TIE-BREAKING RULES USED:
 * 
 * 1. FCFS: Arrival time → Process ID (if same arrival time)
 * 2. SJF:  Burst time → Arrival time → Process ID  
 * 3. SRT:  Remaining time → Arrival time → Process ID
 * 4. Priority: Priority value → Arrival time → Process ID
 * 5. Round Robin: Process order in array (no tie-breaking needed)
 * 
 * ===============================================================================
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

// Max allowed processess
#define MAX_PROCESSES 100

/* ========================================================================================*/
// Structure to store process information
typedef struct
{
    int pid;
    int priority;
    int burst_time;
    int arrival_time;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
    int completion_time;
    bool is_completed;
} Process;

/* ========================================================================================*/
// Structure to hold scheduler context 
typedef struct
{
    Process processes[MAX_PROCESSES];
    int num_processes;
} SchedulerContext;

/* ========================================================================================*/
// Core utility function prototypes
bool read_processes_from_file(const char *filename, SchedulerContext *ctx);
void reset_process_states(SchedulerContext *ctx);
bool validate_input_data(const SchedulerContext *ctx);
void calculate_turnaround_times(SchedulerContext *ctx);
void calculate_average_times(const SchedulerContext *ctx);
void display_results(const SchedulerContext *ctx, const char *algorithm_name);
void clear_input_buffer(void);
int min_value(int a, int b);

/* ========================================================================================*/
// Scheduling algorithm function prototypes
void first_come_first_served(SchedulerContext *ctx);
void shortest_job_first(SchedulerContext *ctx);
void shortest_remaining_time_first(SchedulerContext *ctx);
void round_robin(SchedulerContext *ctx, int time_quantum);
void priority_non_preemptive(SchedulerContext *ctx);
void priority_preemptive_rr(SchedulerContext *ctx, int time_quantum);

/* ========================================================================================*/
// Helper function prototypes for modular design
void init_scheduler_context(SchedulerContext *ctx);

#endif // SCHEDULER_H