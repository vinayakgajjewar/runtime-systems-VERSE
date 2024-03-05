/*
 * Take a human-readable source file and compile it down into bytecode. Output
 * the instructions into a file named <dest>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USAGE_STR "Usage: ./stack-assemble <source> <dest>\n"

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
     * Same with the destination file.
     */
    FILE *dest_f = fopen(dest, "w");
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

        /*
         * Instructions in this language are 1 byte long.
         */
        unsigned char instruction;

        /*
         * "Parse" the source file.
         *
         * TODO Maybe I should put these values into a lookup table.
         */
        if (strcmp(line, "PUSH_IMM\n") == 0) {
            printf("PUSH_IMM\n");

            /*
             * Write the PUSH_IMM instruction itself.
             */
            instruction = strtol("00000000", NULL, 2);
            fwrite(&instruction, sizeof(instruction), 1, dest_f);

            /*
             * Get the immediate value from the next line.
             */
            if (fgets(line, sizeof(line), src_f) == NULL) {
                fprintf(stderr, "Could not read immediate value\n");
                fflush(stderr);
                exit(EXIT_FAILURE);
            }
            printf("%s\n", line);

            /*
             * TODO This is just hard-coded for now. I need to figure out how to
             * TODO get the actual immediate value eventually.
             */
            instruction = strtol("00001100", NULL, 2);
        } else if (strcmp(line, "ADD\n") == 0) {
            printf("ADD\n");
            instruction = strtol("00000001", NULL, 2);
        } else if (strcmp(line, "SUB\n") == 0) {
            printf("SUB\n");
            instruction = strtol("00000010", NULL, 2);
        } else if (strcmp(line, "MUL\n") == 0) {
            printf("MUL\n");
            instruction = strtol("00000011", NULL, 2);
        } else if (strcmp(line, "DIV\n") == 0) {
            printf("DIV\n");
            instruction = strtol("00000100", NULL, 2);
        } else if (strcmp(line, "POP_RES\n") == 0) {
            printf("POP_RES\n");
            instruction = strtol("00000101", NULL, 2);
        } else if (strcmp(line, "DONE\n") == 0) {
            printf("DONE\n");
            instruction = strtol("00000110", NULL, 2);
        }

        /*
         * Write the instruction into the destination file.
         */
        fwrite(&instruction, sizeof(instruction), 1, dest_f);
    }

    /*
     * Close the source file.
     */
    fclose(src_f);
}