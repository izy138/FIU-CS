#include "ds_header.h"
#define LINE_LENGTH 100

void trim(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = 0;
}

void to_uppercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

int main() {
    Stack s;
    CircularQueue q;
    Node *list_head = NULL;
    MinHeap heap;
    heap.size = 0;

    init_stack(&s);
    init_queue(&q);

    FILE *file = fopen("TESTCASES.txt", "r");
    if (!file) {
        printf("Error opening TESTCASES.txt");
        return 1;
    }

    char line[LINE_LENGTH];
    char uppercase_line[LINE_LENGTH];
    
    while (fgets(line, sizeof(line), file)) {
        trim(line);
        
        // Skip comments, line with '+' or blank lines
        if (line[0] == '/' || line[0] == '+' ||line[0] == '\0')
            continue;
        
        strcpy(uppercase_line, line);
        to_uppercase(uppercase_line);
        
        // STACK OPERATIONS
        if (strncmp(uppercase_line, "PUSH", 4) == 0) {
            int val;
            sscanf(line + 5, "%d", &val);
            if (push(&s, val) == -1)
                printf("STACK: Error - Stack Overflow\n");
            else          
                printf("STACK: Pushed into stack: %d\n", val);            
        } 
        else if (strncmp(uppercase_line, "POP", 3) == 0) {
            int result = pop(&s);
            if (result == -1)
                printf("STACK: Error - Stack Empty\n");
            else
                printf("STACK: Popped from stack: %d\n", result);            
        } 
        else if (strncmp(uppercase_line, "PEEK_STACK", 10) == 0) {
            int result = peek_stack(&s);
            if (result == -1)
                printf("STACK: Error - Stack Empty\n");
            else
                printf("STACK: Top of stack: %d\n", result);
        } 
        else if (strncmp(uppercase_line, "IS_STACK_EMPTY", 14) == 0) {
            printf("STACK: Stack empty: %s\n", is_stack_empty(&s) ? "Yes" : "No");
        } 
        else if (strncmp(uppercase_line, "IS_STACK_FULL", 13) == 0) {
            printf("STACK: Stack full: %s\n", is_stack_full(&s) ? "Yes" : "No");
        } 
        else if (strncmp(uppercase_line, "STACK_SIZE", 10) == 0) {
            printf("STACK: Stack size: %d\n", stack_size(&s));           
        }
        else if (strncmp(uppercase_line, "LIST_STACK_ELEMENTS", 19) == 0) {
            if (is_stack_empty(&s)) {
                printf("STACK: Error - Stack Empty\n");
            } else {
                printf("STACK: Stack elements from top to bottom: ");
                list_stack_elements(&s);
                printf("\n");
            }
        }  

        // QUEUE OPERATIONS    
        else if (strncmp(uppercase_line, "ENQUEUE", 7) == 0) {
            int val;
            sscanf(line + 8, "%d", &val);
            if (enqueue(&q, val) == -1)
                printf("QUEUE: Error - Queue Full\n");
            else
                printf("QUEUE: Enqueued: %d\n", val);                
        } 
        else if (strncmp(uppercase_line, "DEQUEUE", 7) == 0) {
            int result = dequeue(&q);
            if (result == -1)
                printf("QUEUE: Error - Queue Empty\n");
            else
                printf("QUEUE: Dequeued: %d\n", result);
        } 
        else if (strncmp(uppercase_line, "PEEK_QUEUE", 10) == 0) {
            int result = peek_queue(&q);
            if(result == -1)
                printf("QUEUE: Error - Queue Empty\n");
            else 
                printf("QUEUE: Front of queue: %d\n", result);
        } 
        else if (strncmp(uppercase_line, "IS_QUEUE_EMPTY", 14) == 0) {
            printf("QUEUE: Queue empty: %s\n", is_queue_empty(&q) ? "Yes" : "No");
        } 
        else if (strncmp(uppercase_line, "IS_QUEUE_FULL", 13) == 0) {
            printf("QUEUE: Queue full: %s\n", is_queue_full(&q) ? "Yes" : "No");
        } 
        else if (strncmp(uppercase_line, "QUEUE_SIZE", 10) == 0) {
            printf("QUEUE: Queue size: %d\n", queue_size(&q));        
        }
        else if (strncmp(uppercase_line, "LIST_QUEUE_ELEMENTS", 19) == 0) {
            if (is_queue_empty(&q)) {
                printf("QUEUE: Error - Queue Empty\n");
            } else {
                printf("QUEUE: Queue elements from front to rear: ");
                list_queue_elements(&q);
                printf("\n");
            }
        }   

        // LIST OPERATIONS
        else if (strncmp(uppercase_line, "INSERT_NODE", 11) == 0) {
            int val;
            sscanf(line + 12, "%d", &val);
            Node *node = create_node(val);
            list_head = insert_node(list_head, node);
            printf("LIST: Node inserted is: %d\n", val);
        } 
        else if (strncmp(uppercase_line, "DELETE_NODE", 11) == 0) {
            int key;
            sscanf(line + 12, "%d", &key);
            list_head = delete_node(list_head, key);
            printf("LIST: Node deleted is: %d\n", key);
        } 
        else if (strncmp(uppercase_line, "SEARCH_NODE", 11) == 0) {
            int key;
            sscanf(line + 12, "%d", &key);
            Node *found = search_node(list_head, key);
            printf("LIST: Node %d %s\n", key, found ? "found" : "not found");
        } 
        else if (strncmp(uppercase_line, "ITERATE_LIST", 12) == 0) {
            if (is_list_empty(list_head)) {
                printf("LIST: List is empty\n");
            } else {
                printf("LIST: List nodes are: ");
                iterate_list(list_head);
                printf("\n");
            }
        }
        else if (strncmp(uppercase_line, "IS_LIST_EMPTY", 13) == 0) {
            printf("LIST: List empty: %s\n", is_list_empty(list_head) ? "Yes" : "No");
        }
        else if (strncmp(uppercase_line, "FREE_LIST", 9) == 0) {
            if (free_list(list_head) == -1)
                printf("LIST: List is already empty\n");
            else    
                printf("LIST: List has been emptied\n");
            list_head = NULL;
        } 
        
        // HEAP OPERATIONS
        else if (strncmp(uppercase_line, "INSERT_ELEMENT", 14) == 0) {
            int val, priority;
            sscanf(line + 15, "%d %d", &val, &priority);
            HeapNode node = {val, priority};
            insert_element(&heap, node);
            printf("HEAP: Inserted element: value=%d, priority=%d\n", val, priority);
        } 
        else if (strncmp(uppercase_line, "EXTRACT_MIN", 11) == 0) {
            if (heap.size == 0) {
                printf("HEAP: Error - Heap Empty\n");
            } else {
                HeapNode min = extract_min(&heap);
                printf("HEAP: Extracted min: value=%d, priority=%d\n", min.value, min.priority);
            }
        } 
        else if (strncmp(uppercase_line, "PEEK_HEAP", 9) == 0) {
            if (heap.size == 0) {
                printf("HEAP: Error - Heap Empty\n");
            } else {
                HeapNode min = peek_heap(&heap);
                printf("HEAP: Top of heap: value=%d, priority=%d\n", min.value, min.priority);
            }
        } 
        else if (strncmp(uppercase_line, "DECREASE_KEY", 12) == 0) {
            int index, new_priority;
            sscanf(line + 13, "%d %d", &index, &new_priority);
            int status = decrease_key(&heap, index, new_priority);
            if (status == -2)
                printf("HEAP: Error - New priority is higher than current\n");
            else
                printf("HEAP: Decreased priority of element at index %d to %d\n", index, new_priority);           
        } 
        else if (strncmp(uppercase_line, "REMOVE_ELEMENT", 14) == 0) {
            int index;
            sscanf(line + 15, "%d", &index);
            remove_element(&heap, index);
            printf("HEAP: Removed element at index %d\n", index);
        }
        else if (strncmp(uppercase_line, "IS_HEAP_EMPTY", 13) == 0) {
             printf("HEAP: Heap empty: %s\n", is_heap_empty(&heap) ? "Yes" : "No");
        } 
        else if (strncmp(uppercase_line, "LIST_HEAP_ELEMENTS", 18) == 0) {
            if (is_heap_empty(&heap)) {
                printf("HEAP: Error - Heap Empty\n");
            } else {
                printf("HEAP: Heap elements (Index[i], Value[v], Priority[p]): ");
                list_heap_elements(&heap);
                printf("\n");
            }
        }   
        
        // BITMAP OPERATIONS
        else if (strncmp(uppercase_line, "INIT_BITMAP", 11) == 0) {
            int size;
            sscanf(line + 12, "%d", &size);
            init_bitmap(size);
            printf("BITMAP: Initialized bitmap with %d bits\n", size);
        }
        else if (strncmp(uppercase_line, "SET_BIT", 7) == 0) {
            int n;
            sscanf(line + 8, "%d", &n);
            set_bit(n);
            printf("BITMAP: Set bit %d\n", n);
        }
        else if (strncmp(uppercase_line, "CLEAR_BIT", 9) == 0) {
            int n;
            sscanf(line + 10, "%d", &n);
            clear_bit(n);
            printf("BITMAP: Cleared bit %d\n", n);
        }
        else if (strncmp(uppercase_line, "TEST_BIT", 8) == 0) {
            int n;
            sscanf(line + 9, "%d", &n);
            int result = test_bit(n);
            if (result == -1)
                printf("BITMAP: Error - Invalid bit index %d\n", n);
            else
                printf("BITMAP: Bit %d is %s\n", n, result ? "set" : "clear");
        }
        else if (strncmp(uppercase_line, "FIND_FIRST_ZERO_BIT", 19) == 0) {
            int result = find_first_zero_bit();
            if (result == -1)
                printf("BITMAP: No zero bits found\n");
            else
                printf("BITMAP: First zero bit at position: %d\n", result);
        }
        else if (strncmp(uppercase_line, "FIND_NEXT_SET_BIT", 17) == 0) {
            int start;
            sscanf(line + 18, "%d", &start);
            int result = find_next_set_bit(start);
            if (result == -1)
                printf("BITMAP: No set bit found after position %d\n", start);
            else
                printf("BITMAP: Next set bit after %d is at position: %d\n", start, result);
        }
        else if (strncmp(uppercase_line, "PRINT_BITMAP", 12) == 0) {
            printf("BITMAP: ");
            print_bitmap();
            printf("\n");
        }
        else if (strncmp(uppercase_line, "IS_BITMAP_EMPTY", 15) == 0) {
            printf("BITMAP: Bitmap empty: %s\n", is_bitmap_empty() ? "Yes" : "No");
        }
        else if (strncmp(uppercase_line, "BITMAP_SIZE", 11) == 0) {
            printf("BITMAP: Bitmap size: %d bits\n", bitmap_size());
        }
        else {
            printf("UNKNOWN: %s\n", line);
        }
    }

    fclose(file);    
    return 0;
}