#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>

#include "interpreter.h"

int main(int argc, char *argv[]) {

    /*
     * Check for the correct number of arguments.
     */
    if (argc != 2) {
        printf("Usage: ./stack-vm <bytecode file>\n");
        exit(EXIT_FAILURE);
    }

    /*
     * Try opening the input file.
     */
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        fprintf(stderr, "Error opening source file\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    /*
     * Read the source file into memory.
     */
    uint8_t code[7];
    size_t size_read = fread(code, sizeof(uint8_t), 7, file);
    if (size_read == 0) {
        fprintf(stderr, "Error reading source file\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    /*
     * Print out what we read.
     */
    for (size_t i = 0; i < size_read; i++) {
        printf("%02X ", code[i]);
    }
    printf("\n");

    /*
     * Run our interpreter.
     */
    reset_vm();
    result r = interpret(code);
    assert(r == SUCCESS);
    printf("Result: %" PRIu64 "\n", vm.res);
}