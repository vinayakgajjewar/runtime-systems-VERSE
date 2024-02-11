CC = gcc
CFLAGS = -Wall -g -std=gnu17
EXECUTABLES = reg-vm test-encode

reg-vm: reg/interpreter.h reg/interpreter.c
	$(CC) $(CFLAGS) -o reg-vm reg/interpreter.c

test-encode: reg/interpreter.h reg/test_encode.c
	$(CC) $(CFLAGS) -o test-encode reg/test_encode.c

all: $(EXECUTABLES)

clean:
	rm -f $(EXECUTABLES) *.o