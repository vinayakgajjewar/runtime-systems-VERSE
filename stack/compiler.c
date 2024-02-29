/*
 * Take a human-readable source file and compile it down into bytecode. Output
 * the instructions into a file named <dest>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        fprintf(stderr, "Can't open %s\n", src);
        exit(EXIT_FAILURE);
    }

    /*
     * Same with the destination file.
     */
    FILE *dest_f = fopen(dest, "rb");
    if (dest_f == NULL) {
        fprintf(stderr, "Can't open %s\n", dest);
        exit(EXIT_FAILURE);
    }

    /*
     * Read lines until we reach the end of the file. 100 characters is kind of
     * overkill, but we're not really suffering because of it.
     */
    char line[100];
    while (fgets(line, sizeof(line), src_f) != NULL) {
        //printf("%s", line);
        fflush(stdout);
        if (strcmp(line, "PUSH_IMM\n") == 0) {
            printf("PUSH_IMM\n");
        } else if (strcmp(line, "ADD\n") == 0) {
            printf("ADD\n");
        } else if (strcmp(line, "SUB\n") == 0) {
            printf("SUB\n");
        } else if (strcmp(line, "MUL\n") == 0) {
            printf("MUL\n");
        } else if (strcmp(line, "DIV\n") == 0) {
            printf("DIV\n");
        } else if (strcmp(line, "POP_RES\n") == 0) {
            printf("POP_RES\n");
        } else if (strcmp(line, "DONE\n") == 0) {
            printf("DONE\n");
        }
    }

    /*
     * Close the source file.
     */
    fclose(src_f);
}