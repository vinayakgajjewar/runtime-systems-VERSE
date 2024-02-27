#ifndef VERSE_STACK_VM_H_
#define VERSE_STACK_VM_H_

#define STACK_MAX 256

#define DEBUG(x) printf("(" + __FILE_NAME__ + ":" + __LINE__ + ") " + x + "\n")

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
    printf("Resetting VM state\n");
    vm.stack_top = vm.stack;
}

void stack_push(uint64_t val) {
    printf("Pushing %" PRIu64 " onto the stack\n", val);
    *vm.stack_top = val;
    vm.stack_top++;
}

uint64_t stack_pop() {
    vm.stack_top--;
    uint64_t val = *vm.stack_top;
    printf("Popped %" PRIu64 " off the stack \n", val);
    return val;
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