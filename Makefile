CC = gcc
CFLAGS = -Wall -std=gnu17 -O0
EXECUTABLES = reg-vm test-encode stack-vm stack-assemble

stack-vm: stack/vm.h stack/vm.c
	$(CC) $(CFLAGS) -o stack-vm stack/vm.c

stack-assemble: stack/assembler.c
	$(CC) $(CFLAGS) -o stack-assemble stack/assembler.c

reg-vm: reg/vm.h reg/vm.c
	$(CC) $(CFLAGS) -o reg-vm reg/vm.c

test-encode: reg/vm.h reg/test_encode.c
	$(CC) $(CFLAGS) -o test-encode reg/test_encode.c

all: $(EXECUTABLES)

clean:
	rm -rf $(EXECUTABLES) *.o *.dSYM reg/*.gch stack/*.gch