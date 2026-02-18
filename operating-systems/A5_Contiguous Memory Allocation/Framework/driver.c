/******************************************************************************
 * File: driver.c
 * 
 * COP4610 BONUS Assignment - Contiguous Memory Allocation
 * 
 * Author: Dr. Bhargav Bhatkalkar, KFSCIS, Florida International University
 *****************************************************************************/

#include "memory_allocator.h"

const char* get_strategy_name(AllocationStrategy strategy) {
    switch (strategy) {
        case FIRST_FIT: return "FIRST_FIT";
        case BEST_FIT: return "BEST_FIT";
        case WORST_FIT: return "WORST_FIT";
        default: return "UNKNOWN";
    }
}

void initialize_memory_manager(MemoryManager *manager, AllocationStrategy strategy, int total_size) {
    manager->total_size = total_size;
    manager->free_size = total_size;
    manager->allocated_size = 0;
    manager->block_count = 1;
    manager->strategy = strategy;
    
    manager->blocks[0].start_address = 0;
    manager->blocks[0].size = total_size;
    manager->blocks[0].is_free = true;
    manager->blocks[0].process_id = -1;
}

int find_block(MemoryManager *manager, int size) {
    switch (manager->strategy) {
        case FIRST_FIT: return find_first_fit(manager, size);
        case BEST_FIT: return find_best_fit(manager, size);
        case WORST_FIT: return find_worst_fit(manager, size);
        default: return -1;
    }
}

void split_block(MemoryManager *manager, int block_index, int allocated_size, Process processes[], int num_processes) {
    MemoryBlock *block = &manager->blocks[block_index];
    
    if (block->size > allocated_size) {
        if (manager->block_count >= MAX_BLOCKS) return;
        
        for (int i = manager->block_count; i > block_index + 1; i--) {
            manager->blocks[i] = manager->blocks[i - 1];
        }
        
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].state == PROCESS_ALLOCATED && processes[i].block_index > block_index) {
                processes[i].block_index++;
            }
        }
        
        manager->blocks[block_index + 1].start_address = block->start_address + allocated_size;
        manager->blocks[block_index + 1].size = block->size - allocated_size;
        manager->blocks[block_index + 1].is_free = true;
        manager->blocks[block_index + 1].process_id = -1;
        
        block->size = allocated_size;
        manager->block_count++;
    }
}

bool allocate_memory(MemoryManager *manager, Process *process, Process processes[], int num_processes) {
    int block_index = find_block(manager, process->size);
    
    if (block_index == -1) {
        printf("ALLOCATE: P%d %d\n", process->id, process->size);
        printf("RESULT: FAILURE\n");
        printf("REASON: No suitable block found\n");
        return false;
    }
    
    MemoryBlock *block = &manager->blocks[block_index];
    
    printf("ALLOCATE: P%d %d\n", process->id, process->size);
    printf("RESULT: SUCCESS\n");
    
    split_block(manager, block_index, process->size, processes, num_processes);
    
    block->is_free = false;
    block->process_id = process->id;
    
    process->state = PROCESS_ALLOCATED;
    process->block_index = block_index;
    
    manager->free_size -= process->size;
    manager->allocated_size += process->size;
    
    printf("BLOCK: P%d [%d-%d] SIZE:%d\n", process->id, block->start_address, 
           block->start_address + block->size - 1, block->size);
    
    return true;
}

bool deallocate_memory(MemoryManager *manager, Process *process, Process processes[], int num_processes) {
    if (process->state != PROCESS_ALLOCATED) {
        printf("TERMINATE: P%d\n", process->id);
        printf("RESULT: FAILURE\n");
        printf("REASON: Process not allocated\n");
        return false;
    }
    
    int block_index = process->block_index;
    MemoryBlock *block = &manager->blocks[block_index];
    int freed_start = block->start_address;
    
    printf("TERMINATE: P%d\n", process->id);
    printf("RESULT: SUCCESS\n");
    
    block->is_free = true;
    block->process_id = -1;
    
    process->state = PROCESS_TERMINATED;
    process->block_index = -1;
    
    manager->free_size += block->size;
    manager->allocated_size -= block->size;
    
    int merged_count = coalesce_memory(manager, processes, num_processes);
    
    printf("COALESCING: %d blocks merged\n", merged_count);
    
    if (merged_count > 0) {
        for (int i = 0; i < manager->block_count; i++) {
            if (manager->blocks[i].is_free && 
                manager->blocks[i].start_address <= freed_start &&
                (manager->blocks[i].start_address + manager->blocks[i].size) > freed_start) {
                printf("MERGED_BLOCK: [%d-%d] SIZE:%d\n", manager->blocks[i].start_address,
                       manager->blocks[i].start_address + manager->blocks[i].size - 1,
                       manager->blocks[i].size);
                break;
            }
        }
    }
    
    printf("\n");
    return true;
}

void print_memory_status(MemoryManager *manager) {
    printf("STATUS:\n");
    printf("MEMORY_BLOCKS: %d\n", manager->block_count);
    
    for (int i = 0; i < manager->block_count; i++) {
        MemoryBlock *block = &manager->blocks[i];
        if (block->is_free) {
            printf("BLOCK: FREE [%d-%d] SIZE:%d STATE:FREE\n", block->start_address,
                   block->start_address + block->size - 1, block->size);
        } else {
            printf("BLOCK: P%d [%d-%d] SIZE:%d STATE:ALLOCATED\n", block->process_id,
                   block->start_address, block->start_address + block->size - 1, block->size);
        }
    }
    
    printf("TOTAL_FREE: %d\n", manager->free_size);
    printf("TOTAL_ALLOCATED: %d\n", manager->allocated_size);
    
    int largest_free_block = 0;
    for (int i = 0; i < manager->block_count; i++) {
        if (manager->blocks[i].is_free && manager->blocks[i].size > largest_free_block) {
            largest_free_block = manager->blocks[i].size;
        }
    }
    
    double fragmentation = 0.0;
    if (manager->free_size > 0) {
        fragmentation = ((double)(manager->free_size - largest_free_block) / manager->free_size) * 100.0;
    }
    
    printf("FRAGMENTATION: %.2f\n", fragmentation);    
    //printf("\n");
}

bool parse_command(const char *line, Command *cmd) {
    char cmd_str[32];
    
    if (line[0] == '\0' || line[0] == '#' || line[0] == '\n') {
        cmd->type = CMD_UNKNOWN;
        return false;
    }
    
    if (sscanf(line, "ALLOCATE %d %d", &cmd->process_id, &cmd->size) == 2) {
        cmd->type = CMD_ALLOCATE;
        return true;
    }
    
    if (sscanf(line, "TERMINATE %d", &cmd->process_id) == 1) {
        cmd->type = CMD_TERMINATE;
        return true;
    }
    
    if (sscanf(line, "%s", cmd_str) == 1 && strcmp(cmd_str, "STATUS") == 0) {
        cmd->type = CMD_STATUS;
        return true;
    }
    
    cmd->type = CMD_UNKNOWN;
    return false;
}

Process* find_process(Process processes[], int num_processes, int process_id) {
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].id == process_id) {
            return &processes[i];
        }
    }
    return NULL;
}

void process_command(MemoryManager *manager, Command *cmd, Process processes[], int *num_processes) {
    switch (cmd->type) {
        case CMD_ALLOCATE: {
            Process *existing = find_process(processes, *num_processes, cmd->process_id);
            
            if (existing != NULL) {
                if (existing->state == PROCESS_ALLOCATED) {
                    printf("ALLOCATE: P%d %d\n", cmd->process_id, cmd->size);
                    printf("RESULT: FAILURE\n");
                    printf("REASON: Process already allocated\n");
                } else {
                    existing->size = cmd->size;
                    existing->state = PROCESS_NEW;
                    existing->block_index = -1;
                    allocate_memory(manager, existing, processes, *num_processes);
                }
            } else {
                if (*num_processes >= MAX_PROCESSES) {
                    printf("ALLOCATE: P%d %d\n", cmd->process_id, cmd->size);
                    printf("RESULT: FAILURE\n");
                    printf("REASON: Maximum process limit reached\n");
                    return;
                }
                
                Process *new_process = &processes[*num_processes];
                new_process->id = cmd->process_id;
                new_process->size = cmd->size;
                new_process->state = PROCESS_NEW;
                new_process->block_index = -1;
                (*num_processes)++;
                
                allocate_memory(manager, new_process, processes, *num_processes);
            }
            break;
        }
        
        case CMD_TERMINATE: {
            Process *process = find_process(processes, *num_processes, cmd->process_id);
            
            if (process == NULL) {
                printf("TERMINATE: P%d\n", cmd->process_id);
                printf("RESULT: FAILURE\n");
                printf("REASON: Process not found\n");
            } else {
                deallocate_memory(manager, process, processes, *num_processes);
            }
            break;
        }
        
        case CMD_STATUS:
            print_memory_status(manager);
            break;
            
        default:
            break;
    }
}

bool read_and_process_input(const char *filename, MemoryManager *manager, Process processes[], int *num_processes) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Could not open input file '%s'\n", filename);
        return false;
    }
    
    char line[MAX_LINE_LENGTH];
    bool first_line = true;
    int memory_size = 0;
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        
        if (first_line) {
            if (sscanf(line, "%d", &memory_size) != 1 || memory_size <= 0) {
                fprintf(stderr, "ERROR: Invalid memory size\n");
                fclose(file);
                return false;
            }
            
            printf("MEMORY_SIZE: %d\n", memory_size);
            printf("STRATEGY: %s\n", get_strategy_name(manager->strategy));
            
            initialize_memory_manager(manager, manager->strategy, memory_size);
            first_line = false;
            continue;
        }
        
        Command cmd;
        if (parse_command(line, &cmd)) {
            process_command(manager, &cmd, processes, num_processes);
        }
    }
    
    fclose(file);
    return true;
}

int main(int argc, char *argv[]) {
    const char *filename = DEFAULT_INPUT_FILE;
    if (argc > 1)
        filename = argv[1];

    for (int s = 0; s < 3; s++) {
        AllocationStrategy strategy = (AllocationStrategy)s;
        const char *strategy_name = (strategy == FIRST_FIT ? "FIRST_FIT" :
                                     strategy == BEST_FIT ? "BEST_FIT" : "WORST_FIT");

        printf("\n=============================================================\n");
        printf("                STRATEGY: %s\n", strategy_name);
        printf("=============================================================\n\n");

        MemoryManager manager;
        manager.strategy = strategy;

        Process processes[MAX_PROCESSES];
        int num_processes = 0;

        if (!read_and_process_input(filename, &manager, processes, &num_processes)) {
            fprintf(stderr, "ERROR: Failed to process input for %s\n", strategy_name);
            return EXIT_FAILURE;
        }

        printf("\n");
    }

    return EXIT_SUCCESS;
}
