/*****************************************************************************************************************************************
 * File: available_skeleton.c
 *
 * Purpose:
 * Student implementation of calculate_available() function for Banker's Algorithm.
 * This function calculates the available resources from total and allocated resources.
 *****************************************************************************************************************************************/

#include "banker_header.h"

/* =======================================================================================================================================*/
/**
 * Calculate the Available resources for Banker's Algorithm
 * Formula: Available[j] = Total[j] - Σ(Allocation[i][j]) for all threads i
 * @param allocation: Current allocation matrix [threads][resources]
 * @param total_resources: Total instances of each resource type
 * @param available: Available resources array to be calculated
 * @param n: Number of threads
 * @param m: Number of resources
 */
void calculate_available(int allocation[][MAX_RESOURCES], 
                         int total_resources[], 
                         int available[], 
                         int n, 
                         int m) {
    
    // TODO: Implement the available resources calculation
    // for each resource j, calculate available[j] = total_resources[j] - sum of all allocations[i][j]
    for (int j = 0; j < m; j++) {
        available[j] = total_resources[j];
        // subtract all allocations for the current resource
        for (int i = 0; i < n; i++) {
            available[j] -= allocation[i][j];
        }
    }
}