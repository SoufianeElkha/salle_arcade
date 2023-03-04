CC=gcc -Wall -Wextra -std=gnu99 -g
LIBS=-lpthread

all: play
	
play: play.o
	$(CC) $^ -o $@ $(LIBS)

clean:
	rm -f play *.o
