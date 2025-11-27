/******************************************************************************
 * File: memory_allocator.h
 * 
 * COP4610 Assignment 5 - Contiguous Memory Allocation
 * 
 * Author: Dr. Bhargav Bhatkalkar, KFSCIS, Florida International University
 *****************************************************************************/

#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_BLOCKS 100
#define MAX_PROCESSES 50
#define MAX_LINE_LENGTH 256
#define DEFAULT_INPUT_FILE "INPUT.txt"

typedef enum {
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT
} AllocationStrategy;

typedef enum {
    PROCESS_NEW,
    PROCESS_ALLOCATED,
    PROCESS_TERMINATED
} ProcessState;

typedef enum {
    CMD_ALLOCATE,
    CMD_TERMINATE,
    CMD_STATUS,
    CMD_UNKNOWN
} CommandType;

typedef struct {
    int start_address;
    int size;
    bool is_free;
    int process_id;
} MemoryBlock;

typedef struct {
    int id;
    int size;
    ProcessState state;
    int block_index;
} Process;

typedef struct {
    int total_size;
    int free_size;
    int allocated_size;
    int block_count;
    MemoryBlock blocks[MAX_BLOCKS];
    AllocationStrategy strategy;
} MemoryManager;

typedef struct {
    CommandType type;
    int process_id;
    int size;
} Command;

// Student implementations
int find_first_fit(MemoryManager *manager, int size);
int find_best_fit(MemoryManager *manager, int size);
int find_worst_fit(MemoryManager *manager, int size);
int coalesce_memory(MemoryManager *manager, Process processes[], int num_processes);

// Framework functions
void initialize_memory_manager(MemoryManager *manager, AllocationStrategy strategy, int total_size);
bool allocate_memory(MemoryManager *manager, Process *process, Process processes[], int num_processes);
bool deallocate_memory(MemoryManager *manager, Process *process, Process processes[], int num_processes);
void split_block(MemoryManager *manager, int block_index, int allocated_size, Process processes[], int num_processes);
void print_memory_status(MemoryManager *manager);
bool parse_command(const char *line, Command *cmd);
void process_command(MemoryManager *manager, Command *cmd, Process processes[], int *num_processes);
bool read_and_process_input(const char *filename, MemoryManager *manager, Process processes[], int *num_processes);

#endif