# Compiler
CC=gcc 
# Flag for debug and error verification
CFLAGS=-Wall -Wextra -std=gnu99 -fsanitize=address -fsanitize=leak -fsanitize=undefined
# library
LIBS=-lpthread

# Executable name
TARGET=tp3_salle_arcade

# All compile
all: $(TARGET) 

# Executable
$(TARGET): main.o
	$(CC) $^ $(CFLAGS) -o $@ $(LIBS)

clean:
	rm -f $(TARGET) *.o
	@echo "Cleanup complete!"


