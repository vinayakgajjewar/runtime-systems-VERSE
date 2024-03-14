#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYTE_LEN 8
#define USAGE_STR "Usage: ./reg-assemble <source> <dest>\n"

/*
 * Macro-ify the instruction strings to avoid having any "magic literals" in my
 * code.
 *
 * TODO
 */
#define LOAD_IMM_STR    "LOAD_IMM\n"
#define ADD_STR         "ADD\n"
#define SUB_STR         "SUB\n"
#define MUL_STR         "MUL\n"
#define DIV_STR         "DIV\n"
#define MOV_RES_STR     "MOV_RES\n"
#define DONE_STR        "DONE\n"

/*
 * Define binary strings corresponding to each opcode we recognize.
 *
 * TODO
 * This actually might be a bit more complicated because of encoding operands. I
 * guess the opcode is the first 4 bits?
 */
#define LOAD_IMM_BIN    "0000"
#define ADD_BIN         "0001"
#define SUB_BIN         "0010"
#define MUL_BIN         "0011"
#define DIV_BIN         "0100"
#define MOVE_RES_BIN    "0101"
#define DONE_BIN        "0110"

/*
 * Define a helper function for converting immediate values into binary strings.
 */
char *imm_bin_str(uint8_t imm) {
    char *bin_str = (char *) malloc(BYTE_LEN + 1);
    if (bin_str == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }
    for (int i = BYTE_LEN - 1; i >= 0; i--) {
        bin_str[i] = (imm & 1) ? '1' : '0';
        imm >>= 1;
    }
    bin_str[BYTE_LEN] = '\0';
    return bin_str;
}

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

        /*
         * "Parse" the source file. We'll require that each token is on its own
         * line just to make our lives easier.
         */
        if (strcmp(line, LOAD_IMM_STR) == 0) {
            printf(LOAD_IMM_STR);
        } else if (strcmp(line, ADD_STR) == 0) {
            printf(ADD_STR);
        } else if (strcmp(line, SUB_STR) == 0) {
            printf(SUB_STR);
        } else if (strcmp(line, MUL_STR) == 0) {
            printf(MUL_STR);
        } else if (strcmp(line, DIV_STR) == 0) {
            printf(DIV_STR);
        } else if (strcmp(line, MOV_RES_STR) == 0) {
            printf(MOV_RES_STR);
        } else if (strcmp(line, DONE_STR) == 0) {
            printf(DONE_STR);
        } else {
            fprintf(stderr, "Cannot parse line\n");
            fflush(stderr);
            exit(EXIT_FAILURE);
        }

        /*
         * Write the instruction into the destination file.
         */
        fwrite(&instruction, sizeof(instruction), 1, dest_f);
    }

    /*
     * Close the source and destination files.
     */
    fclose(src_f);
    fclose(dest_f);
}