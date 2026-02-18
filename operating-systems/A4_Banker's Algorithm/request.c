/*****************************************************************************************************************************************
 * File: request_skeleton.c
 *
 * Purpose:
 * Student implementation of thread_resource_request() function for Banker's Algorithm.
 * This function processes resource allocation and deallocation requests.
 *
 *****************************************************************************************************************************************/

#include "banker_header.h"

/* =======================================================================================================================================*/
/**
 * Process a resource request from a thread
 * 
 * Algorithm:
 * 1. Validate thread_id bounds
 * 2. Handle resource requests (positive = allocate, negative = release):
 *    a. For allocation: Check if request <= need (don't exceed remaining need)
 *       For release: Check if thread has enough allocated resources
 *    b. Check if request <= available (resources must be available)
 * 3. Temporarily apply the changes:
 *    a. Update available resources: available[j] -= request[j]
 *    b. Update allocation: allocation[thread_id][j] += request[j]
 *    c. Update need: need[thread_id][j] -= request[j]
 * 4. Check if resulting state is safe using is_safe_state()
 * 5. If safe, keep allocation; if unsafe, rollback and deny
 * 
 * @param thread_id: ID of the requesting thread (0 to n-1)
 * @param request: Array of requested resources (positive = allocate, negative = release)
 * @param allocation: Current allocation matrix (may be modified)
 * @param need: Need matrix (may be modified)
 * @param available: Available resources array (may be modified)
 * @param n: Number of threads
 * @param m: Number of resources
 * @return: true if request is granted, false if denied
 */
bool thread_resource_request(int thread_id, int request[], 
                            int allocation[][MAX_RESOURCES], 
                            int need[][MAX_RESOURCES], 
                            int available[], int n, int m) {
    
    // TODO: Implement resource request processing
    // first step is to validate the thread_id bounds
    if (thread_id < 0 || thread_id >= n) {
        return false;
    }
    
    // check for zero request [0,0,0] should be denied
    bool zero_request = true;
    for (int j = 0; j < m; j++) {
        if (request[j] != 0) {
            zero_request = false;
            break;
        }
    }
    if (zero_request) {
        return false;  // deny zero requests
    }
    
    // step 2 next cbeck if request is valid for allocation: request <= need and for release: check resource allocation is greater than the request
    for (int j= 0; j < m; j++) {
        // for allocation (positive request) it must not exceed need
        if (request[j] > 0 && request[j] > need[thread_id][j]) {
            return false;
        }
        // for release (negative request): must not exceed current allocation
        if (request[j]< 0 && -request[j] > allocation[thread_id][j]) {
            return false;
        }
        // check if resources are available (for allocation requests)
        if (request[j] > 0 && request[j] > available[j]) {
            return false;
        }
    }
    
    // step 3 temporarily apply the changes
    for (int j = 0; j < m; j++) {
        // Update available resources: available[j] -= request[j]
        available[j] -= request[j];
        //Update allocation: allocation[thread_id][j] += request[j]
        allocation[thread_id][j] += request[j];
        // Update need: need[thread_id][j] -= request[j]
        need[thread_id][j] -= request[j];
    }
    
    // step 4 Check if resulting state is safe using is_safe_state()
    int safe_sequence[MAX_THREADS];
    bool is_safe = is_safe_state(allocation, need, available, safe_sequence, n, m);
    
    //step 5 If safe, keep allocation; if unsafe, rollback and deny
    if (!is_safe) {
        // undo the changes from step 3
        for (int j = 0; j < m; j++) {
            available[j] += request[j];
            allocation[thread_id][j] -= request[j];
            need[thread_id][j] += request[j];
        }
        return false;
    }
    
    // returns true if request is granted and changes are applied
    return true;
}