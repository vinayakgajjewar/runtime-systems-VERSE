#ifndef VERSE_REG_INTERPRETER_H_
#define VERSE_REG_INTERPRETER_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_REGS 16

/*
 * Helpful macros for writing bytecode.
 */
#define ENCODE_OP(op)                   ((op) << 12)
#define ENCODE_OP_REG(op, reg)          (ENCODE_OP(op) | ((reg) << 8))
#define ENCODE_OP_REG_IMM(op, reg, imm) (ENCODE_OP(op) | ((reg) << 8) | (imm))
#define ENCODE_OP_REGS(op, r0, r1, r2)  (ENCODE_OP(op) | ((r0) << 8) | ((r1) << 4) | (r2))

/*
 * Helpful macros for decoding instructions.
 */
#define DECODE_OP(instruction)  ((instruction & 0xF000) >> 12)
#define DECODE_R0(instruction)  ((instruction & 0x0F00) >> 8)
#define DECODE_R1(instruction)  ((instruction & 0x00F0) >> 4)
#define DECODE_R2(instruction)  (instruction & 0x000F)
#define DECODE_IMM(instruction) (instruction & 0x00FF)

struct {
    uint16_t *instruction_ptr;
    uint64_t regs[NUM_REGS];
    uint64_t result;
} vm;

typedef enum {
    LOAD_IMM,
    ADD,
    SUB,
    MUL,
    DIV,
    MOV_RES,
    DONE
} opcode;

typedef enum result {
    SUCCESS,
    ERR_DIV_ZERO,
    ERR_UNKNOWN_OPCODE
} result;

void reset_vm();

result interpret(uint16_t *bytecode) {
    vm.instruction_ptr = bytecode;
    uint8_t op;
    uint8_t r0;
    uint8_t r1;
    uint8_t r2;
    uint8_t imm;

    /*
     * Loop until we hit a DONE instruction or an instruction we don't
     * recognize.
     */
    for (;;) {

        /*
         * Fetch the next instruction and decode its arguments.
         */
        uint16_t instruction = *vm.instruction_ptr++;
        op = DECODE_OP(instruction);
        r0 = DECODE_R0(instruction);
        r1 = DECODE_R1(instruction);
        r2 = DECODE_R2(instruction);
        imm = DECODE_IMM(instruction);

        /*
         * Dispatch based on the opcode.
         */
        switch (op) {
            case LOAD_IMM:
                break;
            case ADD:
                break;
            case SUB:
                break;
            case MUL:
                break;
            case DIV:
                break;
            case MOV_RES:
                break;
            case DONE:
                break;
            default:
                fprintf(stderr, "Unknown opcode\n");
                fflush(stderr);
                exit(EXIT_FAILURE);
        }
    }
}

#endif