/*****************************************************************************************************************************************
 * File: need_skeleton.c
 *
 * Purpose:
 * Student implementation of calculate_need() function for Banker's Algorithm.
 * This function calculates the Need matrix from Max and Allocation matrices.
 *
 ******************************************************************************************************************************************/

#include "banker_header.h"

/* =======================================================================================================================================*/
/**
 * Calculate the Need matrix for Banker's Algorithm
 * Formula: Need[i][j] = Max[i][j] - Allocation[i][j]
 * @param allocation: Current allocation matrix [threads][resources]
 * @param max: Maximum demand matrix [threads][resources]  
 * @param need: Need matrix to be calculated [threads][resources]
 * @param n: Number of threads
 * @param m: Number of resources
 */
void calculate_need(int allocation[][MAX_RESOURCES], 
                    int max[][MAX_RESOURCES], 
                    int need[][MAX_RESOURCES], 
                    int n, 
                    int m) {
    
    // TODO: Implement the need matrix calculation
    // for each thread i and resource j, calculate need[i][j] = max[i][j] - allocation[i][j]
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}