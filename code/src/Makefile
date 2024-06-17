CC = clang
RM = rm -f
DFLAG = -g
GFLAGS = -Wall -Wextra
CFLAGS = $(DFLAG) $(GFLAGS) -c
LFLAGS = $(DFLAG) $(GFLAGS)
LIBFLAGS = -pthread

all: TestQueue TestBlockingQueue

TestQueue: TestQueue.o Queue.o 
	$(CC) $(LFLAGS) TestQueue.o Queue.o -o TestQueue $(LIBFLAGS)

TestBlockingQueue: TestBlockingQueue.o BlockingQueue.o Queue.o
	$(CC) $(LFLAGS) TestBlockingQueue.o BlockingQueue.o Queue.o -o TestBlockingQueue $(LIBFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<


clean:
	$(RM) TestQueue TestBlockingQueue *.o
