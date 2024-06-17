# CS2002 W11-SP

# 1. How to build

See [Makefile](Makefile) for the build scripts.

To build both the *TestQueue* and *TestBlockingQueue* executables, simply run **make** at the command line in the current directory.

To run the test suites, run **./TestQueue** for testing the Queue and **./TestBlockingQueue** for testing the BlockingQueue.

**Important:** the command **make clean** will try to delete any executable or .o files in the src directory.


# 2. Project Overview

1. Blocking Queue:
My program implementation is in [BlockingQueue.c](BlockingQueue.c) where you will find all the functions defined in the header file implemented as required.
You will also find a new static function private to the BlockingQueue.c file called **clean_exit()** which is a small helper function for cleanup and error
handling of the functions from the POSIX library.
The [BlockingQueue's header file](BlockingQueue.h) has been edited to add MACRO definitions as well as defining the BlockingQueue struct.

2. Queue
My program implementation is in [Queue.c](Queue.c) where where you will find all the functions defined in the header file implemented as required.
No helper functions were needed for the non-thread safe queue.
The [Queue's header file](Queue.h) has been edited to add MACRO definitions as well as defining the Queue struct.

3. Makefile
The [Makefile](Makefile) has not been edited.


# 3. Testing Framework

To run the test suites, run **./TestQueue** for testing the Queue and **./TestBlockingQueue** for testing the BlockingQueue.

The [BlockingQueue's test suite](TestBlockingQueue.c) contains **27 unit tests**, some with several assertions.
the [Queue's test suite](TestQueue.c) contains **22 unittests**, some with several assertions.

The **./TestBlockingQueue** can be a bit slow to execute because of the several sleep() calls ensuring that enq and deq threads wait when expected to.