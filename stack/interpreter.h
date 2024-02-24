#ifndef VERSE_STACK_INTERPRETER_H_
#define VERSE_STACK_INTERPRETER_H_

#define STACK_MAX 256

#include <stdint.h>
#include <stdio.h>

struct {
    uint8_t *instruction_ptr;
    uint64_t stack[STACK_MAX];
    uint64_t *stack_top;
    uint64_t res;
} vm;

typedef enum {
    PUSH_IMM,
    ADD,
    SUB,
    MUL,
    DIV,
    POP_RES,
    DONE
} opcode;

typedef enum result {
    SUCCESS,
    ERR_DIV_ZERO,
    ERR_UNKNOWN_OPCODE
} result;

void reset_vm() {
    vm.stack_top = vm.stack;
}

void stack_push(uint64_t val) {
    *vm.stack_top = val;
    vm.stack_top++;
}

uint64_t stack_pop() {
    vm.stack_top--;
    return *vm.stack_top;
}

result interpret(uint8_t *bytecode) {
    vm.instruction_ptr = bytecode;
    for (;;) {
        uint8_t instruction = *vm.instruction_ptr++;
        switch(instruction) {
            case PUSH_IMM: {

                /*
                 * Get the immediate value and push it onto the stack.
                 */
                uint8_t arg = *vm.instruction_ptr++;
                stack_push(arg);
                break;
            }
            case ADD: {
                uint64_t op2 = stack_pop();
                uint64_t op1 = stack_pop();
                stack_push(op1 + op2);
                break;
            }
            case SUB: {
                uint64_t op2 = stack_pop();
                uint64_t op1 = stack_pop();
                stack_push(op1 - op2);
                break;
            }
            case MUL: {
                uint64_t op2 = stack_pop();
                uint64_t op1 = stack_pop();
                stack_push(op1 * op2);
                break;
            }
            case DIV: {
                uint64_t op2 = stack_pop();
                uint64_t op1 = stack_pop();

                /*
                 * Check for division by zero.
                 */
                if (op2 == 0) {
                    return ERR_DIV_ZERO;
                }
                stack_push(op1 / op2);
                break;
            }
            case POP_RES: {

                /*
                 * Pop and set that value as the return val.
                 */
                uint64_t res = stack_pop();
                vm.res = res;
                break;
            }
            case DONE: {
                return SUCCESS;
            }
            default: {
                fprintf(stderr, "Unknown opcode\n");
                fflush(stderr);
                return ERR_UNKNOWN_OPCODE;
            }
        }
    }
}

#endif