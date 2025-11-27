/*###################################################################################                    
⚠️ DONT modify this header file contents!                   
###################################################################################*/
#ifndef BANKER_HEADER_H
#define BANKER_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/* ==========================================================================*/
// Constants
#define MAX_THREADS 10
#define MAX_RESOURCES 10
#define MAX_LINE_LENGTH 256
/* ==========================================================================*/

/*###################################################################################
👍Students implement the following four functions, each in a separate .c file.
                1)need.c 
                2)available.c 
                3)safety.c 
                4)request.c
###################################################################################*/

/**
 * Calculate the need matrix (Need = Max - Allocation)
 * IMPLEMENT IN: need.c
 * @param allocation: Current allocation matrix
 * @param max: Maximum demand matrix  
 * @param need: Need matrix to be calculated
 * @param n: Number of threads
 * @param m: Number of resources
 */
void calculate_need(int allocation[][MAX_RESOURCES],
                    int max[][MAX_RESOURCES],
                    int need[][MAX_RESOURCES], 
                    int n, 
                    int m);

/**
 * Calculate available resources (Available = Total - Sum of Allocation)
 * IMPLEMENT IN: available.c
 * @param allocation: Current allocation matrix
 * @param total_resources: Total instances of each resource
 * @param available: Available resources array to be calculated
 * @param n: Number of threads
 * @param m: Number of resources
 */
void calculate_available(int allocation[][MAX_RESOURCES], 
                         int total_resources[], 
                         int available[], 
                         int n, 
                         int m);

/**
 * Check if the system is in a safe state using Banker's Algorithm
 * IMPLEMENT IN: safety.c
 * @param allocation: Current allocation matrix
 * @param need: Need matrix
 * @param available: Available resources array
 * @param safe_sequence: Array to store the safe sequence (if found)
 * @param n: Number of threads
 * @param m: Number of resources
 * @return: true if safe state exists, false otherwise
 */
bool is_safe_state(int allocation[][MAX_RESOURCES], 
                   int need[][MAX_RESOURCES], 
                   int available[], 
                   int safe_sequence[], 
                   int n, 
                   int m);

/**
 * Process a resource request from a thread
 * IMPLEMENT IN: request.c
 * @param thread_id: ID of the requesting thread
 * @param request: Array of requested resources
 * @param allocation: Current allocation matrix (may be modified)
 * @param max: Maximum demand matrix
 * @param need: Need matrix (may be modified)
 * @param available: Available resources array (may be modified)
 * @param n: Number of threads
 * @param m: Number of resources
 * @return: true if request is granted, false if denied
 * @note: This function should call is_safe_state() to verify safety
 */
bool thread_resource_request(int thread_id, 
                             int request[], 
                             int allocation[][MAX_RESOURCES],                              
                             int need[][MAX_RESOURCES], 
                             int available[], 
                             int n, 
                             int m);


/*###################################################################################
⚠️ Data structure print functions provided in driver (students DONT implement these!) 
###################################################################################*/                            
void print_total(int total_resources[], int m);
void print_available(int available[], int m);
void print_allocation(int allocation[][MAX_RESOURCES] ,int n, int m);
void print_max(int max[][MAX_RESOURCES], int n, int m);
void print_need(int need[][MAX_RESOURCES], int n, int m);

/*###################################################################################
⚠️ Utility functions provided in driver (students DONT implement these!) 
###################################################################################*/
int extract_numbers(const char *line, int *numbers, int max_numbers);
int extract_matrix_values(const char *line, int *numbers, int max_numbers);
int extract_thread_id(const char *line);
int read_input_file(const char *filename, int allocation[][MAX_RESOURCES], 
                   int max[][MAX_RESOURCES], int total_resources[], int *n, int *m);
#endif // BANKER_HEADER_H