// Stack implementation (LIFO) - Skeleton file
// See ds_header.h for structure and prototype definitions

#include "ds_header.h"

/******************************************************************
  👍STUDENT IMPLEMENTATION -Students implement these 8 functions
******************************************************************/

//------------------------------------------------------------------//

/**
 * init_stack - Initialize a new stack
 * @param Stack: Pointer to the stack structure to initialize
 */
void init_stack(Stack *s)
{
    // TODO: Initialize the stack by setting top index to indicate empty stack
    s->top = -1; // top = -1 means no elements in stack
}

//------------------------------------------------------------------//

/**
 * is_stack_empty - Check if the stack is empty
 * @param Stack: Pointer to the stack
 * @return: 1 if stack is empty, 0 otherwise
 */
int is_stack_empty(Stack *stack)
{
    // TODO: Check if the top index indicates empty stack
    return (stack->top == -1); // returns true - 1 if empty and false - 0 if not empty
}

//------------------------------------------------------------------//

/**
 * is_stack_full - Check if the stack is full
 * @param Stack: Pointer to the stack
 * @return: 1 if stack is full, 0 otherwise
 */
int is_stack_full(Stack *stack)
{
    // TODO: Check if the top index reached maximum capacity
    return (stack->top == MAX_SIZE - 1); // return true 1 if stakc is full and 0 if not full
}

//------------------------------------------------------------------//

/**
 * push - Push an element onto the stack
 * @param Stack: Pointer to the stack
 * @param element: Element to push onto the stack
 * @return: -1 if stack is full, 0 otherwise
 */
int push(Stack *stack, int element)
{
    // TODO: Add element to the top of the stack if not full
    if (is_stack_full(stack))
    {
        return -1;
    }
    // increment and add
    stack->top++;
    stack->data[stack->top] = element;
    return 0;
}

//------------------------------------------------------------------//

/**
 * pop - Remove and return the top element from the stack
 * @param Stack: Pointer to the stack
 * @return: Top element if stack is not empty, -1 otherwise
 */
int pop(Stack *stack)
{
    // TODO: Remove and return the top element
    if (is_stack_empty(stack))
    {
        return -1;
    }
    int element = stack->data[stack->top];
    stack->top--;
    return element;
}

//------------------------------------------------------------------//

/**
 * peek_stack - Return the top element without removing it
 * @param Stack: Pointer to the stack
 * @return: Top element if stack is not empty, -1 otherwise
 */
int peek_stack(Stack *stack)
{
    // TODO: Return the top element without popping
    if (is_stack_empty(stack))
    {
        // printf("Stack is empty");
        return -1;
    }
    return stack->data[stack->top];
}

//------------------------------------------------------------------//

/**
 * stack_size - Return the number of elements in the stack
 * @param Stack: Pointer to the stack
 * @return: Number of elements currently in the stack
 */
int stack_size(Stack *stack)
{
    // TODO: Calculate size based on top index
    return stack->top + 1;
}

//------------------------------------------------------------------//

/**
 * list_stack_size - Prints all elements in the stack from top to bottom
 * @param Stack Pointer to the Stack
 * @return: -1 if stack is empty, 0 otherwise after listing elements
 */
int list_stack_elements(Stack *stack)
{
    // TODO: List the elements if stack is not empty
    if (is_stack_empty(stack))
    {
        printf("Stack is empty");
        return -1;
    }
    // int i = stack->top;
    for (int i = stack->top; i >= 0; i--)
    {
        printf("%d ", stack->data[i]);
    }
    return 0;
}

//------------------------------------------------------------------//