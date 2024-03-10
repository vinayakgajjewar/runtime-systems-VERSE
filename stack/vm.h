#ifndef VERSE_STACK_VM_H_
#define VERSE_STACK_VM_H_

/*
 * Our runtime stack has 256 slots.
 */
#define STACK_MAX 256

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * TODO Fail gracefully if we try to pop a value that doesn't exist off the
 * TODO stack.
 */

/*
 * This struct encapsulates the state of our virtual machine.
 */
struct {
    uint8_t *instruction_ptr;

    /*
     * Define our runtime stack.
     */
    uint64_t stack[STACK_MAX];

    /*
     * This points to the first free slot of the runtime stack.
     */
    uint64_t *stack_top;

    /*
     * This variable will hold the result of our execution.
     */
    uint64_t result;
} vm;

/*
 * Define all the opcodes we recognize.
 *
 * TODO Need to synchronize this with the assembler.
 */
typedef enum {
    PUSH_IMM,
    ADD,
    SUB,
    MUL,
    DIV,
    AND,
    OR,
    XOR,
    NOT,
    LSHIFT,
    RSHIFT,
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

void do_and() {
    uint64_t op2 = stack_pop();
    uint64_t op1 = stack_pop();
    stack_push(op1 & op2);
}

void do_or() {
    uint64_t op2 = stack_pop();
    uint64_t op1 = stack_pop();
    stack_push(op1 | op2);
}

void do_xor() {
    uint64_t op2 = stack_pop();
    uint64_t op1 = stack_pop();
    stack_push(op1 ^ op2);
}

/*
 * please do not the cat
 */
void do_not() {
    stack_push(~stack_pop());
}

void do_lshift() {
    uint64_t op2 = stack_pop();
    uint64_t op1 = stack_pop();
    stack_push(op1 << op2);
}

void do_rshift() {
    uint64_t op2 = stack_pop();
    uint64_t op1 = stack_pop();
    stack_push(op1 >> op2);
}

void do_pop_res() {
    vm.result = stack_pop();
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
            case AND: {
                do_and();
                break;
            }
            case OR: {
                do_or();
                break;
            }
            case XOR: {
                do_xor();
                break;
            }
            case NOT: {
                do_not();
                break;
            }
            case LSHIFT: {
                do_lshift();
                break;
            }
            case RSHIFT: {
                do_rshift();
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
            case AND: {
                vm.stack_top--;
                uint64_t op2 = *vm.stack_top;
                vm.stack_top--;
                uint64_t op1 = *vm.stack_top;
                *vm.stack_top = op1 & op2;
                vm.stack_top++;
                break;
            }
            case OR: {
                vm.stack_top--;
                uint64_t op2 = *vm.stack_top;
                vm.stack_top--;
                uint64_t op1 = *vm.stack_top;
                *vm.stack_top = op1 | op2;
                vm.stack_top++;
                break;
            }
            case XOR: {
                vm.stack_top--;
                uint64_t op2 = *vm.stack_top;
                vm.stack_top--;
                uint64_t op1 = *vm.stack_top;
                *vm.stack_top = op1 ^ op2;
                vm.stack_top++;
                break;
            }
            case NOT: {
                vm.stack_top--;
                *vm.stack_top = ~(*vm.stack_top);
                vm.stack_top++;
                break;
            }
            case LSHIFT: {
                vm.stack_top--;
                uint64_t op2 = *vm.stack_top;
                vm.stack_top--;
                uint64_t op1 = *vm.stack_top;
                *vm.stack_top = op1 << op2;
                vm.stack_top++;
                break;
            }
            case RSHIFT: {
                vm.stack_top--;
                uint64_t op2 = *vm.stack_top;
                vm.stack_top--;
                uint64_t op1 = *vm.stack_top;
                *vm.stack_top = op1 >> op2;
                vm.stack_top++;
                break;
            }
            case POP_RES: {

                /*
                 * Pop and set that value as the return val.
                 */
                vm.stack_top--;
                vm.result = *vm.stack_top;
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