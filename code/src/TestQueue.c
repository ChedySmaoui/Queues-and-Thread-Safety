/*
 * TestQueue.c
 *
 * Very simple unit test file for Queue functionality.
 *
 */

#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "myassert.h"
#include "Queue.h"

#include <stdbool.h>

#define DEFAULT_MAX_QUEUE_SIZE 20

/*
 * The queue to use during tests
 */
static Queue *queue;

/*
 * The number of tests that succeeded
 */
static int success_count = 0;

/*
 * The total number of tests run
 */
static int total_count = 0;


/*
 * Setup function to run prior to each test
 */
void setup(){
    queue = new_Queue(DEFAULT_MAX_QUEUE_SIZE);
    total_count++;
}

/*
 * Teardown function to run after each test
 */
void teardown(){
    Queue_destroy(queue);
}

/*
 * This function is called multiple times from main for each user-defined test function
 */
void runTest(int (*testFunction)()) {
    setup();

    if (testFunction()) success_count++;

    teardown();
}


/*
 * Two sample user-defined tests included below.
 * You will have to write many more tests.
 *
 * Each test function should return TEST_SUCCESS at the end.
 * Test functions can contain more than one assertion.
 *
 * If any assertion fails, the function name and line number
 * will be printed and the test will return a failure value.
 *
 * You will have to call runTest on your test functions in main below.
 */

/*
 * Checks that the Queue constructor returns a non-NULL pointer.
 */
int newQueueIsNotNull() {
    assert(queue != NULL);
    return TEST_SUCCESS;
}

/*
 * Checks that the size of an empty queue is 0.
 */
int newQueueSizeZero() {
    assert(Queue_size(queue) == 0);
    return TEST_SUCCESS;
}

/*
 * Write more of your own test functions below (such as enqOneElement, enqAndDeqOneElement, ...)
 * to help you verify correctness of your Queue
 */

/**
 * Checks that enqueuing an element works as expected.
*/
int enqOneElement() {
    int x = 10;
    void *pointer = &x;
    /** Ensures that the enqueuing operation is successful.*/
    assert(Queue_enq(queue, pointer)  == true);
    return TEST_SUCCESS;
}

/**
 * Checks that enqueueing and dequeuing an element works.
*/
int enqAndDeqOneElement() {
    /** Enqueues a single void pointer (the pointer points to the integer value x = 10).*/
    int x = 10;
    void *pointer = &x;
    Queue_enq(queue, pointer);

    /** Ensures the size of the queue increases.*/
    assert(Queue_size(queue) == 1);

    /** Dequeues, casts void pointer back to integer, and check that it is equal to the enqueued element.*/
    int response = *((int*)Queue_deq(queue));
    assert(response == x);

    /** Ensures that the queue is empty after dequeuing its single element.*/
    assert(Queue_isEmpty(queue) == true);
    return TEST_SUCCESS;
}

/**
 * Checks that isEmpty returns true as expected on an empty Queue.
*/
int isEmptyWorks() {
    assert(Queue_isEmpty(queue) == true);
    assert(Queue_size(queue) == 0);
    return TEST_SUCCESS;
}

/**
 * Checks that isEmpty returns false as expected on a non-empty Queue.
*/
int isEmptyReturnsFalseAfterEnq() {
    /** Enqueues a single void pointer (the pointer points to the integer value a = 2).*/
    int a = 2;
    void* pointer_a = &a;
    assert(Queue_enq(queue, pointer_a) == true);

    /** Ensures that isEmpty returns false.*/
    assert(Queue_isEmpty(queue) == false);
    return TEST_SUCCESS;
}

/**
 * Checks that clearing the Queue works as expected.
*/
int clearWorks() {
    /** Three integer variables to be enqueued*/
    int a, b, c = 10;
    void *pointer_a = &a;
    void *pointer_b = &b;
    void *pointer_c = &c;
    Queue_enq(queue, pointer_a);
    Queue_enq(queue, pointer_b);
    Queue_enq(queue, pointer_c);

    assert(Queue_size(queue) == 3);
    Queue_clear(queue);
    assert(Queue_size(queue) == 0);

    assert(Queue_deq(queue) == NULL);
    assert(Queue_deq(queue) == NULL);
    assert(Queue_deq(queue) == NULL);
    return TEST_SUCCESS;
}

/**
 * Checks that attempting to enqueue an element to a already full queue returns false as expected.
*/
int enqWhenFull() {
    /** Creates a new queue of size 4.*/
    int max_size = 4;
    Queue *queue2 = new_Queue(max_size);

    /** Enqueue 4 elements to the queue.*/
    int a = 7;
    int b = 4;
    int c = 2;
    int d = 5;
    void *pointer_a = &a;
    void *pointer_b = &b;
    void *pointer_c = &c;
    void *pointer_d = &d;
    assert(Queue_enq(queue2, pointer_a) == true);
    assert(Queue_enq(queue2, pointer_b) == true);
    assert(Queue_enq(queue2, pointer_c) == true);
    assert(Queue_enq(queue2, pointer_d) == true);

    /** The Queue should now be full.*/
    assert(Queue_size(queue2) == max_size);

    /** Tries to enqueue another element which should result in Queue_enq returning false since the queue is full.*/
    int e = 3;
    void *pointer_e = &e;
    assert(Queue_enq(queue2, pointer_e) == false);

    /** Dequeueing should return a = 7 which is the value at the front of the Queue. */
    int response = *((int*)Queue_deq(queue2));
    assert(response == a);
    Queue_destroy(queue2);
    return TEST_SUCCESS;
}

/**
 * Checks that enqueing a null element returns false.
*/
int enqNull() {
    void *element = NULL;
    assert(Queue_enq(queue, element) == false);
    return TEST_SUCCESS;
}

/**
 * Checks that dequeuing returns NULL when the Queue is empty.
*/
int deqWhenEmpty() {
    /** Verify that the queue is empty.*/
    assert(Queue_isEmpty(queue) == true);

    /** Checks that deuqeuing on an empty queue returns NULL.*/
    assert(Queue_deq(queue) == NULL);
    return TEST_SUCCESS;
}

/**
 * Checks that enqueueing and dequeuing several elements works.
*/
int enqAndDeqTwoElements() {
    /** Enqueues a single void pointer (the pointer points to the integer value x = 10).*/
    int a = 7; //int
    void *pointer_a = &a;
    Queue_enq(queue, pointer_a);

    /** Enqueues another element.*/
    int b = 13;
    void *pointer_b = &b;
    Queue_enq(queue, pointer_b);

    /** Dequeues the element at the front of the queue which should be a = 10.*/
    int response1 = *((int*)Queue_deq(queue));
    assert(response1 == a);

    /** Dequeues the element at the front of the queue which should now be b = 13.*/
    int response2 = *((int*)Queue_deq(queue));
    assert(response2 == b);

    /** Ensures that the queue is empty after dequeuing all of its elements.*/
    assert(Queue_isEmpty(queue) == true);
    return TEST_SUCCESS;
}

/**
 * Checks that creating a Queue of negative maximum size returns NULL.
*/
int negativeSizedQueue() {
    int negative_max_size = -1;
    Queue *queue  = new_Queue(negative_max_size);
    assert(queue == NULL);
    return TEST_SUCCESS;
}

/**
 * Checks that creating a Queue of maximum size equal to 0 returns NULL.
*/
int zeroSizedQueue () {
    int zero_max_size = 0;
    Queue *queue = new_Queue(zero_max_size);
    assert(queue == NULL);
    return TEST_SUCCESS;
}

/**
 * Checks that isEmpty works after dequeuing all the elements previously added to the queue.
*/
int isEmptyWorksAfterDequeue() {
    /** Ensures that the queue is initially empty.*/
    assert(Queue_isEmpty(queue) == true);

    /** Adds an element to the queue making it non-empty.*/
    int a = 10;
    void *pointer_a = &a;
    assert(Queue_enq(queue, pointer_a) == true);

    /** Ensures that the queue is now non-empty.*/
    assert(Queue_isEmpty(queue) == false);

    /** Dequeue the element from the new making it empty again.*/
    assert(*((int*)Queue_deq(queue)) == a);

    /** Ensures isEmpty works after emptying the Queue again.*/
    assert(Queue_isEmpty(queue) == true);
    return TEST_SUCCESS;
}

/**
 * Checks that enqueueing an element increases the size of the Queue.
*/
int sizeIncreasesAfterEnq() {
    /** Integer element 3 to enqueue.*/
    int element = 3;

    /** Void pointer to the element to enqueue.*/
    void *pointer_element = &element;

    /** Ensures that the size of the Queue is initially zero.*/
    assert(Queue_size(queue) == ZERO);

    /** Enqueues the element and checks that the operation was successfull.*/
    assert(Queue_enq(queue, pointer_element) == true);

    /** Checks that the size increased by one.*/
    assert(Queue_size(queue) == ONE);
    return TEST_SUCCESS;
}

/**
 * Checks that dequeueing an element decreases the size of the Queue.
*/
int sizeDecreasesAfterDeq() {
    /** Integer element 3 to enqueue and dequeue..*/
    int element = 3;

    /** Void pointer to the element to enqueue and dequeue..*/
    void *pointer_element = &element;

    /** Enqueues the element and checks that the operation was successfull.*/
    assert(Queue_enq(queue, pointer_element) == true);

    /** Checks that the size is one.*/
    assert(Queue_size(queue) == ONE);

    /** Dequeues the lement from the Queue and ensures that it corresponds the the element enqueued earlier.*/
    assert(*(int*)Queue_deq(queue) == element);

    /** Checks that the size decreased by one.*/
    assert(Queue_size(queue) == ZERO);
    return TEST_SUCCESS;
}

/**
 * Checks that dequeueing returns NULL after all the elements of the Queue have been cleared.
*/
int deqAfterClear() {
    /** Enqueue 20 elements to the BlockingQueue.*/
    int a = 1;
    void *pointer_a = &a;
    for (int i = 0; i < DEFAULT_MAX_QUEUE_SIZE; i++) {
        assert(Queue_enq(queue, pointer_a) == true);
    }

    /** Clears the queue.*/
    Queue_clear(queue);

    /** Checks that Dequeueing returns NULL*/
    assert(Queue_deq(queue) == NULL);
    return TEST_SUCCESS;
}

/**
 * Checks that enqueuing an element works as expected. after clearing the Queue.
*/
int enqAfterClear() {
    /** Enqueue 20 elements to the BlockingQueue.*/
    int a = 1;
    void *pointer_a = &a;
    for (int i = 0; i < DEFAULT_MAX_QUEUE_SIZE; i++) {
        assert(Queue_enq(queue, pointer_a) == true);
    }

    /** Clears the queue.*/
    Queue_clear(queue);

    int b = 2;
    void* pointer_b = &b;
    assert(Queue_enq(queue, pointer_b) == true);
    assert(*((int*)Queue_deq(queue)) == b);
    return TEST_SUCCESS;
}

/**
 * Checks that the Queue accepts elements of integer type.
*/
int enqAndDeqInteger() {
    /** Integer element 3 to enqueue and dequeue..*/
    int element = 3;

    /** Void pointer to the element to enqueue and dequeue..*/
    void *pointer_element = &element;

    /** Enqueues the element and checks that the operation was successfull.*/
    assert(Queue_enq(queue, pointer_element) == true);

    /** Dequeues the element from the Queue and ensures that it corresponds the the element enqueued earlier.*/
    assert(*(int*)Queue_deq(queue) == element);

    return TEST_SUCCESS;
}

/**
 * Checks that the Queue accepts element of double type.
*/
int enqAndDeqDouble() {
    /** Double element 10.2 to enqueue and dequeue..*/
    double element = 10.2;

    /** Void pointer to the element to enqueue and dequeue..*/
    void *pointer_element = &element;

    /** Enqueues the element and checks that the operation was successfull.*/
    assert(Queue_enq(queue, pointer_element) == true);

    /** Dequeues the element from the Queue and ensures that it corresponds the the element enqueued earlier.*/
    assert(*(double*)Queue_deq(queue) == element);

    return TEST_SUCCESS;
}

/**
 * Checks that the Queue accepts element of character type.
*/
int enqAndDeqCharacter() {
    /** Char element "c" to enqueue and dequeue..*/
    char element = 'c';

    /** Void pointer to the element to enqueue and dequeue..*/
    void *pointer_element = &element;

    /** Enqueues the element and checks that the operation was successfull.*/
    assert(Queue_enq(queue, pointer_element) == true);

    /** Dequeues the element from the Queue and ensures that it corresponds the the element enqueued earlier.*/
    assert(*(char*)Queue_deq(queue) == element);

    return TEST_SUCCESS;
}

/**
 * Checks that the Queue accepts elements of char arry type (i.e., C strings).
*/
int enqAndDeqString() {
    /** Char element "c" to enqueue and dequeue..*/
    const char *element = "I love kiwis!";

    /** Enqueues the element and checks that the operation was successfull.*/
    assert(Queue_enq(queue, (void*)element) == true);

    /** Dequeues the element from the Queue and ensures that it corresponds the the element enqueued earlier.*/
    const char *dequeued_element = (char*)Queue_deq(queue);
    assert(strcmp(element, dequeued_element) == ZERO);

    return TEST_SUCCESS;
}

/*
 * Main function for the Queue tests which will run each user-defined test in turn.
 */

int main() {
    runTest(newQueueIsNotNull);
    runTest(newQueueSizeZero);
    /*
     * you will have to call runTest on all your test functions above, such as
     *
     * runTest(enqOneElement);
     * runTest(enqAndDeqOneElement);
     *
     */
    runTest(enqOneElement);

    runTest(enqAndDeqOneElement);

    runTest(isEmptyWorks);

    runTest(clearWorks);

    runTest(enqWhenFull);

    runTest(enqNull);

    runTest(deqWhenEmpty);

    runTest(enqAndDeqTwoElements);

    runTest(negativeSizedQueue);

    runTest(zeroSizedQueue);

    runTest(isEmptyReturnsFalseAfterEnq);

    runTest(isEmptyWorksAfterDequeue);

    runTest(sizeIncreasesAfterEnq);

    runTest(sizeDecreasesAfterDeq);

    runTest(deqAfterClear);

    runTest(enqAfterClear);

    runTest(enqAndDeqInteger);

    runTest(enqAndDeqDouble);

    runTest(enqAndDeqCharacter);

    runTest(enqAndDeqString);

    printf("Queue Tests complete: %d / %d tests successful.\n----------------\n", success_count, total_count);

}
