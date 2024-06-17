/*
 * TestBlockingQueue.c
 *
 * Very simple unit test file for BlockingQueue functionality.
 *
 */

#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>

#include "BlockingQueue.h"
#include "myassert.h"


#define DEFAULT_MAX_QUEUE_SIZE 20

/*
 * The queue to use during tests
 */
static BlockingQueue *queue;

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
    queue = new_BlockingQueue(DEFAULT_MAX_QUEUE_SIZE);
    total_count++;
}

/*
 * Teardown function to run after each test
 */
void teardown(){
    BlockingQueue_destroy(queue);
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
 * Checks that the BlockingQueue constructor returns a non-NULL pointer.
 */
int newQueueIsNotNull() {
    assert(queue != NULL);
    return TEST_SUCCESS;
}

/*
 * Checks that the size of an empty blocking queue is 0.
 */
int newQueueSizeZero() {
    assert(BlockingQueue_size(queue) == 0);
    return TEST_SUCCESS;
}

/*
 * Write more of your own test functions below (such as enqOneElement, enqAndDeqOneElement, ...)
 * to help you verify correctness of your BlockingQueue.
 */


/**
 * Threads...
 * 
 * 
 * 
*/
/**
 * Thread to enqueue an element at the rear the blocking queue.
*/
void* enqueueThread(void* element) {
    bool result = BlockingQueue_enq(queue, element);
    pthread_exit((void*)(__intptr_t)result);
}

/**
 * Thread to dequeue an element at the front the blocking queue.
*/
void* dequeueThread(void* queue) {
    pthread_exit(BlockingQueue_deq((BlockingQueue*)queue));
}

/**
 * Thread to clear the blocking queue.
*/
void* clearThread(void* queue) {
    BlockingQueue_clear((BlockingQueue*)queue);
    pthread_exit(NULL);
}

/**
 * Thread to obtain the current size of the blocking queue.
*/
void* sizeThread(void* queue) {
    int result = BlockingQueue_size((BlockingQueue*)queue);
    pthread_exit((void*)(__intptr_t)result);
}

/**
 * Thread for BlockingQueue_isEmpty.
*/
void* isEmptyThread(void* queue) {
    bool result = BlockingQueue_isEmpty((BlockingQueue*)queue);
    pthread_exit((void*)(__intptr_t)result);
}

/**
 * Unit Tests...
 * 
 * 
 * 
*/

/**
 * Checks that creating a BlockingQueue of negative maximum size returns NULL.
*/
int negativeSizedQueue() {
    int negative_max_size = -1;
    BlockingQueue *queue  = new_BlockingQueue(negative_max_size);
    assert(queue == NULL);
    return TEST_SUCCESS;
}

/**
 * Checks that creating a BlockingQueue of maximu size equal to 0 returns NULL.
*/
int zeroSizedQueue () {
    int zero_max_size = 0;
    BlockingQueue *queue = new_BlockingQueue(zero_max_size);
    assert(queue == NULL);
    return TEST_SUCCESS;
}

/**
 * Checks that enqueuing an element works.
*/
int enqOneElement() {
    int x = 10;
    void *pointer = &x;
    /** Ensures that the enqueuing operation is successful.*/
    assert(BlockingQueue_enq(queue, pointer)  == true);

    /** Ensures */
    assert(BlockingQueue_size(queue) == 1);
    return TEST_SUCCESS;
}

/**
 * Checks that enqueueing and dequeuing an element works.
*/
int enqAndDeqOneElement() {
    /** Enqueues a single void pointer (the pointer points to the integer value x = 10).*/
    int x = 10;
    void *pointer = &x;
    BlockingQueue_enq(queue, pointer);

    /** Ensures the size of the queue increases.*/
    assert(BlockingQueue_size(queue) == 1);

    /** Dequeues, casts void pointer back to integer, and check that it is equal to the enqueued element.*/
    int response = *((int*)BlockingQueue_deq(queue));
    assert(response == x);

    /** Ensures that the queue is empty after dequeuing its single element.*/
    assert(BlockingQueue_isEmpty(queue) == true);
    return TEST_SUCCESS;
}

/**
 * Checks that isEmpty works on an empty Queue.
*/
int isEmptyWorks() {
    assert(BlockingQueue_isEmpty(queue) == true);
    assert(BlockingQueue_size(queue) == 0);
    return TEST_SUCCESS;
}

/**
 * Checks that isEmpty works after dequeuing all the elements previously added to the queue.
*/
int isEmptyWorksAfterDequeue() {
    /** Ensures that the queue is initially empty.*/
    assert(BlockingQueue_isEmpty(queue) == true);

    /** Adds an element to the queue making it non-empty.*/
    int a = 10;
    void *pointer_a = &a;
    assert(BlockingQueue_enq(queue, pointer_a) == true);

    /** Ensures that the queue is now non-empty.*/
    assert(BlockingQueue_isEmpty(queue) == false);

    /** Dequeue the element from the new making it empty again.*/
    assert(*((int*)BlockingQueue_deq(queue)) == a);

    /** Ensures isEmpty works after emptying the Queue again.*/
    assert(BlockingQueue_isEmpty(queue) == true);
    return TEST_SUCCESS;
}

/**
 * Checks that clearing the Queue works.
*/
int clearWorks() {
    /** Three integer variables to be enqueued*/
    int a, b, c = 10;
    void *pointer_a = &a;
    void *pointer_b = &b;
    void *pointer_c = &c;
    BlockingQueue_enq(queue, pointer_a);
    BlockingQueue_enq(queue, pointer_b);
    BlockingQueue_enq(queue, pointer_c);

    assert(BlockingQueue_size(queue) == 3);
    BlockingQueue_clear(queue);
    assert(BlockingQueue_size(queue) == 0);

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
    assert(BlockingQueue_size(queue) == ZERO);

    /** Enqueues the element and checks that the operation was successfull.*/
    assert(BlockingQueue_enq(queue, pointer_element) == true);

    /** Checks that the size increased by one.*/
    assert(BlockingQueue_size(queue) == ONE);
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
    assert(BlockingQueue_enq(queue, pointer_element) == true);

    /** Checks that the size is one.*/
    assert(BlockingQueue_size(queue) == ONE);

    /** Dequeues the lement from the Queue and ensures that it corresponds the the element enqueued earlier.*/
    assert(*(int*)BlockingQueue_deq(queue) == element);

    /** Checks that the size decreased by one.*/
    assert(BlockingQueue_size(queue) == ZERO);
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
    assert(BlockingQueue_enq(queue, pointer_element) == true);

    /** Dequeues the element from the Queue and ensures that it corresponds the the element enqueued earlier.*/
    assert(*(int*)BlockingQueue_deq(queue) == element);

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
    assert(BlockingQueue_enq(queue, pointer_element) == true);

    /** Dequeues the element from the Queue and ensures that it corresponds the the element enqueued earlier.*/
    assert(*(double*)BlockingQueue_deq(queue) == element);

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
    assert(BlockingQueue_enq(queue, pointer_element) == true);

    /** Dequeues the element from the Queue and ensures that it corresponds the the element enqueued earlier.*/
    assert(*(char*)BlockingQueue_deq(queue) == element);

    return TEST_SUCCESS;
}

/**
 * 
*/
int enqAndDeqString() {
    /** Char element "c" to enqueue and dequeue..*/
    const char *element = "I love kiwis!";

    /** Enqueues the element and checks that the operation was successfull.*/
    assert(BlockingQueue_enq(queue, (void*)element) == true);

    /** Dequeues the element from the Queue and ensures that it corresponds the the element enqueued earlier.*/
    const char *dequeued_element = (char*)BlockingQueue_deq(queue);
    assert(strcmp(element, dequeued_element) == ZERO);

    return TEST_SUCCESS;
}

/**
 * Checks that Dequeueing is thread safe.
 * 
 * This test changes the thread scheduling order: the first thread created tries to dequeue an element while the second tries to enqueue an element.
 * The program sleeps between the creation of the two threads to ensure that the first one is blocked and waiting for an element to be added to the BlockingQueue.
*/
int deqFirstThenEnqOneElement() {
    /** Initializes and defines an integer element to be enqueued and dequeued in/from the BlockingQueue.*/
    int a = 7;

    /** Defines a void pointer to the integer variable a to be passed in the enqueueing function.*/
    void *pointer_a = &a;

    /**
     * Defines two thread identifiers:
     * Thread 1 will be the dequeueing thread and the first one to be created.
     * Thread 2 will be the enqueueing thread and the second one to be created.
    */
    pthread_t tid1,tid2;
    /** Void pointers to threads 1 and 2 identifiers'.*/
    void *tr1, *tr2;

    /** Create a thread to dequeue an element off the empty BlockingQueue.*/
    pthread_create(&tid1, NULL, dequeueThread, queue);

    /** Make the program sleep for 1 second, the Dequeueing thread should be blocked and waiting for an element to be enqueued.*/
    sleep(1);

    /** Create a thread to enqueue an element in the BlockingQueue.*/
    pthread_create(&tid2, NULL, enqueueThread, pointer_a);

    /** Waits for the Dequeuing thread to return the dequeued element and stores it at the location pointed by the void pointer tr1.*/
    pthread_join( tid1, &tr1 );

    /**
     * Waits for the Enqueuing thread to return whether it successfully enqueued the element,
     * and stores the boolean response at the location pointed by the void pointer tr2.
    */
    pthread_join( tid2, &tr2 );

    /** Asserts that the element dequeued is the enqueued element.*/
    assert( (*((int*)tr1)) == a );

    /** Asserts that the enqueuing operation was successfull.*/
    assert( (bool)tr2 == true );

    return TEST_SUCCESS;
}

/**
 * Checks that enqueueing is thread safe.
*/
int enqThreadWhenFull() {

    /** Enqueue 20 elements to the BlockingQueue.*/
    int a = 1;
    void *pointer_a = &a;
    for (int i = ZERO; i < DEFAULT_MAX_QUEUE_SIZE; i++) {
        assert(BlockingQueue_enq(queue, pointer_a) == true);
    }

    /** Asserts that the BlockingQueue is now full.*/
    assert(BlockingQueue_size(queue) == DEFAULT_MAX_QUEUE_SIZE);

    /**
     * The Queue should now be full.
     * 
     * The following code first create a thread to enqueue two other elements in the BlockingQueue.
     * These Threads should be blocked and wait for elements to be dequeued.
    */
    int b = 5;
    void *pointer_b = &b;

    /**
     * Thread identifiers:
     * 
     * tid1: enqueueing thread's identifier.
     * tid2: dequeueing thread's identifier.
    */
    pthread_t tid1, tid2;

    /**
     * Void pointers to the threads' identifiers:
     * 
     * tr1: pointer to the enqueueing thread's identifier.
     * tr2: pointer to the dequeueing thread's identifier.
    */
    void *tr1, *tr2;

    /** Create a thread to enqueue an element in the BlockingQueue.*/
    pthread_create(&tid1, NULL, enqueueThread, pointer_b);

    /** Make the program sleep for 1 second, the Enqueueing thread should be blocked and waiting for an element to be dequeued.*/
    sleep(1);

    /** Create a thread to dequeue an element off the empty BlockingQueue.*/
    pthread_create(&tid2, NULL, dequeueThread, queue);

    /**
     * Waits for the Enqueuing thread to return whether it successfully enqueued the element,
     * and stores the boolean response at the location pointed by the void pointer tr1.
    */
    pthread_join( tid1, &tr1 );

    /** Waits for the Dequeuing thread to return the dequeued element and stores it at the location pointed by the void pointer tr2.*/
    pthread_join( tid2, &tr2 );

    /** Asserts that the enqueuing operation was successfull.*/
    assert( (bool)tr1 == true );

    return TEST_SUCCESS;
}

int manyEnqThreads() {
    pthread_t* thread_id = malloc(sizeof(pthread_t) * DEFAULT_MAX_QUEUE_SIZE);
    int* thread_value = malloc(sizeof(int) * DEFAULT_MAX_QUEUE_SIZE);
    bool* tr_id = malloc(sizeof(bool) * DEFAULT_MAX_QUEUE_SIZE);

    if (thread_id == NULL || thread_value == NULL || tr_id == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Create threads for enqueuing
    for (int i = 0; i < DEFAULT_MAX_QUEUE_SIZE; i++) {
        thread_value[i] = i + ONE;
        int err = pthread_create(&thread_id[i], NULL, enqueueThread, &thread_value[i]);
        if (err) {
            printf("ERROR: return code from pthread_create is %d \n", err);
            exit(EXIT_FAILURE);
        }
    }

    // Join threads and check their exit status
    for (int i = 0; i < DEFAULT_MAX_QUEUE_SIZE; i++) {
        pthread_join(thread_id[i], (void**)&tr_id[i]); // Cast to void**
        assert(tr_id[i] == true); // Check if the result is true
    }

    free(thread_id);
    free(thread_value);
    free(tr_id);
    return TEST_SUCCESS;
}

/**
 * Checks that enqueue threads will not wait and are able to enqueue after the queue is cleared.
 * 
 * This test ensures that the empty_slots semaphore indicating how many free spaces available for enqueuing elements are in the queue is
 * initialized to the max_size of the queue after calling BlockingQueue_clear().
*/
int enqThreadsAfterClear() {
    /** Enqueue 20 elements to the BlockingQueue.*/
    int a = 1;
    void *pointer_a = &a;
    //void *pointer_a
    for (int i = ZERO; i < DEFAULT_MAX_QUEUE_SIZE; i++) {
        assert(BlockingQueue_enq(queue, pointer_a) == true);
    }

    /** Asserts that the BlockingQueue is now full.*/
    assert(BlockingQueue_size(queue) == DEFAULT_MAX_QUEUE_SIZE);

    /** Clears the BlockingQueue.*/
    BlockingQueue_clear(queue);

    /** Ensures the Blocking Queue is now empty.*/
    assert(BlockingQueue_isEmpty(queue) == true);

    pthread_t* thread_id = malloc(sizeof(pthread_t) * DEFAULT_MAX_QUEUE_SIZE);
    int* thread_value = malloc(sizeof(int) * DEFAULT_MAX_QUEUE_SIZE);
    bool* tr_id = malloc(sizeof(bool) * DEFAULT_MAX_QUEUE_SIZE);

    if (thread_id == NULL || thread_value == NULL || tr_id == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    /**
     * Creates 20 threads to enqueue the integers elements from 1 to 20 included.
    */
    for (int i = 0; i < DEFAULT_MAX_QUEUE_SIZE; i++) {
        thread_value[i] = i + ONE;
        int err = pthread_create(&thread_id[i], NULL, enqueueThread, &thread_value[i]);
        if (err) {
            printf("ERROR: return code from pthread_create is %d \n", err);
            exit(EXIT_FAILURE);
        }
    }

    /**
     * Obtains the exit status of each thread which is a void pointer to the boolean response returned by BlockingQueue_enq.
     * Asserts that all enqueue threads terminated and exited by return true (all enqueuing operations must have been successfull since the Queue was cleared).
    */
    for (int i = 0; i < DEFAULT_MAX_QUEUE_SIZE; i++) {
        pthread_join(thread_id[i], (void**)&tr_id[i]); // Cast to void**
        assert(tr_id[i] == true); // Check if the result is true
    }

    free(thread_id);
    free(thread_value);
    free(tr_id);

    return TEST_SUCCESS;
}

/**
 * Checks that addtional enqueue threads will wait and are able to enqueue once the queue is cleared.
 * 
 * This test ensures that the empty_slots semaphore indicating how many free spaces available for enqueuing elements are in the queue is
 * initialized to the max_size of the queue after calling BlockingQueue_clear().
*/
int enqThreadsWaitBeforeClear() {

    pthread_t* thread_id = malloc(sizeof(pthread_t) * DEFAULT_MAX_QUEUE_SIZE);
    int* thread_value = malloc(sizeof(int) * DEFAULT_MAX_QUEUE_SIZE);
    bool* tr_id = malloc(sizeof(bool) * DEFAULT_MAX_QUEUE_SIZE);

    if (thread_id == NULL || thread_value == NULL || tr_id == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    /**
     * Creates 20 threads to enqueue the integers elements from 1 to 20 included.
    */
    for (int i = 0; i < DEFAULT_MAX_QUEUE_SIZE; i++) {
        thread_value[i] = i + ONE;
        int err = pthread_create(&thread_id[i], NULL, enqueueThread, &thread_value[i]);
        if (err) {
            printf("ERROR: return code from pthread_create is %d \n", err);
            exit(EXIT_FAILURE);
        }
    }

    /**
     * Obtains the exit status of each thread which is a void pointer to the boolean response returned by BlockingQueue_enq.
     * Asserts that all enqueue threads terminated and exited by return true (all enqueuing operations must have been successfull since the Queue was empty).
    */
    for (int i = 0; i < DEFAULT_MAX_QUEUE_SIZE; i++) {
        pthread_join(thread_id[i], (void**)&tr_id[i]); // Cast to void**
        assert(tr_id[i] == true); // Check if the result is true
    }

    /**
     * Thread identifiers:
     * 
     * tid1: enqueueing thread's identifier.
     * tid2: dequeueing thread's identifier.
    */
    pthread_t tid1, tid2;

    /**
     * Void pointers to the threads' identifiers:
     * 
     * tr1: pointer to the enqueueing thread's identifier.
     * tr2: pointer to the dequeueing thread's identifier.
    */
    void *tr1, *tr2;

    /** Integer element a = 1 to be enqueued.*/
    int a = 1;

    /** Void pointer to integer element a.*/
    void *pointer_a = &a;

    /** Integer element b = 2 to be enqueued.*/
    int b = 2;

    /** Void pointer to integer element b.*/
    void *pointer_b = &b;

    /** Create a thread to enqueue element a in the BlockingQueue.*/
    pthread_create(&tid1, NULL, enqueueThread, pointer_a);

    /** Create a thread to enqueue element b in the BlockingQueue*/
    pthread_create(&tid2, NULL, enqueueThread, pointer_b);


    /** Make the program sleep for 1 second, the two Enqueueing threads should be blocked and waiting for elements to be dequeued or the queue to be cleared.*/
    sleep(1);

    /** Clears the blocking queue which should allow the two waiting threads to enqueue elements.*/
    BlockingQueue_clear(queue);

    /**
     * Waits for the Enqueuing thread to return whether it successfully enqueued the element a,
     * and stores the boolean response at the location pointed by the void pointer tr1.
    */
    pthread_join( tid1, &tr1 );

    /**
     * Waits for the Enqueuing thread to return whether it successfully enqueued the element b,
     * and stores the boolean response at the location pointed by the void pointer tr2.
    */
    pthread_join( tid2, &tr2 );

    /** Asserts that the first enqueuing operation was successfull.*/
    assert( (bool)tr1 == true );

    /** Asserts that the second enqueuing operation was successfull.*/
    assert( (bool)tr2 == true );

    free(thread_id);
    free(thread_value);
    free(tr_id);

    return TEST_SUCCESS;
}

/**
 * Checks that a dequeue thread waits for an element to be enqueued after all the elements of the Queue have been cleared.
*/
int deqThreadAfterClear() {

    pthread_t* thread_id = malloc(sizeof(pthread_t) * DEFAULT_MAX_QUEUE_SIZE);
    int* thread_value = malloc(sizeof(int) * DEFAULT_MAX_QUEUE_SIZE);
    bool* tr_id = malloc(sizeof(bool) * DEFAULT_MAX_QUEUE_SIZE);

    if (thread_id == NULL || thread_value == NULL || tr_id == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Create threads for enqueuing
    for (int i = 0; i < DEFAULT_MAX_QUEUE_SIZE; i++) {
        thread_value[i] = i + ONE;
        int err = pthread_create(&thread_id[i], NULL, enqueueThread, &thread_value[i]);
        if (err) {
            printf("ERROR: return code from pthread_create is %d \n", err);
            exit(EXIT_FAILURE);
        }
    }

    // Join threads and check their exit status
    for (int i = 0; i < DEFAULT_MAX_QUEUE_SIZE; i++) {
        pthread_join(thread_id[i], (void**)&tr_id[i]); // Cast to void**
        assert(tr_id[i] == true); // Check if the result is true
    }

    /** Clears the queue.*/
    BlockingQueue_clear(queue);

    /** Initializes and defines an integer element to be enqueued and dequeued in/from the BlockingQueue.*/
    int a = 7;

    /** Defines a void pointer to the integer variable a to be passed in the enqueueing function.*/
    void *pointer_a = &a;

    /**
     * Defines two thread identifiers:
     * Thread 1 will be the dequeueing thread and the first one to be created.
     * Thread 2 will be the enqueueing thread and the second one to be created.
    */
    pthread_t tid1,tid2;
    /** Void pointers to threads 1 and 2 identifiers'.*/
    void *tr1, *tr2;

    /** Create a thread to dequeue an element off the empty BlockingQueue.*/
    pthread_create(&tid1, NULL, dequeueThread, queue);

    /** Create a thread to enqueue an element in the BlockingQueue.*/
    pthread_create(&tid2, NULL, enqueueThread, pointer_a);

    /** Waits for the Dequeuing thread to return the dequeued element and stores it at the location pointed by the void pointer tr1.*/
    pthread_join( tid1, &tr1 );

    /**
     * Waits for the Enqueuing thread to return whether it successfully enqueued the element,
     * and stores the boolean response at the location pointed by the void pointer tr2.
    */
    pthread_join( tid2, &tr2 );

    /** Asserts that the element dequeued is the enqueued element.*/
    assert( (*((int*)tr1)) == a );

    /** Asserts that the enqueuing operation was successfull.*/
    assert( (bool)tr2 == true );

    free(thread_id);
    free(thread_value);
    free(tr_id);

    return TEST_SUCCESS;
}

/**
 * Checks that a single enqueuing thread works.
*/
int enqThreadWorks() {
    int a = 1;
    void* pointer_a = &a;

    pthread_t enqueuingThread;
    void *enqueuingThreadResult;

    /**
     * Creates a thread to enqueue element a.
    */
    pthread_create(&enqueuingThread, NULL, enqueueThread, pointer_a);
    pthread_join(enqueuingThread, &enqueuingThreadResult);

    /** Ensures that the enqueuing thread was successfull.*/
    assert((bool)enqueuingThreadResult == true);
    return TEST_SUCCESS;
}

/**
 * Checks that a single dequeuing thread works.
*/
int deqThreadWorks() {
    int a = 1;
    void *pointer_a = &a;

    pthread_t dequeuingThread;
    void *dequeuingThreadResult;

    /** Enqueues element a in the blocking queue.*/
    BlockingQueue_enq(queue, pointer_a);

    /**
     * Create a thread to dequeue element a.
    */
    pthread_create(&dequeuingThread, NULL, dequeueThread, queue);
    pthread_join(dequeuingThread, &dequeuingThreadResult);

    /** Ensures that the dequeuing thread successfully returned element a.*/
    assert((*((int*)dequeuingThreadResult)) == a);
    return TEST_SUCCESS;
}

/**
 * Thread to enqueue many element (from 1 to iteration).
*/
void* many_enq_thread(void* iterations) {
    for (int i = ONE; i <= *(int*)iterations; i++) {
        BlockingQueue_enq(queue, (void*)(__intptr_t)i);
    }
    pthread_exit(NULL);
}

/**
 * Thread to enqueue many element (from 1 to iteration).
*/
void* many_deq_thread(void* iterations) {
    for (int i = 1; i <= *(int*)iterations; i++) {
        BlockingQueue_deq(queue);
    }
    pthread_exit(NULL);
}

/**
 * Thread to perform enqueue and dequeue operations and check the size of the queue.
*/
void* enq_deq_size_thread(void* iterations) {
    for (int i = 1; i <= *(int*)iterations; i++) {
        BlockingQueue_enq(queue, (void*)(__intptr_t)i); // Enqueue an element
        BlockingQueue_deq(queue); // Dequeue the element
        assert(BlockingQueue_size(queue) == ZERO); // Asserts that the size of the queue is always zero
    }
    pthread_exit(NULL);
}

/**
 * Thread to perform enqueue and dequeue operations and check that the queue is empty.
*/
void* enq_deq_isEmpty_thread(void* iterations) {
    for (int i = 1; i <= *(int*)iterations; i++) {
        BlockingQueue_enq(queue, (void*)(__intptr_t)i); // Enqueue an element
        BlockingQueue_deq(queue); // Dequeue the element
        assert(BlockingQueue_isEmpty(queue) == true); // Asserts that the queue is always empty
    }
    pthread_exit(NULL);
}

/**
 * Thread to perform an enqueue operation and clear the queue and check that the queue has been emptied.
*/
void* enq_clear_size_thread(void* iterations) {
    for (int i = 1; i <= *(int*)iterations; i++) {
        BlockingQueue_enq(queue, (void*)(__intptr_t)i); // Enqueue an element
        BlockingQueue_clear(queue); // Clears the queue
        assert(BlockingQueue_isEmpty(queue) == true); // Asserts that the queue is always empty
    }
    pthread_exit(NULL);
}

/**
 * Verifies that BlockingQueue_enq behaves as expected under concurrent access by multiple threads.
*/
int test_BlockingQueue_enq_threadSafety() {
    int iterations = 2;
    int num_threads = 10;
    pthread_t* thread_id = malloc(sizeof(pthread_t) * num_threads);

    for (int i = ZERO; i < num_threads; i++) {
        pthread_create(&thread_id[i], NULL, many_enq_thread, &iterations);
    }
    for (int i = ZERO; i < num_threads; i++) {
        pthread_join(thread_id[i], NULL);
    }
    assert(BlockingQueue_size(queue) == iterations * num_threads);
    free(thread_id);
    return TEST_SUCCESS;
}

/**
 * Verifies that BlockingQueue_deq behave as expected under concurrent access by multiple threads.
*/
int test_BlockingQueue_deq_threadSafety() {
    int iterations = 2;
    int num_threads = 10;
    pthread_t* thread_id = malloc(sizeof(pthread_t) * num_threads);

    /** Adds 20 elements to the blocking queue making it full.*/
    for (int i = ZERO; i < num_threads * iterations; i++) {
        BlockingQueue_enq(queue, (void*)(__intptr_t)i);
    }

    for (int i = ZERO; i < num_threads; i++) {
        pthread_create(&thread_id[i], NULL, many_deq_thread, &iterations);
    }
    for (int i = ZERO; i < num_threads; i++) {
        pthread_join(thread_id[i], NULL);
    }
    assert(BlockingQueue_isEmpty(queue) == true);
    free(thread_id);
    return TEST_SUCCESS;
}

/**
 * Ensures thread safety of BlockingQueue_size under concurrent access.
 * 
 * To do this, creates multiple threads performing enqueue and dequeue operations while checking the size of the queue.
 * The test ensures that even under concurrent access, the size of the queue remains consistent and reflects the correct state of the queue.
*/
int test_BlockingQueue_size_threadSafety() {
    int iterations = 2;
    int num_threads = 10;
    pthread_t* thread_id = malloc(sizeof(pthread_t) * num_threads);

    /**
     * Creates multiple threads each performing a series of enqueue and dequeue operations concurrently.
     * After each operation, the thread checks the size of the queue and asserts it's always zero (because we enqueue and dequeue one element).
    */
    for (int i = ZERO; i < num_threads; i++) {
        pthread_create(&thread_id[i], NULL, enq_deq_size_thread, &iterations);
    }
    for (int i = ZERO; i < num_threads; i++) {
        pthread_join(thread_id[i], NULL);
    }
    /** Ensures that the queue is empty after all operations.*/
    assert(BlockingQueue_isEmpty(queue) == true);
    free(thread_id);
    return TEST_SUCCESS;
}

/**
 * Ensures thread safety of BlockingQueue_isEmpty under concurrent access.
 * 
 * To do this, creates multiple threads performing enqueue, dequeue, and isEmpty operations concurrently while checking the emptiness of the queue.
 * The test ensures that even under concurrent access, the isEmpty operation remains consistent and reflects the correct state of the queue.
*/
int test_BlockingQueue_isEmpty_threadSafety() {
    int iterations = 2;
    int num_threads = 10;
    pthread_t* thread_id = malloc(sizeof(pthread_t) * num_threads);

    /**
     * Creates multiple threads each performing a series of enqueue, dequeue, and isEmpty operations concurrently.
     * After each operation, the thread checks that the queue is always empty (because we enqueue and then clear the queue).
    */
    for (int i = ZERO; i < num_threads; i++) {
        pthread_create(&thread_id[i], NULL, enq_deq_isEmpty_thread, &iterations);
    }
    for (int i = ZERO; i < num_threads; i++) {
        pthread_join(thread_id[i], NULL);
    }

    /** Ensures that the queue is empty after all operations.*/
    assert(BlockingQueue_isEmpty(queue) == true);

    free(thread_id);
    return TEST_SUCCESS;
}

/**
 * Ensures thread safety of BlockingQueue_clear under concurrent access.
 * 
 * To do this, creates multiple threads performing enqueue and clear operations concurrently while checking the emptiness of the queue.
 * The test ensures that even under concurrent access, clearing the queue remains consistent and reflects the correct state of the queue (emptied after each operation).
*/
int test_BlockingQueue_clear_threadSafety() {
    int iterations = 2;
    int num_threads = 10;
    pthread_t* thread_id = malloc(sizeof(pthread_t) * num_threads);

    /**
     * Creates multiple threads each performing a series of enqueue and clearing operations concurrently.
     * After each operation, the thread checks that the queue is always empty (because we enqueue and then clear the queue).
    */
    for (int i = ZERO; i < num_threads; i++) {
        pthread_create(&thread_id[i], NULL, enq_clear_size_thread, &iterations);
    }
    for (int i = ZERO; i < num_threads; i++) {
        pthread_join(thread_id[i], NULL);
    }

    /** Ensures that the queue is empty after all operations.*/
    assert(BlockingQueue_isEmpty(queue) == true);

    free(thread_id);
    return TEST_SUCCESS;
}

/*
 * Main function for the BlockingQueue tests which will run each user-defined test in turn.
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
    runTest(deqFirstThenEnqOneElement);

    runTest(enqThreadWhenFull);

    runTest(negativeSizedQueue);

    runTest(zeroSizedQueue);

    runTest(isEmptyWorks);

    runTest(isEmptyWorksAfterDequeue);

    runTest(sizeIncreasesAfterEnq);

    runTest(sizeDecreasesAfterDeq);

    runTest(clearWorks);

    runTest(enqOneElement);

    runTest(enqAndDeqOneElement);

    runTest(enqAndDeqInteger);

    runTest(enqAndDeqDouble);

    runTest(enqAndDeqCharacter);

    runTest(enqAndDeqString);

    runTest(manyEnqThreads);

    runTest(enqThreadWorks);

    runTest(deqThreadWorks);

    //runTest(isEmptyThreadWorks);

    runTest(enqThreadsAfterClear);

    runTest(enqThreadsWaitBeforeClear);

    runTest(deqThreadAfterClear);

    runTest(test_BlockingQueue_enq_threadSafety);

    runTest(test_BlockingQueue_deq_threadSafety);

    runTest(test_BlockingQueue_size_threadSafety);

    runTest(test_BlockingQueue_clear_threadSafety);

    printf("\nBlockingQueue Tests complete: %d / %d tests successful.\n----------------\n", success_count, total_count);

}
