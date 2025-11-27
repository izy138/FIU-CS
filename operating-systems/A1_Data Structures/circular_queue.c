// Circular Queue (FIFO) - Skeleton file
// See ds_header.h for structure and prototype definitions

#include "ds_header.h"

/******************************************************************
  👍STUDENT IMPLEMENTATION -Students implement these 8 functions
******************************************************************/

//------------------------------------------------------------------//

/**
 * init_queue - Initialize a new circular queue
 * @param queue: Pointer to the CircularQueue structure to initialize
 */
void init_queue(CircularQueue *queue)
{
    // TODO: Initialize (front = rear = -1) to indicate empty state.
    //
    // initialize (count = 0) to make numbre of elements zero initially
    queue->front = -1;
    queue->rear = -1;
    queue->count = 0;
}

//------------------------------------------------------------------//

/**
 * is_queue_empty - Check if the queue is empty
 * @param queue: Pointer to the queue
 * @return: 1 if empty, 0 otherwise
 */
int is_queue_empty(CircularQueue *queue)
{
    // TODO: Return 1 if queue is empty
    return (queue->count == 0);
}

//------------------------------------------------------------------//

/**
 * is_queue_full - Check if the queue is full
 * @param queue: Pointer to the queue
 * @return: 1 if full, 0 otherwise
 */
int is_queue_full(CircularQueue *queue)
{
    // TODO: Return 1 if queue is full using circular condition
    return (queue->count == MAX_SIZE);
}

//------------------------------------------------------------------//

/**
 * enqueue - Add an element to the rear of the queue
 * @param queue: Pointer to the queue
 * @param element: Element to add
 * @return: 0 on success, -1 if queue is full
 */
int enqueue(CircularQueue *queue, int element)
{
    // TODO: Add an element to the rear of the queue
    if (is_queue_full(queue))
    { // checks if queue is full so you can wrap around to first
        return -1;
    }
    if (is_queue_empty(queue))
    {
        queue->front = 0; // set front pointer to 0 - start
        queue->rear = 0;  // set rear pointer to 0
    }
    else
    {
        // moves the rear pointer to the next position
        queue->rear = (queue->rear + 1) % MAX_SIZE; // position 1 / 5 = 1; position 5 /5 = 0
    }
    // store the data at rear pointer
    queue->data[queue->rear] = element;
    queue->count++; // increase count
    return 0;
}

//------------------------------------------------------------------//

/**
 * dequeue - Remove and return the front element of the queue
 * @param queue: Pointer to the queue
 * @return: front element on success, -1 if queue is empty
 */
int dequeue(CircularQueue *queue)
{
    // TODO: Remove and return the front element of the queue
    if (is_queue_empty(queue))
    {
        return -1;
    }

    // the element we are removing is at the front pointer
    int element = queue->data[queue->front];

    // if its the only element in the queue you reset the positions for front/rear
    if (queue->count == 1)
    {
        queue->front = -1;
        queue->rear = -1;
    }
    else
    {                                                 // increment the front point by 1
        queue->front = (queue->front + 1) % MAX_SIZE; // 0+1 = 1 % 5 = 1
    }
    // lower the count because we are removing it
    queue->count--;
    return element;
}

//------------------------------------------------------------------//

/**
 * peek_queue - Return the front element without removing it
 * @param queue: Pointer to the queue
 * @return: Front element if not empty, -1 if queue is empty
 */
int peek_queue(CircularQueue *queue)
{
    // TODO: Return the front element without removing it
    if (is_queue_empty(queue))
    {
        return -1;
    }
    return queue->data[queue->front];
}

//------------------------------------------------------------------//

/**
 * queue_size - Return the number of elements in the queue
 * @param queue: Pointer to the queue
 * @return: Number of elements in queue
 */
int queue_size(CircularQueue *queue)
{
    // TODO: Calculate queue size accounting for circular wrap-around
    return queue->count;
}

//------------------------------------------------------------------//

/**
 * list_queue_elements - Prints all elements in the queue from front to rear
 * @param queue: Pointer to the Queue
 * @return: -1 if queue is empty, 0 otherwise after listing elements
 */
int list_queue_elements(CircularQueue *queue)
{
    // TODO: List the elements if stack is not empty
    if (is_queue_empty(queue))
    { // no elements to list
        return -1;
    }

    int curr = queue->front;
    for (int i = 0; i < queue->count; i++)
    {
        printf("%d ", queue->data[curr]);
        curr = (curr + 1) % MAX_SIZE;
    }
    // printf("\n");
    return 0;
}

//------------------------------------------------------------------//