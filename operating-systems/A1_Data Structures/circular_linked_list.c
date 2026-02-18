// Circular Linked List - Skeleton file
// See ds_header.h for structure and prototype definitions

#include "ds_header.h"

/******************************************************************
  👍STUDENT IMPLEMENTATION -Students implement these 7 functions
******************************************************************/

//------------------------------------------------------------------//
/**
 * create_node - Allocate and initialize a new node
 * @param data: Value to store in the new node
 * @return: Pointer to the newly created node, EXIT application otherwise
 */
Node *create_node(int data)
{
    // TODO: Allocate and initialize node
    // allocate space
    Node *node = (Node *)malloc(sizeof(Node));

    //
    if (node == NULL)
    {
        exit(1);
    }
    // set data
    node->data = data;
    node->next = NULL; // updated later
    return node;
}

//------------------------------------------------------------------//

/**
 * is_list_empty - Check if the list is empty
 * @param head: Pointer to head of list
 * @return: 1 if empty, 0 otherwise
 */
int is_list_empty(Node *head)
{
    // TODO: Return whether head is NULL
    if (head == NULL)
    {
        return 1;
    }
    return 0;
}

//------------------------------------------------------------------//
/**
 * insert_node - Insert node into circular list
 * @param head: Current head of list
 * @param new_node: Node to insert
 * @return: New head of the list
 */
Node *insert_node(Node *head, Node *new_node)
{
    // TODO: Insert node into circular list
    if (new_node == NULL)
    {
        return head;
    }
    if (head == NULL)
    {                              // empty list
        new_node->next = new_node; // make the node point to itself (circular)
        return new_node;
    }

    Node *last = head; // list with data in it
    while (last->next != head)
    {                      // iterates through the nodes until the end
        last = last->next; // the last node is found
    }
    last->next = new_node; // sets the new node to be added in after the last element in list
    new_node->next = head; // sets the head as the node after it (circular)
    return head;
}

//------------------------------------------------------------------//

/**
 * delete_node - Delete a node by key
 * @param head: Head of list
 * @param key: Data value to remove
 * @return: New head of the list if it originally had two or more nodes.
 *          Returns NULL if the list was empty or had only one node (which is deleted).
 */
Node *delete_node(Node *head, int key)
{
    // TODO: Remove node with matching data
    if (is_list_empty(head))
    {
        return NULL;
    }

    if (head->next == head)
    {
        if (head->data == key)
        {
            free(head);
            return NULL;
        }
        return head;
    }

    // deleting head node
    if (head->data == key)
    {
        // first find last node
        Node *last = head;
        while (last->next != head)
        {
            last = last->next;
        }
        // update the last node to point to new head
        last->next = head->next;
        Node *new_head = head->next;
        free(head);
        return new_head;
    }

    Node *prev = head;
    Node *current = head->next;
    // search for the node to delete
    while (current != head)
    {
        if (current->data == key)
        {
            prev->next = current->next;
            free(current);
            return head;
        }
        prev = current;
        current = current->next;
    }

    // Key not found
    return head;
}

//------------------------------------------------------------------//

/**
 * search_node - Search for a node by key
 * @param head: Head of list
 * @param key: Data value to find
 * @return: Pointer to matching node or NULL if key not found or list is empty
 */
Node *search_node(Node *head, int key)
{
    // TODO: Search circular list for key
    // empty list
    if (head == NULL)
    {
        return NULL;
    }
    // non empty
    Node *current = head;
    do
    {
        if (current->data == key)
        { // checks if current is a match
            return current;
        }
        current = current->next; // iterates through each node

    } while (current != head); // stops when it gets back to the head
    return NULL; // key not found
}

//------------------------------------------------------------------//

/**
 * iterate_list - Traverse and print all nodes
 * @param head: Head of list
 * @return: -1 if list is empty, 0 otherwise
 */
int iterate_list(Node *head)
{
    // TODO: Loop through and print each node's data

    if (head == NULL)
    { // set head to empty
        return -1;
    }
    Node *current = head;
    do
    {
        printf("%d ", current->data);
        current = current->next; // iterate through each node and prints
    } while (current != head); // stops at the end of list
    // printf("\n");
    return 0;
}

//------------------------------------------------------------------//

/**
 * free_list - Frees all nodes in a circular linked list
 * @param head: Head of list
 * @return: -1 if list is already empty, 0 if memory was successfully freed
 */
int free_list(Node *head)
{
    if (head == NULL)
    {
        return -1;
    }

    // only head node
    if (head->next == head)
    {
        free(head);
        return 0;
    }

    // more than one node we first break the list
    Node *current = head;
    Node *next_node;

    // we find the last node to break the link to the head node
    Node *last = head;
    while (last->next != head)
    {
        last = last->next;
    }
    last->next = NULL; // setting the last->next to null breaks it

    // iterate through and free
    while (current != NULL)
    {
        next_node = current->next;
        free(current);
        current = next_node;
    }

    return 0;
}
//------------------------------------------------------------------//
