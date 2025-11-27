/*****************************************************************************************************************************************
 * File: safety_skeleton.c
 *
 * Purpose:
 * Student implementation of is_safe_state() function for Banker's Algorithm.
 * This function implements the safety algorithm to check if the system is in a safe state.
 *
 *****************************************************************************************************************************************/

#include "banker_header.h"

/* =======================================================================================================================================*/
/**
 * Check if the system is in a safe state using Banker's Algorithm
 * 
 * Safety Algorithm:
 * 1. Initialize Work = Available and Finish[i] = false for all threads
 * 2. Find a thread i such that Finish[i] == false and Need[i] <= Work
 * 3. If found: Work = Work + Allocation[i], Finish[i] = true, add i to safe_sequence
 * 4. Repeat step 2-3 until all threads finish or no progress can be made
 * 5. Return true if all threads finished (safe), false otherwise (unsafe)
 * 
 * @param allocation: Current allocation matrix [threads][resources]
 * @param need: Need matrix [threads][resources]
 * @param available: Available resources array
 * @param safe_sequence: Array to store the safe sequence (caller allocated)
 * @param n: Number of threads
 * @param m: Number of resources
 * @return: true if safe state exists, false otherwise
 */
bool is_safe_state(int allocation[][MAX_RESOURCES], 
                   int need[][MAX_RESOURCES], 
                   int available[], 
                   int safe_sequence[], 
                   int n, 
                   int m) {
    
    // TODO: Implement the safety algorithm
    // step 1 init the work array with available resources
    //  Available and Finish[i] = false for all threads
    int work[MAX_RESOURCES];
    for (int i = 0; i < m; i++) {
        work[i] = available[i];
    }
    
    // setp 2 init finish array since all threads start as unfinished
    bool finish[MAX_THREADS];
    for (int i = 0; i < n; i++) {
        finish[i] = false;
    }
    
    // next we find a safe sequence by repeatedly finding threads that can finish safely
    //step 2 Find a thread i such that Finish[i] == false and Need[i] <= Work
    int sequence_index = 0;
    bool progress = true;
    
    while (progress) {
        progress = false;
        
        // try to find a thread that can finish by iterating through all threads
        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                // check if need[i] <= work for all resources
                bool can_finish = true;
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        can_finish = false;
                        break;
                    }
                }
                
                // if thread can finish, it means we can add it to the safe sequence
                // step 3 Work = Work + Allocation[i], Finish[i] = true, add i to safe_sequence
                if (can_finish) {
                    // add thread to safe sequence
                    // finish[i] = true
                    safe_sequence[sequence_index++] = i;
                    finish[i] = true;
                    
                    // release the thread's allocated resources back to work
                    // Work = Work + Allocation[i]
                    for (int j = 0; j < m; j++) {
                        work[j] += allocation[i][j];
                    }
                    
                    // set progress to true to indicate that we made progress
                    progress = true;
                }
            }
        }
    }
    
    // check if all threads finished which means the system is safe
    for (int i = 0; i < n; i++) {
        if (!finish[i]) {
            return false;
        }
    }
    
    // if all threads finished, return true
    return true;
}