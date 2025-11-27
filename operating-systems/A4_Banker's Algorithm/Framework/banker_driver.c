/*****************************************************************************************************************************************
 * File: banker_driver.c
 *
 * Purpose:
 * Driver program for Banker's Algorithm autograding.
 * Handles file I/O, test case execution, and output formatting.
 * Students only need to implement the core algorithm functions.
 *
 *****************************************************************************************************************************************/

#include "banker_header.h"

/* =======================================================================================================================================*/
// Test case structure
typedef struct {
    char type[20];      // "SAFETY_CHECK" or "REQUEST"
    int thread_id;      // For REQUEST type
    int request[MAX_RESOURCES];  // For REQUEST type
} TestCase;

/* =======================================================================================================================================*/
// Global system state (managed by driver)
static int allocation[MAX_THREADS][MAX_RESOURCES];
static int max[MAX_THREADS][MAX_RESOURCES];
static int need[MAX_THREADS][MAX_RESOURCES];
static int available[MAX_RESOURCES];
static int total_resources[MAX_RESOURCES];
static int n, m; // number of threads and resources

/* =======================================================================================================================================*/
// Utility Functions (provided to students)

int extract_numbers(const char *line, int *numbers, int max_numbers) {
    int count = 0;
    const char *ptr = line;
    char *endptr;

    while (*ptr && count < max_numbers) {
        // Skip non-digit characters (but allow negative sign)
        while (*ptr && !isdigit(*ptr) && *ptr != '-')
            ptr++;

        if (!*ptr)
            break;

        // Convert to integer
        long val = strtol(ptr, &endptr, 10);

        if (ptr != endptr) { // Conversion successful
            numbers[count++] = (int)val;
            ptr = endptr;
        } else { // No valid number found
            break;
        }
    }
    return count;
}

int extract_matrix_values(const char *line, int *numbers, int max_numbers) {
    int count = 0;
    const char *ptr = line;

    // First, find the thread ID part
    while (*ptr) {
        if (*ptr == 'T' && isdigit(*(ptr + 1))) {
            // Skip past the thread ID (T0, T1, etc.)
            while (*ptr && !isspace(*ptr))
                ptr++;

            // Now extract the numbers that follow
            while (*ptr && count < max_numbers) {
                // Skip whitespace
                while (*ptr && isspace(*ptr))
                    ptr++;

                if (!*ptr)
                    break;

                // Check for negative numbers or digits
                if (*ptr == '-' || isdigit(*ptr)) {
                    char *endptr;
                    long val = strtol(ptr, &endptr, 10);

                    if (ptr != endptr) { // Conversion successful
                        numbers[count++] = (int)val;
                        ptr = endptr;
                    } else {
                        break;
                    }
                } else {
                    break;
                }
            }
            break;
        }
        ptr++;
    }
    return count;
}

int extract_thread_id(const char *line) {
    const char *ptr = line;

    // Look for "T" followed by digits
    while (*ptr) {
        if (*ptr == 'T' && isdigit(*(ptr + 1))) {
            return atoi(ptr + 1);
        }
        ptr++;
    }
    return -1; // Thread ID not found
}

int read_input_file(const char *filename, int alloc[][MAX_RESOURCES], 
                   int max_matrix[][MAX_RESOURCES], int total_res[], int *threads, int *resources) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("ERROR: Cannot open SYSTEM_STATE.txt  file %s\n", filename);
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    int in_allocation_matrix = 0;
    int in_max_matrix = 0;
    int allocation_rows_found = 0;
    int max_rows_found = 0;

    // Initialize matrices
    memset(alloc, 0, sizeof(int) * MAX_THREADS * MAX_RESOURCES);
    memset(max_matrix, 0, sizeof(int) * MAX_THREADS * MAX_RESOURCES);
    memset(total_res, 0, sizeof(int) * MAX_RESOURCES);

    while (fgets(line, sizeof(line), file)) {
        // Remove trailing newline
        size_t len = strlen(line);
        if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
            line[--len] = '\0';
        if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
            line[--len] = '\0';

        // Skip empty lines
        if (len == 0) continue;

        // Parse number of threads
        if (strstr(line, "threads") != NULL) {
            int numbers[2];
            if (extract_numbers(line, numbers, 2) > 0) {
                *threads = numbers[0];
                if (*threads > MAX_THREADS) {
                    printf("ERROR: Number of threads exceeds maximum (%d)\n", MAX_THREADS);
                    fclose(file);
                    return 0;
                }
            }
            continue;
        }

        // Parse number of resources
        if (strstr(line, "resources") != NULL) {
            int numbers[2];
            if (extract_numbers(line, numbers, 2) > 0) {
                *resources = numbers[0];
                if (*resources > MAX_RESOURCES) {
                    printf("ERROR: Number of resources exceeds maximum (%d)\n", MAX_RESOURCES);
                    fclose(file);
                    return 0;
                }
            }
            continue;
        }

        // Parse resource instances
        if (strstr(line, "instances of resource") != NULL) {
            int numbers[2];
            int count = extract_numbers(line, numbers, 2);
            if (count >= 2) {
                int resource_id = numbers[0];
                total_res[resource_id] = numbers[1];
            }
            continue;
        }

        // Matrix section markers
        if (strstr(line, "Allocation matrix") != NULL) {
            in_allocation_matrix = 1;
            in_max_matrix = 0;
            continue;
        }

        if (strstr(line, "Max matrix") != NULL) {
            in_allocation_matrix = 0;
            in_max_matrix = 1;
            continue;
        }

        // Skip header and divider lines
        if (strstr(line, "R0") != NULL) continue;
        
        bool is_divider = true;
        for (int i = 0; line[i]; i++) {
            if (line[i] != '=' && line[i] != '-' && !isspace(line[i])) {
                is_divider = false;
                break;
            }
        }
        if (is_divider) continue;

        // Parse matrix rows
        int thread_id = extract_thread_id(line);
        if (thread_id >= 0 && thread_id < *threads) {
            int matrix_values[MAX_RESOURCES];
            int value_count = extract_matrix_values(line, matrix_values, MAX_RESOURCES);

            if (in_allocation_matrix && value_count >= *resources) {
                for (int j = 0; j < *resources; j++) {
                    alloc[thread_id][j] = matrix_values[j];
                }
                allocation_rows_found++;
            } else if (in_max_matrix && value_count >= *resources) {
                for (int j = 0; j < *resources; j++) {
                    max_matrix[thread_id][j] = matrix_values[j];
                }
                max_rows_found++;
            }
        }
    }

    fclose(file);

    // Validation
    if (*threads <= 0 || *resources <= 0) {
        printf("ERROR: Invalid number of threads or resources\n");
        return 0;
    }
    
    if (allocation_rows_found < *threads || max_rows_found < *threads) {
        printf("ERROR: Incomplete matrix data\n");
        return 0;
    }

    return 1;
}

void print_total(int total_resources[], int m)
{
    printf("Total: ");
    for (int j = 0; j < m; j++) {
        printf("R%d=%d ", j, total_resources[j]);
    }
    printf("\n");
}

void print_available(int available[], int m)
{
    printf("Available: ");
    for (int j = 0; j < m; j++) {
        printf("R%d=%d ", j, available[j]);
    }
    printf("\n");    
}

void print_allocation(int allocation[][MAX_RESOURCES] ,int n, int m)
{
     printf("Allocation: ");
    for (int i = 0; i < n; i++) {
        printf("T%d[", i);
        for (int j = 0; j < m; j++) {
            printf("%d%s", allocation[i][j], (j < m-1) ? "," : "");
        }
        printf("] ");
    }
    printf("\n");
}

void print_max(int max[][MAX_RESOURCES], int n, int m)
{
    printf("Max: ");
    for (int i = 0; i < n; i++) {
        printf("T%d[", i);
        for (int j = 0; j < m; j++) {
            printf("%d%s", max[i][j], (j < m-1) ? "," : "");
        }
        printf("] ");
    }
     printf("\n");
}

void print_need(int need[][MAX_RESOURCES], int n, int m)
{
    printf("Need: ");
    for (int i = 0; i < n; i++) {
        printf("T%d[", i);
        for (int j = 0; j < m; j++) {
            printf("%d%s", need[i][j], (j < m-1) ? "," : "");
        }
        printf("] ");
    }
    printf("\n");
}

/* =======================================================================================================================================*/
// Test execution functions

void execute_safety_check() {
    int safe_sequence[MAX_THREADS];
    
    if (is_safe_state(allocation, need, available, safe_sequence, n, m)) {
        printf("SAFETY_CHECK: SAFE <");
        for (int i = 0; i < n; i++) {
            printf("T%d%s", safe_sequence[i], (i < n-1) ? "," : "");
        }
        printf(">\n");
    } else {
        printf("SAFETY_CHECK: UNSAFE\n");
    }
}

void execute_request(int thread_id, int request[]) {
    // Validate thread ID
    if (thread_id < 0 || thread_id >= n) {
        printf("REQUEST_RESULT: ERROR_INVALID_THREAD\n");
        return;
    }

    // Print request info
    printf("REQUEST: T%d [", thread_id);
    for (int j = 0; j < m; j++) {
        printf("%d%s", request[j], (j < m-1) ? "," : "");
    }
    printf("]\n");

    // Execute request
    if (thread_resource_request(thread_id, request, allocation, need, available, n, m)) {
        // Request granted - show new safe sequence
        int safe_sequence[MAX_THREADS];
        if (is_safe_state(allocation, need, available, safe_sequence, n, m)) {
            printf("REQUEST_RESULT: GRANTED <");
            for (int i = 0; i < n; i++) {
                printf("T%d%s", safe_sequence[i], (i < n-1) ? "," : "");            }
            printf(">\n");

        } else {
            printf("REQUEST_RESULT: ERROR_INCONSISTENT_STATE\n");
        }
    } else {
        printf("REQUEST_RESULT: DENIED\n");
    }
    
   // print_available(available, m);
}

/* =======================================================================================================================================*/
// Main function

int main() {
     // Read system state file
    if (!read_input_file("SYSTEM_STATE.txt", allocation, max, total_resources, &n, &m)) {
        printf("FATAL_ERROR: Failed to read SYSTEM_STATE.txt file\n");
        return 1;
    }

    // Calculate initial state using student functions
    calculate_need(allocation, max, need, n, m);
    calculate_available(allocation, total_resources, available, n, m);

    // Print initial system state  
    printf("INITIAL_SYSTEM_STATE:\n");
    printf("Threads=%d Resources=%d\n", n, m);  
    print_total(total_resources, m);
    print_available(available, m);
    print_allocation(allocation, n,m);
    print_max(max, n, m);
    print_need(need, n, m);

    // Execute initial safety check
    execute_safety_check();

    // Read and execute test cases from INPUT.txt
    FILE *testfile = fopen("INPUT.txt", "r");
    if (!testfile) {
        printf("INFO: No INPUT.txt found, running only initial safety check\n");
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    int test_case_num = 1;

    while (fgets(line, sizeof(line), testfile)) {
        // Remove trailing newline and carriage return
        size_t len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[--len] = '\0';
        }

        // Skip empty lines and comments
        if (len == 0 || line[0] == '#') continue;

        printf("\nTEST_CASE_%d:\n", test_case_num++);

        if (strncmp(line, "SAFETY_CHECK", 12) == 0) {
            execute_safety_check();
        } else if (strncmp(line, "REQUEST", 7) == 0) {
            // Parse: REQUEST thread_id r0 r1 r2 ...
            int numbers[MAX_RESOURCES + 1];
            int count = extract_numbers(line, numbers, MAX_RESOURCES + 1);
            
            if (count >= m + 1) {
                int thread_id = numbers[0];
                int request[MAX_RESOURCES];
                for (int j = 0; j < m; j++) {
                    request[j] = numbers[j + 1];
                }
                execute_request(thread_id, request);
                print_available(available, m);
                print_allocation(allocation, n,m);
                print_max(max, n, m);
                print_need(need, n, m);
            } else {
                printf("REQUEST_RESULT: ERROR_INVALID_FORMAT\n");
            }
        } else {
            printf("UNKNOWN_COMMAND: %s\n", line);
        }
    }

    fclose(testfile);   
    return 0;
}