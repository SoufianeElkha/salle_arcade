# Compiler
CC=gcc 
# Flag for debug and error verification
CFLAGS=-Wall -Wextra -std=gnu99 -fsanitize=address -fsanitize=leak -fsanitize=undefined
# library
LIBS=-lpthread

# Executable name
TARGET=tp3_salle_arcade
SRC=src/

# All compile
all: $(TARGET) 

# Executable
$(TARGET): $(SRC)main.o
	$(CC) $^ $(CFLAGS) -o $@ $(LIBS)

clean:
	rm -f $(TARGET) *.o && rm -f $(SRC)*.o
	@echo "Cleanup complete!"


