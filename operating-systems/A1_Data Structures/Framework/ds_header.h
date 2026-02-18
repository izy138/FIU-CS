/**
 * @file ds_header.h
 * @brief Header file for kernel data structure implementations.
 * 
 * STUDENT INSTRUCTIONS:
 * - Implement ONLY the function bodies in separate .c files
 * - Do NOT modify this header file
 * - Do NOT add any printf statements in your implementations (the driver handles all output)
 * - Focus purely on algorithm correctness
 * - The testing framework handles all input/output
 */

#ifndef DS_HEADER_H
#define DS_HEADER_H

#include <stdio.h>    
#include <stdlib.h>   
#include <limits.h>   
#include <string.h>
#include <ctype.h>
#define MAX_SIZE 100

/* ----------------- STACK ----------------- */
/**
 * Stack - Structure representing a LIFO stack.
 * @data: Array holding stack elements
 * @top: Index of the topmost element
 */
typedef struct {
    int data[MAX_SIZE];
    int top;
} Stack;

// STUDENT TODO: Implement these functions in stack.c
void init_stack(Stack* stack);
int is_stack_empty(Stack* stack);
int is_stack_full(Stack* stack);
int push(Stack* stack, int element);          
int pop(Stack* stack);                        
int peek_stack(Stack* stack);                 
int stack_size(Stack* stack);                 
int list_stack_elements(Stack* stack);        

/* ------------- CIRCULAR QUEUE ------------ */
/**
 * CircularQueue - FIFO queue with circular wrap-around.
 * @data: Storage array
 * @front: Index of front element
 * @rear: Index of rear element
 * @count: Number of elements currently in queue
 */
typedef struct {
    int data[MAX_SIZE];
    int front;
    int rear;
    int count;
} CircularQueue;

// STUDENT TODO: Implement these functions in circular_queue.c
void init_queue(CircularQueue* queue);
int is_queue_empty(CircularQueue* queue);
int is_queue_full(CircularQueue* queue);
int enqueue(CircularQueue* queue, int element);  
int dequeue(CircularQueue* queue);               
int peek_queue(CircularQueue* queue);            
int queue_size(CircularQueue* queue);            
int list_queue_elements(CircularQueue* queue);   

/* ----------- CIRCULAR LINKED LIST -------- */
/**
 * Node - Single node in circular linked list.
 * @data: Data stored in the node
 * @next: Pointer to the next node in the list
 */
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// STUDENT TODO: Implement these functions in circular_linked_list.c
Node* create_node(int data);                    
int is_list_empty(Node* head);                  
Node* insert_node(Node* head, Node* new_node);  
Node* delete_node(Node* head, int key);         
Node* search_node(Node* head, int key);         
int iterate_list(Node* head);                   
int free_list(Node* head);                      

/* --------------- MIN HEAP ---------------- */
/**
 * HeapNode - Node storing value with associated priority.
 * @value: Payload data
 * @priority: Sorting key (lower number = higher priority)
 */
typedef struct {
    int value;
    int priority;
} HeapNode;

/**
 * MinHeap - Min-heap structure for priority queue.
 * @data: Array of HeapNodes
 * @size: Current number of elements in the heap
 */
typedef struct {
    HeapNode data[MAX_SIZE];
    int size;
} MinHeap;

// STUDENT TODO: Implement these functions in min_heap.c
void insert_element(MinHeap* heap, HeapNode element);
void remove_element(MinHeap* heap, int index);
HeapNode extract_min(MinHeap* heap);            
HeapNode peek_heap(MinHeap* heap);              
int decrease_key(MinHeap* heap, int index, int new_priority);
void min_heapify(MinHeap* heap, int index);
int is_heap_empty(MinHeap* heap);               
int list_heap_elements(MinHeap* heap);          

/* --------------- BITMAP ---------------- */
/**
 * Bitmap - Efficient bit manipulation for kernel operations.
 * Used in: page frame tracking, CPU masks, process scheduling
 * 
 * Global bitmap implementation using fixed-size arrays.
 * Maximum supported size: MAX_BITMAP_BITS bits
 */

#define MAX_BITMAP_BITS 64                      // Maximum number of bits supported
#define MAX_BITMAP_BYTES (MAX_BITMAP_BITS / 8)  // Maximum bytes needed (8 bytes)

// Global bitmap storage
extern unsigned char bitmap_data[MAX_BITMAP_BYTES];
extern int current_bitmap_size;

// STUDENT TODO: Implement these functions in bitmap.c
void init_bitmap(int size);                     // Initialize bitmap with specified size (PROVIDED - no need to implement)
void set_bit(int n);                           // Set bit n to 1
void clear_bit(int n);                         // Clear bit n to 0
int test_bit(int n);                           // Test if bit n is set (returns 1/0/-1)
int find_first_zero_bit(void);                 // Find first unset bit
int find_next_set_bit(int start);              // Find next set bit after position
void print_bitmap(void);                       // Print bitmap in binary format
int is_bitmap_empty(void);                     // Check if all bits are 0
int bitmap_size(void);                         // Get current bitmap size

#endif // DS_HEADER_H