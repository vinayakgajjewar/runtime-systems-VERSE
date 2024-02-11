#ifndef VERSE_REG_INTERPRETER_H_
#define VERSE_REG_INTERPRETER_H_

#include <stdint.h>

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

void decode(uint16_t instruction,
            uint8_t *op,
            uint8_t *r0,
            uint8_t *r1,
            uint8_t *r2,
            uint8_t *imm);

result interpret(uint16_t *bytecode);

#endif