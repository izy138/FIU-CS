# Student Information
- Full Name: Isabella Correa
- PID: 6043518
- Section: U01
- FIU Email: icorr014@fiu.edu

# Homework: Number Processing Pipeline

[Brief description of your implementation approach]
This implementation uses a three stage pipeline using the producer and consumer pattern.
The Generator thread uses 3 producers, which reads numbers from the input and adds them to buffer 1. The Processor threads uses 3 consumers/producers and reads buffer 1, squares their value, and adds them to buffer 2.
The Writer thread uses 1 consumer and uses buffer 2 to write the values in order.
Each buffer has a max capacity of 10 slots, and uses semaphores to track which slots are empty or full. The mutex is used to prevent deadlocks while accessing the buffers.


[Describe how you implemented the synchronization for buffer1 and buffer2]
-Each buffer uses:
sem_t empty to keep track of empty slots, with a buffer capacity of 10.
sem_t full to keep track of filled slots with data, and the buffer is initialized to 0.
pthread_mutex_t is the lock that makes sure only one thread can access the buffer at a time.

-For the producer thread:
sem_wait(&empty) is used to block if the buffer is full, if not it decrements the empty count.
pthread_mutex_lock(&mutex) is used to enter the critical method to insert data.
Data is inserted into the buffer and incremented in using circular indexing.
pthread_mutex_unlock(&mutex) is then used to exit the critical method.
sem_post(&full) is used to signal that the full slot is available to use.

- For the consumer thread:
sem_wait(&full) is used to block if the buffer is empty, if not it decrements the full count.
pthread_mutex_lock(&mutex) is used to enter the critical method to insert data.
Data is extracted from the buffer and incremented out using circular indexing.
pthread_mutex_unlock(&mutex) is then used to exit the critical method.
sem_post(&full) is used to signal that the empty slot is available to use.

-Buffer 1 takes the input to the processor and Buffer 2 takes the data from the processor to the writer.


[Explain your use of semaphores (empty/full) and mutex locks]

-Semaphores are used to count and keep track of the buffer states, wihtout requiring threads to wait.
empty semaphores prevents the producers from overfilling the buffer, and full states prevents consumers from reading the buffer when its empty. 
Semaphores are used to handle synchronization for the buffer states.

-Mutex is used to make sure only one thread can execute the critical section of the code, where data is updated in and out of the buffers. 
Mutex prevents race conditioning where multiple threads can not access buffers at the same time. 


[Discuss how you prevented race conditions and deadlocks]

-Race conditions are prevented by making sure all buffer state modification is protected by mutex. All data is accessed using mutex protection.
Semaphores wait and post to prevent race conditions for empty or full buffers.
Circular indexing is used with (index + 1) % BUFFER_SIZE to make sure the indexes wrap without overflowing.

-Deadlocks are prevented by using mutex locks. However you have to make sure you acquire the semaphore before the mutex. 
sem_wait must be used before mutex_lock, and then mutex_unlock is done before sem_post.
Deadlock can occur if mutex_lock is done before sem_wait. 
