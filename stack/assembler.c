/*
 * Take a human-readable source file and compile it down into bytecode. Output
 * the instructions into a file named <dest>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYTE_LEN 8
#define USAGE_STR "Usage: ./stack-assemble <source> <dest>\n"

/*
 * TODO
 * I should also macro-ify the error strings.
 */

/*
 * It would also be nice to macro-ify the instruction strings themselves. I'd
 * like to avoid having any "magic literals" in my code if possible.
 */
#define PUSH_IMM    "PUSH_IMM\n"
#define ADD         "ADD\n"
#define SUB         "SUB\n"
#define MUL         "MUL\n"
#define DIV         "DIV\n"
#define AND         "AND\n"
#define OR          "OR\n"
#define XOR         "XOR\n"
#define NOT         "NOT\n"
#define LSHIFT      "LSHIFT\n"
#define RSHIFT      "RSHIFT\n"
#define POP_RES     "POP_RES\n"
#define DONE        "DONE\n"

/*
 * Define binary strings corresponding to each opcode we support.
 */
#define PUSH_IMM_STR    "00000000"
#define ADD_STR         "00000001"
#define SUB_STR         "00000010"
#define MUL_STR         "00000011"
#define DIV_STR         "00000100"
#define AND_STR         "00000101"
#define OR_STR          "00000110"
#define XOR_STR         "00000111"
#define NOT_STR         "00001000"
#define LSHIFT_STR      "00001001"
#define RSHIFT_STR      "00001010"
#define POP_RES_STR     "00001011"
#define DONE_STR        "00001100"

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
         */
        if (strcmp(line, PUSH_IMM) == 0) {
            printf(PUSH_IMM);

            /*
             * Write the PUSH_IMM instruction itself.
             */
            instruction = strtol(PUSH_IMM_STR, NULL, 2);
            fwrite(&instruction, sizeof(instruction), 1, dest_f);

            /*
             * Get the immediate value from the next line.
             */
            if (fgets(line, sizeof(line), src_f) == NULL) {
                fprintf(stderr, "Could not read immediate value\n");
                fflush(stderr);
                exit(EXIT_FAILURE);
            }
            printf("%s", line);
            char *imm_str = imm_bin_str(strtoul(line, NULL, 10));
            instruction = strtol(imm_str, NULL, 2);
            free(imm_str);
        } else if (strcmp(line, ADD) == 0) {
            printf(ADD);
            instruction = strtol(ADD_STR, NULL, 2);
        } else if (strcmp(line, SUB) == 0) {
            printf(SUB);
            instruction = strtol(SUB_STR, NULL, 2);
        } else if (strcmp(line, MUL) == 0) {
            printf(MUL);
            instruction = strtol(MUL_STR, NULL, 2);
        } else if (strcmp(line, DIV) == 0) {
            printf(DIV);
            instruction = strtol(DIV_STR, NULL, 2);
        } else if (strcmp(line, AND) == 0) {
            printf(AND);
            instruction = strtol(AND_STR, NULL, 2);
        } else if (strcmp(line, OR) == 0) {
            printf(OR);
            instruction = strtol(OR_STR, NULL, 2);
        } else if (strcmp(line, XOR) == 0) {
            printf(XOR);
            instruction = strtol(XOR_STR, NULL, 2);
        } else if (strcmp(line, NOT) == 0) {
            printf(NOT);
            instruction = strtol(NOT_STR, NULL, 2);
        } else if (strcmp(line, LSHIFT) == 0) {
            printf(LSHIFT);
            instruction = strtol(LSHIFT_STR, NULL, 2);
        } else if (strcmp(line, RSHIFT) == 0) {
            printf(RSHIFT);
            instruction = strtol(RSHIFT_STR, NULL, 2);
        } else if (strcmp(line, POP_RES) == 0) {
            printf(POP_RES);
            instruction = strtol(POP_RES_STR, NULL, 2);
        } else if (strcmp(line, DONE) == 0) {
            printf(DONE);
            instruction = strtol(DONE_STR, NULL, 2);
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
     * Close the source file.
     */
    fclose(src_f);

    /*
     * Close the destination file.
     */
    fclose(dest_f);
}