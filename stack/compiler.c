/*
 * Take a human-readable source file and compile it down into bytecode. Output
 * the instructions into a file named <dest>.
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    /*
     * Check for the correct number of arguments.
     */
    if (argc != 3) {
        printf("Usage: ./stack-compile <source> <dest>\n");
    }

    /*
     * Unpack our arguments.
     */
    char *src = argv[1];
    char *dest = argv[2];

    /*
     * Try to open the source file and complain if we fail.
     */
    FILE *src_f = fopen(src, "rb");
    if (src_f == NULL) {
        fprintf(stderr, "Can't open provided source file\n");
        exit(EXIT_FAILURE);
    }
}