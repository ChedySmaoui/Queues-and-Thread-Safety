/*
 * Queue.h
 *
 * Module interface for a generic fixed-size Queue implementation.
 *
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdbool.h>
#include <stdlib.h>

#define ZERO 0
#define ONE 1
#define TWO 2
#define THREE 3
#define FOUR 4

typedef struct Queue Queue;

/* You should define your struct Queue here */
struct Queue {
    int front, rear, current_size, max_size;
    void* array;
};

/*
 * Creates a new Queue for at most max_size void* elements.
 * Returns a pointer to a new Queue on success and NULL on failure.
 */
Queue* new_Queue(int max_size);

/*
 * Enqueues the given void* element at the back of this Queue.
 * Returns true on success and false on enq failure when element is NULL or queue is full.
 */
bool Queue_enq(Queue* this, void* element);

/*
 * Dequeues an element from the front of this Queue.
 * Returns dequeued void* element on success or NULL if queue is empty.
 */
void* Queue_deq(Queue* this);

/*
 * Returns the number of elements currently in this Queue.
 */
int Queue_size(Queue* this);

/*
 * Returns true if this Queue is empty, false otherwise.
 */
bool Queue_isEmpty(Queue* this);

/*
 * Clears this Queue returning it to an empty state.
 */
void Queue_clear(Queue* this);

/*
 * Destroys this Queue by freeing the memory used by the Queue.
 */
void Queue_destroy(Queue* this);

#endif /* QUEUE_H_ */
