CC = gcc
CFLAGS = -Wall -std=gnu17 -O0
EXECUTABLES = reg-vm test-encode stckvm stacka reg-assemble

stckvm: stack/vm.h stack/vm.c
	$(CC) $(CFLAGS) -o stckvm stack/vm.c

stacka: stack/assembler.c
	$(CC) $(CFLAGS) -o stacka stack/assembler.c

reg-vm: reg/vm.h reg/vm.c
	$(CC) $(CFLAGS) -o reg-vm reg/vm.c

reg-assemble: reg/assembler.c
	$(CC) $(CFLAGS) -o reg-assemble reg/assembler.c

test-encode: reg/vm.h reg/test_encode.c
	$(CC) $(CFLAGS) -o test-encode reg/test_encode.c

all: $(EXECUTABLES)

clean:
	rm -rf $(EXECUTABLES) *.o *.dSYM reg/*.gch stack/*.gch