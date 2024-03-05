#ifndef VERSE_STACK_VM_H_
#define VERSE_STACK_VM_H_

#define STACK_MAX 256

#include <stdint.h>
#include <stdio.h>

/*
 * TODO Fail gracefully if we try to pop a value that doesn't exist off the
 * TODO stack.
 */

/*
 * This struct encapsulates the state of our virtual machine.
 */
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

/*
 * Define possible termination statuses for our VM.
 */
typedef enum result {
    SUCCESS,
    ERR_DIV_ZERO,
    ERR_UNKNOWN_OPCODE
} result;

void reset_vm() {
    printf("Resetting VM state\n");
    vm.stack_top = vm.stack;
}

/*
 * Define helper functions for manipulating the stack.
 */

void stack_push(uint64_t val) {
    printf("Pushing %llu onto the stack\n", val);
    *vm.stack_top = val;
    vm.stack_top++;
}

uint64_t stack_pop() {
    vm.stack_top--;
    uint64_t val = *vm.stack_top;
    printf("Popped %llu off the stack\n", val);
    return val;
}

void do_push_imm() {
    uint8_t arg = *vm.instruction_ptr++;
    stack_push(arg);
}

void do_add() {
    uint64_t op2 = stack_pop();
    uint64_t op1 = stack_pop();
    stack_push(op1 + op2);
}

void do_sub() {
    uint64_t op2 = stack_pop();
    uint64_t op1 = stack_pop();
    stack_push(op1 - op2);
}

void do_mul() {
    uint64_t op2 = stack_pop();
    uint64_t op1 = stack_pop();
    stack_push(op1 * op2);
}

void do_div() {
    uint64_t op2 = stack_pop();
    uint64_t op1 = stack_pop();

    /*
     * Check for division by zero.
     *
     * TODO How should we fail gracefully here?
     */
    if (op2 == 0) {
        printf("Division by zero encountered\n");
        exit(EXIT_FAILURE);
    }
    stack_push(op1 / op2);
}

void do_pop_res() {
    vm.res = stack_pop();
}

/*
 * I want to test how much function calls slow down our dispatch loop.
 */
result interpret_function_dispatch(uint8_t *bytecode) {
    vm.instruction_ptr = bytecode;
    for (;;) {
        uint8_t instruction = *vm.instruction_ptr++;
        switch (instruction) {
            case PUSH_IMM: {
                printf("Doing PUSH_IMM\n");
                do_push_imm();
                printf("Done with PUSH_IMM\n");
                break;
            }
            case ADD: {
                printf("Doing ADD\n");
                do_add();
                printf("Done with ADD\n");
                break;
            }
            case SUB: {
                printf("Doing SUB\n");
                do_sub();
                printf("Done with SUB\n");
                break;
            }
            case MUL: {
                printf("Doing MUL\n");
                do_mul();
                printf("Done with MUL\n");
                break;
            }
            case DIV: {
                printf("Doing DIV\n");
                do_div();
                printf("Done with DIV\n");
                break;
            }
            case POP_RES: {
                printf("Doing POP_RES\n");
                do_pop_res();
                printf("Done with POP_RES\n");
                break;
            }
            case DONE: {
                printf("Done!\n");
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

/*
 * Interpreter loop without any function calls.
 */
result interpret_inline(uint8_t *bytecode) {
    printf("Inside inline dispatch\n");
    vm.instruction_ptr = bytecode;
    for (;;) {
        uint8_t instruction = *vm.instruction_ptr++;
        switch (instruction) {
            case PUSH_IMM: {
                uint8_t arg = *vm.instruction_ptr++;
                *vm.stack_top = arg;
                vm.stack_top++;
                break;
            }
            case ADD: {
                vm.stack_top--;
                uint64_t op2 = *vm.stack_top;
                vm.stack_top--;
                uint64_t op1 = *vm.stack_top;
                *vm.stack_top = op1 + op2;
                vm.stack_top++;
                break;
            }
            case SUB: {
                vm.stack_top--;
                uint64_t op2 = *vm.stack_top;
                vm.stack_top--;
                uint64_t op1 = *vm.stack_top;
                *vm.stack_top = op1 - op2;
                vm.stack_top++;
                break;
            }
            case MUL: {
                vm.stack_top--;
                uint64_t op2 = *vm.stack_top;
                vm.stack_top--;
                uint64_t op1 = *vm.stack_top;
                *vm.stack_top = op1 * op2;
                vm.stack_top++;
                break;
            }
            case DIV: {
                vm.stack_top--;
                uint64_t op2 = *vm.stack_top;
                vm.stack_top--;
                uint64_t op1 = *vm.stack_top;

                /*
                 * Check for division by zero.
                 */
                if (op2 == 0) {
                    return ERR_DIV_ZERO;
                }
                *vm.stack_top = op1 / op2;
                vm.stack_top++;
                break;
            }
            case POP_RES: {

                /*
                 * Pop and set that value as the return val.
                 */
                vm.stack_top--;
                vm.res = *vm.stack_top;
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