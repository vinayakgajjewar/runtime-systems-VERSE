#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USAGE_STR "Usage: ./reg-assemble <source> <dest>\n"

/*
 * Macro-ify the instruction strings to avoid having any "magic literals" in my
 * code.
 *
 * TODO
 */

/*
 * Convert a human-readable source file into bytecode.
 */
int main(int argc, char *argv[]) {

    /*
     * Check for the correct number of arguments.
     */
    if (argc != 3) {
        printf(USAGE_STR);
        exit(EXIT_FAILURE);
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
        fprintf(stderr, "Can't open %s\n", src);
        exit(EXIT_FAILURE);
    }

    /*
     * Do the same with our destination file.
     */
    FILE *dest_f = fopen(dest, "w");
    if (dest_f == NULL) {
        fprintf(stderr, "Can't open %s\n", dest);
        exit(EXIT_FAILURE);
    }

    /*
     * Read lines until we reach the end of the file.
     */
    char line[100];
    while (fgets(line, sizeof(line), src_f) != NULL) {

        /*
         * Instructions in this language are 2 bytes long.
         */
        uint16_t instruction;
    }

    /*
     * Close the source and destination files.
     */
    fclose(src_f);
    fclose(dest_f);
}