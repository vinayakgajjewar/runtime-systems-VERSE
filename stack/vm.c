#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "vm.h"

/*
 * Right now, we don't support source files larger than 100 bytes.
 */
#define MAX_SIZE 100

#define USAGE_STR "Usage: ./stckvm <bytecode file> <dispatch type>\n"

int main(int argc, char *argv[]) {

    /*
     * Start the clock.
     */
    clock_t begin = clock();

    /*
     * Check for the correct number of arguments.
     */
    if (argc != 3) {
        printf(USAGE_STR);
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
    uint8_t code[MAX_SIZE];
    size_t size_read = fread(code, sizeof(uint8_t), MAX_SIZE, file);
    if (size_read == 0) {
        fprintf(stderr, "Error reading source file\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    /*
     * Print out what we read.
     */
    for (size_t i = 0; i < size_read; i++) {
        printf("%zu:\t%02X\n", i, code[i]);
    }

    /*
     * Invoke the interpreter depending on what the user specifies.
     */
    reset_vm();
    result r;
    if (strcmp(argv[2], "inline") == 0) {
        printf("Invoking inline interpreter\n");
        fflush(stdout);
        r = interpret_inline(code);
    } else if (strcmp(argv[2], "func") == 0) {
        printf("Invoking function dispatch interpreter\n");
        fflush(stdout);
        r = interpret_function_dispatch(code);
    } else if (strcmp(argv[2], "threaded") == 0) {
        printf("Invoking direct threaded interpreter\n");
        fflush(stdout);
        r = interpret_threaded_dispatch(code);
    } else {
        fprintf(stderr, "Unrecognized dispatch type\n");
        fprintf(stderr, "Quitting...\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }
    assert(r == SUCCESS);
    printf("Result: %" PRIu64 "\n", vm.result);

    /*
     * Stop the clock.
     */
    clock_t end = clock();
    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
    printf("Execution took %lf seconds\n", time_spent);
}