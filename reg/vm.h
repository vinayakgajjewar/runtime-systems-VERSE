#ifndef VERSE_REG_VM_H_
#define VERSE_REG_VM_H_

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Our virtual machine has 16 registers with 64 bits each.
 */
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

/*
 * Define all the different opcodes we support.
 */
typedef enum {
    LOAD_IMM,
    ADD,
    SUB,
    MUL,
    DIV,
    MOV_RES,
    DONE
} opcode;

/*
 * Define possible exit statuses for our VM.
 */
typedef enum result {
    SUCCESS,
    ERR_DIV_ZERO,
    ERR_UNKNOWN_OPCODE
} result;

void reset_vm() {
    printf("Resetting VM state\n");
    vm = (typeof(vm)) {NULL};
}

/*
 * The heart and soul of our virtual machine.
 */
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
                printf("LOAD_IMM %" PRIu8 "\n", imm);
                vm.regs[r0] = imm;
                break;
            case ADD:
                printf("ADD\n");
                vm.regs[r2] = vm.regs[r0] + vm.regs[r1];
                break;
            case SUB:
                printf("SUB\n");
                vm.regs[r2] = vm.regs[r0] - vm.regs[r1];
                break;
            case MUL:
                printf("MUL\n");
                vm.regs[r2] = vm.regs[r0] * vm.regs[r1];
                break;
            case DIV:
                printf("DIV\n");
                if (vm.regs[r1] == 0) {
                    return ERR_DIV_ZERO;
                }
                vm.regs[r2] = vm.regs[r0] / vm.regs[r1];
                break;
            case MOV_RES:
                printf("MOV_RES %" PRIu64 "\n", vm.regs[r0]);
                vm.result = vm.regs[r0];
                break;
            case DONE:
                printf("DONE\n");
                return SUCCESS;
            default:
                fprintf(stderr, "Unknown opcode\n");
                fflush(stderr);
                return ERR_UNKNOWN_OPCODE;
        }
    }
}

#endif