/*
 * ===============================================================================
 * HOMEWORK-6: ROUND ROBIN (RR) CPU SCHEDULER
 * Student Implementation Template
 *
 * INSTRUCTIONS:
 * Complete the implementation of the round_robin_scheduling() function below.
 * Follow the Round Robin algorithm specifications exactly as provided.
 *
 * REQUIREMENTS:
 * - Implement Round Robin (RR) scheduling algorithm with time quantum
 * - Use a circular queue (FIFO) to manage the ready queue
 * - Sort processes by arrival time (use PID for tie-breaking)
 * - Handle process preemption when time quantum expires
 * - Handle CPU idle time when no process is ready
 * - Processes arriving during execution are added to ready queue BEFORE re-queuing current process
 * - Do NOT modify the global ctx->processes[] array order during sorting
 *
 * 💡TIE-BREAKING RULE:
 * - Primary: Arrival Time (ascending - earlier arrivals first)
 * - Secondary: Process ID (ascending - lower PID first)
 * - Example: If P3 and P1 both arrive at time 0, schedule P1 first
 *
 * 💯FORMULAS (as per lecture slides):
 * - Turnaround Time (TAT) = Completion Time - Arrival Time
 * - Waiting Time (WT) = Turnaround Time - CPU Burst Time
 * - Average WT = Sum of all WT / Number of Processes
 * - Average TAT = Sum of all TAT / Number of Processes
 *
 * NOTE: You only need to calculate and store completion_time for each process.
 *       The framework will automatically calculate TAT and WT using the formulas above.
 *
 * ⚠️ GRADING CRITERIA:
 * - Correct RR implementation with proper queue management
 * - Accurate time quantum handling and process preemption
 * - Proper handling of CPU idle time
 * - Correct order of enqueuing: new arrivals BEFORE preempted process
 * - Accurate completion time calculations
 * - Exact output format matching expected results
 * - Code must compile without errors or warnings
 */

#include "rr_scheduler.h"

#define QUEUE_MAX 1000  // Max size for circular queue

/* ===============================================================================
 * CIRCULAR QUEUE STRUCTURE
 * ===============================================================================
 * Used for managing the ready queue in Round Robin scheduling.
 * FIFO structure with wrap-around indexing.
 */
typedef struct {
    int items[QUEUE_MAX];
    int front, rear;
} Queue;

/* ===============================================================================
 * QUEUE OPERATIONS - YOU MUST IMPLEMENT THESE
 * =============================================================================== */

/**
 * @brief Initialize an empty queue
 * @param q Pointer to the queue
 */
static void init_queue(Queue *q) {
    // TODO: Initialize queue to indicate it's empty
    // HINT: Set both front and rear to indicate empty state
    // initialize  both front and rear of queue to -1 to indicate empty queue
    q->front = -1;   
    q->rear = -1;   
}

/**
 * @brief Check if queue is empty
 * @param q Pointer to the queue
 * @return true if queue is empty, false otherwise
 */
static bool is_empty(Queue *q) {
    // TODO: Return whether the queue is empty
    // HINT: Check the front pointer's value
    // queue is considered empty when front is -1
    return q->front == -1;  
}

/**
 * @brief Add a process index to the rear of the queue
 * @param q Pointer to the queue
 * @param pid Process index to enqueue
 */
static void enqueue(Queue *q, int pid) {
    // TODO: Check if queue is full and handle appropriately
    // check if queue is full (rear+1) % QUEUE_MAX == front
    if ((q->rear + 1) % QUEUE_MAX == q->front) {
        return;
    }
    
    // TODO: Handle the case when queue is currently empty
    if (is_empty(q)) {
        q->front = 0;  // set front to 0 for first element
        q->rear = 0;   // set rear to 0 for first element
    } else {
        // TODO: Handle the normal case when queue has elements
        // HINT: Use circular/wrap-around indexing with modulo operator
        q->rear = (q->rear + 1) % QUEUE_MAX;  // wraps around using modulo
    }
    
    // TODO: Store the process index in the queue
    q->items[q->rear] = pid;  // store the process ID at the rear of queue
}

/**
 * @brief Remove and return the front element from the queue
 * @param q Pointer to the queue
 * @return Process index from front of queue, or -1 if empty
 */
static int dequeue(Queue *q) {
    // TODO: Handle empty queue case
    if (is_empty(q)) {
        return -1;  // return empty
    }
    
    // TODO: Get the front element to return
    int pid = q->items[q->front];  // front element of queue
    
    // TODO: Update queue pointers after removal
    // HINT: Consider two cases - queue becoming empty vs queue still having elements
    // HINT: Use circular/wrap-around indexing with modulo operator for the normal case
    if (q->front == q->rear) {
        // queue becomes empty after dequeue
        q->front = -1;
        q->rear = -1;
    } else {
        // queue still containts elements, move front to next element by wrapping around using modulo
        q->front = (q->front + 1) % QUEUE_MAX;
    }
    
    // TODO: Return the dequeued element
    return pid;
}

/* ===============================================================================
 * ROUND ROBIN SCHEDULING ALGORITHM IMPLEMENTATION
 * ===============================================================================
 *
 * @brief Implements the Round Robin scheduling algorithm with time quantum
 *
 * @param ctx Pointer to SchedulerContext containing process information
 * @param time_quantum The maximum time slice allocated to each process
 *
 * ALGORITHM STEPS:
 * 1. Validate input and reset process states
 * 2. Sort processes by arrival time, then by PID (tie-breaker)
 * 3. Initialize tracking arrays and ready queue
 * 4. Enqueue all processes arriving at time 0
 * 5. Main scheduling loop:
 *    a. If queue empty, advance time to next arrival
 *    b. Dequeue front process
 *    c. Execute for min(remaining_time, time_quantum)
 *    d. Update completion time if process finishes
 *    e. Enqueue newly arrived processes (IMPORTANT: before re-queuing current)
 *    f. Re-enqueue current process if not complete
 * 6. Display results
 *
 * CRITICAL ORDERING RULE:
 * When a process uses its time quantum:
 * 1. First enqueue all processes that arrived during execution
 * 2. Then re-enqueue the current process (if not complete)
 * This ensures new arrivals get priority over preempted processes
 *
 * ===============================================================================
 */
void round_robin_scheduling(SchedulerContext *ctx, int time_quantum)
{
    // Step1️⃣: Input validation
    if (time_quantum <= 0 || ctx->num_processes <= 0) return;       
    // Reset all process states using the provided framework function
        reset_process_states(ctx);
    
    // Step2️⃣: Initialize variables
    // TODO: Create a local variable to store number of processes
    int n = ctx->num_processes;  // store number of processes locally to n
    
    // TODO: Initialize current time and completed process counter
    int current_time = 0;  // start time set to 0
    int completed = 0;     // count of completed processes
    
    // Step3️⃣: Declare tracking arrays
    // TODO: Create array to track remaining burst time for each process
    int remaining_time[MAX_PROCESSES];  
    
    // TODO: Create array to track which processes have completed
    bool is_completed[MAX_PROCESSES];   
    
    // TODO: Create array to track which processes are currently in the ready queue
    // HINT: The third array prevents adding duplicate processes to the queue
    bool in_ready_queue[MAX_PROCESSES]; //track if process is in queue
    
    // initialize all tracking arrays
    for (int i = 0; i < n; i++) {
        is_completed[i] = false; // set all processes to not completed
        in_ready_queue[i] = false; // set all processes to not in ready queue
    }
    
    // Step4️⃣: Sort processes by arrival time, then by PID (tie-breaker)
    // TODO: Implement sorting algorithm (bubble sort or selection sort)
    // SORTING LOGIC:
    // - Primary sort key: arrival_time (ascending order)
    // - Secondary sort key: pid (ascending order for tie-breaking)
    // HINT: Use nested loops to compare and swap processes
    
    // use Bubble sort to sort processes by arrival time, then by PID
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            bool swap = false;
            // cmpares by arrival time first
            if (ctx->processes[j].arrival_time > ctx->processes[j+1].arrival_time) {
                swap = true;
            }
            // if arrival times are equal, compare by PID, tie break
            else if (ctx->processes[j].arrival_time == ctx->processes[j+1].arrival_time) {
                if (ctx->processes[j].pid > ctx->processes[j+1].pid) {
                    swap = true;
                }
            }
            // if swap is true, swap processes for bubble sort.
            if (swap) {
                // create temp and swap
                Process temp = ctx->processes[j];
                ctx->processes[j] = ctx->processes[j + 1];
                ctx->processes[j + 1] = temp;
            }
        }
    }
    
    // Step5️⃣: Initialize remaining time for all processes AFTER sorting
    // TODO: Set remaining time equal to burst time for each process
    for (int i = 0; i < n; i++) {
        remaining_time[i] = ctx->processes[i].burst_time;  // nitialize remaining time to burst time
    }
    
    // Step6️⃣: Initialize the ready queue
    // TODO: Declare a Queue variable
    Queue ready_queue;  
    
    // TODO: Initialize the queue using the init_queue function
    init_queue(&ready_queue); 
    
    // Step7️⃣: Enqueue all processes that arrive at time 0
    // TODO: Loop through all processes
    for (int i = 0; i < n; i++) {
        // TODO: Add processes with arrival time 0 to the ready queue
        if (ctx->processes[i].arrival_time == 0) {
            // TODO: Mark these processes as being in the ready queue
            enqueue(&ready_queue, i);  // enqueue process index
            in_ready_queue[i] = true;  // mark process as in queue
        }
    }
    
    // Step8️⃣: Main scheduling loop
    // TODO: Continue looping until all processes are completed
    while (completed < n) {  // loop until all processes are completed
    
        // Step 8a: Handle CPU idle time (when ready queue is empty)
        // HINT: Search through incomplete processes to find minimum arrival time greater than current time
        // TODO: Check if the ready queue is empty
        if (is_empty(&ready_queue)) {
            // TODO: If empty, find the next process arrival time
            int next_arrival = INT_MAX;  // start with highest possible value to compare and find smaller arrival time
            
            // search through incomplete processes to find minimum arrival time greater than current time
            for (int i = 0; i < n; i++) {
                if (!is_completed[i] && ctx->processes[i].arrival_time > current_time) {
                    if (ctx->processes[i].arrival_time < next_arrival) {
                        next_arrival = ctx->processes[i].arrival_time;
                    }
                }
            }
            
            // TODO: Advance current time to that arrival
            // checks if next arrival time is valid
            if (next_arrival != INT_MAX) {
                current_time = next_arrival;  
            }
            
            // TODO: Enqueue all processes arriving at that time
            for (int i = 0; i < n; i++) {
                if (!is_completed[i] && ctx->processes[i].arrival_time == current_time && !in_ready_queue[i]) {
                    // TODO: Mark them as being in the ready queue
                    enqueue(&ready_queue, i);  // enqueue process to ready q
                    in_ready_queue[i] = true;  // marks as in q
                }
            }
            
            // TODO: Continue to next iteration
            
            continue;  
        }
        
        // Step 8b: Get the next process from ready queue
        // TODO: Dequeue a process from the front of the ready queue
        int current_index = dequeue(&ready_queue);  // get process index from queue set to current
        
        // TODO: Mark this process as no longer in the ready queue
        in_ready_queue[current_index] = false;  // mark process as not in queue
        
        // TODO: Get a pointer to this process from the context
        Process *current_process = &ctx->processes[current_index];  // creats a process pointer to the current index
        
        // Step 8c: Determine execution time for this CPU burst
        // TODO: Calculate how long this process will execute
        // HINT: Process executes for either its remaining time or the time quantum, whichever is smaller
        int execution_time = min_value(remaining_time[current_index], time_quantum);  // calculates execution time by min of remaining time and time quantum
        
        // TODO: Save the current time before execution starts (needed for checking new arrivals)
        int start_time = current_time;  // save start time to check for arrivals
        
        // Step 8d: Execute the process
        // TODO: Reduce the remaining time of the process
        remaining_time[current_index] -= execution_time;  //reduces remaining time by execution time
        
        // TODO: Advance current time by the execution time
        current_time += execution_time; 
        
        // Step 8e: Check if process completed its execution
        // TODO: If remaining time is zero, mark process as completed
        if (remaining_time[current_index] == 0) {
            is_completed[current_index] = true;  // marks as completed
            
            // TODO: Record the completion time
            current_process->completion_time = current_time; //sets the completion time to the current 
            
            // TODO: Increment the completed process counter
            completed++; 
        }
        
        // Step 8f: ⚠️ CRITICAL STEP - Enqueue newly arrived processes FIRST
        // TODO: Loop through all processes to find new arrivals
        for (int i = 0; i < n; i++) {
            // TODO: Check if each process arrived during the execution window
            // HINT: A process arrives during execution if its arrival time is between the start time (exclusive) and current time (inclusive)
            if (ctx->processes[i].arrival_time > start_time && ctx->processes[i].arrival_time <= current_time) {
                // TODO: Verify the process is not completed, not already in queue, and not the currently executing process
                if (!is_completed[i] && !in_ready_queue[i] && i != current_index) {
                    // TODO: If conditions are met, add to ready queue and mark as in queue
                    enqueue(&ready_queue, i);  
                    in_ready_queue[i] = true;  
                }
            }
        }
        
        // Step 8g: Re-enqueue the current process if it hasn't finished
        // TODO: Check if the process still has remaining time
        if (remaining_time[current_index] > 0) { //checks if process any time left
            // TODO: If yes, add it back to the ready queue (goes to the back)
            enqueue(&ready_queue, current_index);  // re-enqueue 
            
            // TODO: Mark it as being in the ready queue
            in_ready_queue[current_index] = true; 
        }
    } 
    
    // Step9️⃣: Display results
    display_results(ctx, "RR");
}

/*
 * Common Mistakes to Avoid:
 * -------------------------
 * ❌ Not sorting processes by arrival time before scheduling
 * ❌ Forgetting to initialize remaining_time array AFTER sorting
 * ❌ Not handling CPU idle time when queue is empty
 * ❌ Incorrect queue implementation (not using circular indexing)
 * ❌ Adding duplicate processes to queue (use in_ready_queue tracking)
 * ❌ Wrong order: Must enqueue new arrivals BEFORE re-queuing current process
 * ❌ Not checking if process arrived during execution window
 * ❌ Forgetting to mark processes as completed when remaining_time becomes 0
 * ❌ Not recording completion_time when process finishes
 * ❌ Incorrect use of min_value() for execution time calculation
 * 
 * 💡 Remember: The framework provides min_value(), reset_process_states(), 
 *    and display_results() - use them correctly!
 */