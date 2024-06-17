/*
 * Queue.c
 *
 * Fixed-size generic array-based Queue implementation.
 *
 */

#include <stddef.h>

#include "Queue.h"

/*
 * The functions below all return default values and don't work.
 * You will need to provide a correct implementation of the Queue module interface as documented in Queue.h.
 */
#include <stdlib.h>

Queue *new_Queue(int max_size) {

    /** Checks that the given max_size is a valid maximum capacity.*/
    if (max_size <= ZERO) {
        /** If max_size is not valid, return NULL indicating failure to create the queue.*/
        return NULL;
    }

    /** Allocate memory for the Queue structure.*/
    Queue *this = malloc(sizeof(Queue));

    /**
     * Initialize the Queue structure fields...
     * 
    */

    /** Sets the maximum capacity of the queue. */
    this->max_size = max_size;

    /** Initializes the current size of the queue and the index of the front element to 0.*/
    this-> front = this->current_size = ZERO;

    /** Sets the rear to the last index.*/
    this->rear = max_size - ONE;

    /**
     * Allocate memory for the queue elements.
     * Dynamically allocates memory for an array of pointers
    */
    this->array = malloc(this->max_size * sizeof(void*));

    /** Return the pointer to the newly created Queue.*/
    return this;
}

bool Queue_enq(Queue* this, void* element) {

    /** Check if the Queue is full or the element is NULL.*/
    if ((this->current_size == this->max_size) || (element == NULL)) {
        /** Return false to indicate failure to enqueue.*/
        return false;
    }
    /**
     * Otherwise,
     * Increases the  current rear index by 1 to move the rear pointer to the next available position in the array.
     * 
    */
    else {
        /** Calculate the new rear position and wrap around if necessary.*/
        this->rear = (this-> rear + ONE) % this->max_size;

        /** Insert the element at the rear of the queue.*/
        ((void**)this->array)[this->rear] = element;

        /** Increment the current size of the queue.*/
        this->current_size = this->current_size + ONE;

        /** Return true to indicate success.*/
        return true;
    }
}

void* Queue_deq(Queue* this) {

    /** Check if the queue is empty.*/
    if (Queue_isEmpty(this)) {
        /** Return NULL to indicate the queue is empty and no element can be dequeued.*/
        return NULL;
    }
    /** Retrieve the element at the front of the queue.*/
    void* item = ((void**)this->array)[this->front];

    /** Move the front forward and wrap around if necessary.*/
    this->front = (this->front + ONE) % this->max_size;

    /** Decrement the current size of the queue.*/
    this->current_size = this->current_size - ONE;

    /** Return the dequeued element.*/
    return item;
}

int Queue_size(Queue* this) {
    /** Return the current size of the Queue.*/
    return this->current_size;
}

bool Queue_isEmpty(Queue* this) {
    /** Return true if the current size is 0, indicating the queue is empty.*/
    return (this->current_size == ZERO);
}

void Queue_clear(Queue* this) {
    /** Reset the size of the queue to 0.*/
    this->current_size = ZERO;
    /** Reset the front indicator to 0.*/
    this->front = ZERO;
    /** Reset the rear indicator to the last index.*/
    this->rear = this->max_size - ONE;
}

void Queue_destroy(Queue* this) {
    /** Free the array of elements.*/
    free(this->array);
    /** Free the Queue structure itself.*/
    free(this);
}
