// Min-Heap  - Skeleton file
// See ds_header.h for structure and prototype definitions
#include "ds_header.h"

// Helpful macros (for easier heap navigation):
#define LEFT(i) (2 * (i) + 1)
#define RIGHT(i) (2 * (i) + 2)
#define PARENT(i) (((i) - 1) / 2)

/******************************************************************
  👍STUDENT IMPLEMENTATION -Students implement these 8 functions
******************************************************************/

//------------------------------------------------------------------//

/**
 * insert_element - Insert a new element into the min-heap
 * @param heap: Pointer to the MinHeap structure
 * @param element: The HeapNode to insert
 * @return: Simply return if the Heap is full
 */
void insert_element(MinHeap *heap, HeapNode element)
{
    /* TODO: Add the element to the end and bubble up to maintain heap property */
    // null heap, if the heap is full = cannot insert
    if (heap == NULL || heap->size >= MAX_SIZE)
    {
        return;
    }
    // add the element to the end of the heap
    heap->data[heap->size] = element;
    int current = heap->size;
    heap->size++;

    // continue to move current until its in the right spot
    while (current > 0 && heap->data[PARENT(current)].priority > heap->data[current].priority)
    {
        // swap the current data with the parent data
        HeapNode temp = heap->data[current];
        heap->data[current] = heap->data[PARENT(current)];
        heap->data[PARENT(current)] = temp;

        current = PARENT(current);
    }
}

//------------------------------------------------------------------//

/**
 * remove_element - Remove an element at index i from the min-heap
 * @param heap: Pointer to the MinHeap structure
 * @param i: Index of the element to remove
 */
void remove_element(MinHeap *heap, int i)
{
    /* TODO: Decrease the priority to INT_MIN, then extract the min */
    if (heap == NULL || i < 0 || i >= heap->size)
    { // check null and bounds
        return;
    }
    // decreases the priority to the minimum possible
    decrease_key(heap, i, INT_MIN);
    // extracts the minimum - the current root of the heap
    extract_min(heap);
}

//------------------------------------------------------------------//

/**
 * extract_min - Extract and return the element with minimum priority
 * @param heap: Pointer to the MinHeap structure
 * @return: HeapNode with minimum priority, or {-1, INT_MAX} if heap is empty/NULL
 */
/* TODO: Check whether Heap empty if not, remove the root node and re-heapify */

HeapNode extract_min(MinHeap *heap)
{
    // dummy holds the minimum priority possible
    HeapNode dummy = {-1, INT_MAX};
    if (heap == NULL || heap->size == 0)
    {
        return dummy;
    }
    // stores the root - the minimum element in heap
    HeapNode min = heap->data[0];
    // moves the last element of the heap to the root
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;

    if (heap->size > 0)
    {
        // heapify from the root
        min_heapify(heap, 0);
    }
    return min;
}

//------------------------------------------------------------------//

/**
 * peek_heap - Return the element with the lowest priority without removing it
 * @param heap: Pointer to the MinHeap structure
 * @return: HeapNode at the top of the heap, or dummy node {-1, INT_MAX} if empty/invalid
 */
HeapNode peek_heap(MinHeap *heap)
{
    /* TODO: Check whether Heap empty
             if not, Return the root of the heap */
    // dummy node returns if empty/or invalid heap
    HeapNode dummy = {-1, INT_MAX};
    if (heap == NULL || heap->size == 0)
    {
        return dummy;
    }
    // return the root
    return heap->data[0];
}

//------------------------------------------------------------------//

/**
 * decrease_key - Decrease the priority of a node and maintain heap order
 * @param heap: Pointer to the MinHeap structure
 * @param i: Index of the element to update
 * @param new_priority: The new, lower priority value
 */
int decrease_key(MinHeap *heap, int i, int new_priority)
{
    /* TODO: Update the priority and bubble up */
    // if null / check bounds
    if (heap == NULL || i < 0 || i >= heap->size)
    {
        return -1; // Invalid index
    }

    // check new priority is greater than current priority
    if (new_priority > heap->data[i].priority)
    {
        return -2; // means the new prio is higher, in minheap the priority will always go down.
    }

    // update priority
    heap->data[i].priority = new_priority;

    // bubble up from the current position
    int current = i;
    while (current > 0 && heap->data[PARENT(current)].priority > heap->data[current].priority)
    {
        // swap current with parent
        HeapNode temp = heap->data[current];
        heap->data[current] = heap->data[PARENT(current)];
        heap->data[PARENT(current)] = temp;

        // move to parent position
        current = PARENT(current);
    }

    return 0;
}

//------------------------------------------------------------------//

/**
 * min_heapify - Restore heap property from index i downward
 * @param heap: Pointer to the MinHeap structure
 * @param i: Index from where to start heapifying down
 */
void min_heapify(MinHeap *heap, int i)
{
    /* TODO: Compare with children and push down recursively */
    if (heap == NULL || i >= heap->size)
    {
        return;
    }
    int left = LEFT(i);
    int right = RIGHT(i);
    int smallest = i;

    // finds the smallest prio between the current node and its children
    if (left < heap->size && heap->data[left].priority < heap->data[smallest].priority)
    {
        smallest = left;
    }

    if (right < heap->size && heap->data[right].priority < heap->data[smallest].priority)
    {
        smallest = right;
    }

    // if the current is not the smallest prio, it will swap and keep heapifying
    if (smallest != i)
    {
        HeapNode temp = heap->data[i];
        heap->data[i] = heap->data[smallest];
        heap->data[smallest] = temp;

        // recursion for heapify
        min_heapify(heap, smallest);
    }
}

//------------------------------------------------------------------//

/**
 * is_heap_empty - Checks if the heap is empty
 * @param heap Pointer to the MinHeap structure
 * @return: 1 if the heap is empty, 0 otherwise
 */
int is_heap_empty(MinHeap *heap)
{
    /* TODO: Return whether heap is is zero or not */
    if (heap == NULL)
    {
        return 1;
    }
    return (heap->size == 0) ? 1 : 0; // 000
}

//------------------------------------------------------------------//

/**
 * Prints all elements in the min heap along with their priorities and indices
 * @param heap Pointer to the MinHeap structure
 * @return: -1 if error, 0 if heap is empty, or the number of elements printed
 */
int list_heap_elements(MinHeap *heap)
{
    // TODO: Loop through all elements and print each one with its index, value and priority

    // head is null
    if (heap == NULL)
    {
        return -1; // Error
    }

    // head is empty
    if (heap->size == 0)
    {
        return 0; // Empty heap
    }

    // iterate through each element and print
    for (int i = 0; i < heap->size; i++)
    {
        printf("i->[%d] v->%d, p->%d", i, heap->data[i].value, heap->data[i].priority);
        if (i < heap->size - 1)
        {
            printf("; ");
        }
    }
    printf("; ");

    return heap->size; // Number of elements printed
}

//------------------------------------------------------------------//