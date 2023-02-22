CC=gcc -Wall -Wextra -std=gnu99 -g
LIBS=-lpthread

all: console
	
console: console.o thread_wrapper.o
	$(CC) $^ -o $@ $(LIBS)

thread_wrapper.o: thread_wrapper.c thread_wrapper.h
	$(CC) $< -c

clean:
	rm -f console *.o

run:
	./console
