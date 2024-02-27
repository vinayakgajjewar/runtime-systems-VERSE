#include <assert.h>
#include <time.h>

#include "vm.h"

#define MAX_SIZE 100

int main(int argc, char *argv[]) {

    /*
     * Start the clock.
     */
    clock_t begin = clock();

    /*
     * Check for the correct number of arguments.
     */
    if (argc != 2) {
        printf("Usage: ./reg-vm <bytecode file>\n");
        exit(EXIT_FAILURE);
    }

    /*
     * Try opening the input file and complain if things go wrong.
     */
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        fprintf(stderr, "Error opening source file\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    /*
     * Read the program source into memory.
     */
    /*uint16_t code[MAX_SIZE];
    size_t size_read = fread(code, sizeof(uint16_t), MAX_SIZE, file);
    if (size_read == 0) {
        fprintf(stderr, "Error reading source file\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }*/

    uint16_t code[] = {
            ENCODE_OP_REG_IMM(LOAD_IMM, 3, 5),
            ENCODE_OP_REG(MOV_RES, 3),
            ENCODE_OP(DONE)
    };

    /*
     * Print out whatever we just read in.
     */
    for (size_t i = 0; i < 3; i++) {
        printf("%zu: %X\n", i, code[i]);
    }

    reset_vm();
    result res = interpret(code);
    assert(res == SUCCESS);
    assert(vm.result == 5);

    /*
     * Stop the clock.
     */
    clock_t end = clock();
    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
    printf("Execution took %lf seconds\n", time_spent);
}