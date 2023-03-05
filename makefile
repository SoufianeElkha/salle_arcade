CC=gcc -Wall -Wextra -std=gnu99 -fsanitize=address -fsanitize=leak -fsanitize=undefined
LIBS=-lpthread

all: tp3_salle_arcade
	
tp3_salle_arcade: tp3_salle_arcade.o
	$(CC) $^ -o $@ $(LIBS)

clean:
	rm -f tp3_salle_arcade *.o
