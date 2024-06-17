/*
 * BlockingQueue.c
 *
 * Fixed-size generic array-based BlockingQueue implementation.
 *
 */

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

#include "BlockingQueue.h"

/*
 * The functions below all return default values and don't work.
 * You will need to provide a correct implementation of the BlockingQueue module interface as documented in BlockingQueue.h.
 */

/**
 * Private function to cleanup and exit the program in case of POSIX functions errors.
 * 
 * The function destroys the BlockingQueue, prints the error message and terminates the program with EXIT_FAILURE status.
*/
static void cleanup_exit(BlockingQueue* this, char *error_mesg) {
    /** Cleanup the BlockingQueue ressources.*/
    BlockingQueue_destroy(this);

    /** Prints out the provided error message.*/
    perror(error_mesg);

    /** Terminates the program with EXIT_FAILURE status.*/
    exit(EXIT_FAILURE);
}


BlockingQueue *new_BlockingQueue(int max_size) {

    /** Allocate memory for the BlockingQueue structure.*/
    BlockingQueue *this = malloc(sizeof(BlockingQueue));
    if (this == NULL) {
        /** If the initialized BlockingQueue is NULL, indicates to the user that that memory allocation failed.*/
        perror("Error: Failed to allocate memory for BlockingQueue");
        return this;
    }

    /** Sets the maximum size of the Queue.*/
    this->max_size = max_size;

    /** Initializes the count of initialized variables (excluding max_size) to 0.*/
    this->initialized = ZERO;

    /**
     * Initializes the internal Queue struct.
     * 
     * Increment the initialization counter
    */
    this->initialized += ONE;
    this->queue = new_Queue(max_size);

    if (this->queue == NULL) {
        /** Free the BlockingQueue structure if queue initialization fails.*/
        free(this);
        return NULL;
    }

    /**
     * Initializes the mutex ensuring thread-safety.
     * 
     * Increment for mutex initialization
     * Cleanup and terminates if the mutex initialization failed.
    */
    this->initialized += ONE;
    if (pthread_mutex_init(&this->mutex, NULL)) { cleanup_exit(this, "Error: failed to initialize mutex.");}

    /**
     * Initializes the semaphore indicating the number of full slots in the queue to 0 full slots.
     * 
     * Increments the number of initialized variables to 3.
     * Cleanup and terminates if the semaphore initialization failed.
    */
    this->initialized += ONE;
    if (sem_init(&this->full_slots, ZERO, ZERO)) { cleanup_exit(this, "Error: Failed to initialize full_slots semaphore");}

    /**
     * Initializes the semaphore indicating the number of empty slots in the queue to max_size empty slots.
     * 
     * Increments the number of initialized variables to 4.
     * Cleanup and terminates if the semaphore initialization failed.
    */
    this->initialized += ONE;
    if (sem_init(&this->empty_slots, ZERO, max_size)) { cleanup_exit(this, "Error: Failed to initialize empty_slots semaphore");}

    /** Return the initialized BlockingQueue.*/
    return this;
}

bool BlockingQueue_enq(BlockingQueue* this, void* element) {

    /** Waits until there is at least one empty slot in the blocking queue.*/
    if (sem_wait(&this->empty_slots)) { cleanup_exit(this, "Error: sem_wait() failed for empty_slots semaphore");}

    /**
     * When space becomes available...
    */

    /** Locks the mutex to ensure thread safety.*/
    if (pthread_mutex_lock(&this->mutex)) { cleanup_exit(this, "Error: pthread_mutex_lock() failed before enqueueing");}

    /** Attempt to enqueue the element at the rear of the queue.*/
    bool success = Queue_enq(this->queue, element);

    /** Unlocks the mutex.*/
    if (pthread_mutex_unlock(&this->mutex)) { cleanup_exit(this, "Error: pthread_mutex_unlock() failed after enqueueing");}

    /** Signals that there is one more full slot in the blocking queue.*/
    if (sem_post(&this->full_slots)) { cleanup_exit(this, "Error: sem_post() failed for full_slots semaphore");}

    /** Return the result of the enqueue operation.*/
    return success;
}

void* BlockingQueue_deq(BlockingQueue* this) {

    /** Waits until there is at least one full slot in the blocking queue.*/
    if (sem_wait(&this->full_slots)) { cleanup_exit(this, "Error: sem_wait() failed for full_slots semaphore");}

    /**
     * When there are elements in the queue...
    */

    /** Locks the mutex to ensure thread safety.*/
    if (pthread_mutex_lock(&this->mutex)) { cleanup_exit(this, "Error: pthread_mutex_lock() failed before dequeueing");}

    /** Dequeues the front element*/
    void *element = Queue_deq(this->queue);

    /** Unlocks the mutex.*/
    if (pthread_mutex_unlock(&this->mutex)) { cleanup_exit(this, "Error: pthread_mutex_unlock() failed after dequeuing");}

    /** Signals that there is one more empty slot in the blocking queue.*/
    if (sem_post(&(this->empty_slots))) { cleanup_exit(this, "Error: sem_post() failed for empty_slots semaphore");}

    /** Return the dequeued element.*/
    return element;
}

int BlockingQueue_size(BlockingQueue* this) {
    /** Locks the mutex to ensure thread safety.*/
    if (pthread_mutex_lock(&this->mutex)) { cleanup_exit(this, "Error: pthread_mutex_lock() failed before getting the current size");}

    /** Retrieve the current size of the internal Queue.*/
    int size = Queue_size(this->queue);

    /** Unlocks the mutex and return the size of the Queue.*/
    if (pthread_mutex_unlock(&this->mutex)) { cleanup_exit(this, "Error: pthread_mutex_unlock() failed after getting the current size");}
    return size;
}

bool BlockingQueue_isEmpty(BlockingQueue* this) {
    /** Locks the mutex to ensure thread safety.*/
    if (pthread_mutex_lock(&this->mutex)) { cleanup_exit(this, "Error: pthread_mutex_lock() failed during isEmpty()");}

    /** Check if the internal Queue is empty.*/
    bool empty = Queue_isEmpty(this->queue);

    /** Unlocks the mutex and return true if the queue is empty, false otherwise.*/
    if (pthread_mutex_unlock(&this->mutex)) { cleanup_exit(this, "Error: pthread_mutex_unlock() failed during isEmpty()");}

    /** Return true if empty, false otherwise.*/
    return empty;
}

/**
 * Important: This function is NOT thread safe, use with CAUTION.
 * 
 * While it resets the value of the two semaphores used for the BlockingQueue allowing for new threads to be created,
 * if an enqueuing or deqeuing thread is already waiting on the semaphores, calling this function will result in having two threads concurrently waiting on
 * the same semaphore and modifying its value. This can cause a segmentation fault.
 * 
 * Before clearing a BlockingQueue, ensure that no thread to enqueue elements are still waiting for slots to be emptied.
*/
void BlockingQueue_clear(BlockingQueue* this) {

    /** Locks the mutex to ensure thread safety.*/
    if (pthread_mutex_lock(&this->mutex)) { cleanup_exit(this, "Error: pthread_mutex_lock() failed during isEmpty()");}

    /** Clear the internal Queue, resetting the current size and the front and rear indexes.*/
    Queue_clear(this->queue);

    /** When initialized, holds the current number of empty slots in the blocking queue.*/
    int value_empty_slots;

    /** Grabs the value of the empty_slots semaphore and saves it in the value_empty_slots int variable.*/
    if (sem_getvalue(&this->empty_slots, &value_empty_slots)) { cleanup_exit(this, "Error: sem_getvalue(&this->empty_slots, &value_empty_slots) failed in BlockingQueue_clear");}

    /**
     * This while loop resets the value of the full_slots semaphore to zero.
     * This is important since after being cleared, the Blocking Queue should have zero slots occupied.
    */
    while (sem_trywait(&this->full_slots) == 0); // sem_trywait returns -1 when the value of full_slots is 0.

    /**
     * This for loop resets the value of the empty_slots semaphore to the maximum size.
     * This is important since after being cleared, the Blocking Queue should have max_size free slots.
    */
    for (int i = value_empty_slots; i < ((this->max_size) + 1); i++) {
        if(sem_post(&this->empty_slots)) { cleanup_exit(this, "Error: sem_post(&this->empty_slots) failed in BlockingQueue_clear");}
    }

    /** Unlocks the mutex and return true if the queue is empty, false otherwise.*/
    if (pthread_mutex_unlock(&this->mutex)) { cleanup_exit(this, "Error: pthread_mutex_unlock() failed during isEmpty()");}
}

void BlockingQueue_destroy(BlockingQueue* this) {
    /** Destroy the internal Queue if initialized.*/
    if (this->initialized >= ONE) { Queue_destroy(this->queue);}

    /** Destroy the mutex if initialized.*/
    if (this->initialized >= TWO) { pthread_mutex_destroy(&this->mutex);}

    /** Destroy the full_slots semaphore if initialized.*/
    if (this->initialized >= THREE) { sem_destroy(&this->full_slots);}

    /** Destroy the empty_slots semaphore if initialized.*/
    if (this->initialized >= FOUR) { sem_destroy(&this->empty_slots);}

    /** Free the memory allocated for the BlockingQueue.*/
    free(this);
}
