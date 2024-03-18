#ifndef VERSE_STACK_VM_H_
#define VERSE_STACK_VM_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Our runtime stack has 256 slots.
 */
#define STACK_MAX 256

/*
 * We'll use this macro for direct threading dispatch. Bump the instruction
 * pointer and go to the next instruction label.
 */
#define go_next vm.instruction_ptr++; goto *table[*vm.instruction_ptr]

/*
 * TODO Fail gracefully if we try to pop a value that doesn't exist off the
 * TODO stack.
 */

/*
 * This struct encapsulates the state of our virtual machine.
 */
struct {

    /*
     * Pointer to the instruction we're currently executing.
     */
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
    JIF,
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

/*
 * This doesn't do much right now.
 */
void reset_vm() {
    printf("Resetting VM state\n");
    vm.stack_top = vm.stack;
}

/*
 * Define helper functions for manipulating the stack.
 */

void stack_push(uint64_t val) {
    *vm.stack_top = val;
    vm.stack_top++;
}

uint64_t stack_pop() {
    vm.stack_top--;
    uint64_t val = *vm.stack_top;
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

void do_jif(uint8_t *bytecode) {
    if (*(vm.stack_top - 1) != 0) {
        uint8_t loc = *vm.instruction_ptr++;
        vm.instruction_ptr = bytecode + loc - 1;
    } else {
        vm.instruction_ptr++;
    }
}

void do_pop_res() {
    vm.result = stack_pop();
}

/*
 * Direct threading dispatch using computed GOTO statements.
 */
result interpret_threaded_dispatch(uint8_t *bytecode) {
    vm.instruction_ptr = bytecode;

    /*
     * e TODO
     */
    uint64_t op1;
    uint64_t op2;
    uint64_t imm;

    /*
     * This is our lookup table of GOTO labels for each instruction. We can use
     * the VM's instruction pointer to index into this table.
     */
    void *table[] = {
            &&push_imm_label,
            &&add_label,
            &&sub_label,
            &&mul_label,
            &&div_label,
            &&and_label,
            &&or_label,
            &&xor_label,
            &&not_label,
            &&lshift_label,
            &&rshift_label,
            &&jif_label,
            &&pop_res_label,
            &&done_label
    };

    /*
     * Get the ball rolling.
     */
    goto *table[*vm.instruction_ptr];

    /*
     * GOTO labels for each instruction.
     */

    push_imm_label:
    //do_push_imm();
    imm = *(vm.instruction_ptr + 1);
    vm.instruction_ptr++;
    *vm.stack_top = imm;
    vm.stack_top++;
    //vm.instruction_ptr++;
    go_next;

    add_label:
    //do_add();
    vm.stack_top--;
    op2 = *vm.stack_top;
    vm.stack_top--;
    op1 = *vm.stack_top;
    *vm.stack_top = op1 + op2;
    vm.stack_top++;
    //vm.instruction_ptr++;
    go_next;

    sub_label:
    //do_sub();
    vm.stack_top--;
    op2 = *vm.stack_top;
    vm.stack_top--;
    op1 = *vm.stack_top;
    *vm.stack_top = op1 - op2;
    vm.stack_top++;
    go_next;

    mul_label:
    //do_mul();
    vm.stack_top--;
    op2 = *vm.stack_top;
    vm.stack_top--;
    op1 = *vm.stack_top;
    *vm.stack_top = op1 * op2;
    vm.stack_top++;
    go_next;

    div_label:
    //do_div();
    vm.stack_top--;
    op2 = *vm.stack_top;
    vm.stack_top--;
    op1 = *vm.stack_top;
    if (op2 == 0) {
        return ERR_DIV_ZERO;
    }
    *vm.stack_top = op1 / op2;
    vm.stack_top++;
    go_next;

    and_label:
    //do_and();
    vm.stack_top--;
    op2 = *vm.stack_top;
    vm.stack_top--;
    op1 = *vm.stack_top;
    *vm.stack_top = op1 & op2;
    vm.stack_top++;
    go_next;

    or_label:
    //do_or();
    vm.stack_top--;
    op2 = *vm.stack_top;
    vm.stack_top--;
    op1 = *vm.stack_top;
    *vm.stack_top = op1 | op2;
    vm.stack_top++;
    go_next;

    xor_label:
    //do_xor();
    vm.stack_top--;
    op2 = *vm.stack_top;
    vm.stack_top--;
    *vm.stack_top = op1 ^ op2;
    vm.stack_top++;
    go_next;

    not_label:
    // do_not();
    vm.stack_top--;
    *vm.stack_top = ~(*vm.stack_top);
    vm.stack_top++;
    go_next;

    lshift_label:
    //do_lshift();
    vm.stack_top--;
    op2 = *vm.stack_top;
    vm.stack_top--;
    op1 = *vm.stack_top;
    *vm.stack_top = op1 << op2;
    vm.stack_top++;
    go_next;

    rshift_label:
    //do_rshift();
    vm.stack_top--;
    op2 = *vm.stack_top;
    vm.stack_top--;
    op1 = *vm.stack_top;
    *vm.stack_top = op1 >> op2;
    vm.stack_top++;
    go_next;

    jif_label:
    //do_jif(bytecode);
    if (*(vm.stack_top - 1) != 0) {

        /*
         * This is ugly and I should rewrite it but it works.
         * TODO
         */
        vm.instruction_ptr = bytecode + (*(vm.instruction_ptr + 1)) - 2;
    } else {
        vm.instruction_ptr++;
    }
    go_next;

    pop_res_label:
    //do_pop_res();
    vm.stack_top--;
    vm.result = *vm.stack_top;
    //vm.instruction_ptr++;
    go_next;

    done_label:
    printf("Done!\n");
    return SUCCESS;
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
                do_push_imm();
                break;
            }
            case ADD: {
                do_add();
                break;
            }
            case SUB: {
                do_sub();
                break;
            }
            case MUL: {
                do_mul();
                break;
            }
            case DIV: {
                do_div();
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
            case JIF: {
                do_jif(bytecode);
                break;
            }
            case POP_RES: {
                do_pop_res();
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
    vm.instruction_ptr = bytecode;
    for (;;) {
        uint8_t instruction = *vm.instruction_ptr++;
        switch (instruction) {
            case PUSH_IMM: {
                *vm.stack_top = *vm.instruction_ptr++;
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
                printf("Doing SUB\n");
                vm.stack_top--;
                uint64_t op2 = *vm.stack_top;
                vm.stack_top--;
                uint64_t op1 = *vm.stack_top;
                printf("Doing %llu - %llu\n", op1, op2);
                *vm.stack_top = op1 - op2;
                vm.stack_top++;
                break;
            }
            case MUL: {
                printf("Doing MUL\n");
                vm.stack_top--;
                uint64_t op2 = *vm.stack_top;
                vm.stack_top--;
                uint64_t op1 = *vm.stack_top;
                *vm.stack_top = op1 * op2;
                vm.stack_top++;
                break;
            }
            case DIV: {
                printf("Doing DIV\n");
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
                printf("Doing AND\n");
                vm.stack_top--;
                uint64_t op2 = *vm.stack_top;
                vm.stack_top--;
                uint64_t op1 = *vm.stack_top;
                *vm.stack_top = op1 & op2;
                vm.stack_top++;
                break;
            }
            case OR: {
                printf("Doing OR\n");
                vm.stack_top--;
                uint64_t op2 = *vm.stack_top;
                vm.stack_top--;
                uint64_t op1 = *vm.stack_top;
                *vm.stack_top = op1 | op2;
                vm.stack_top++;
                break;
            }
            case XOR: {
                printf("Doing XOR\n");
                vm.stack_top--;
                uint64_t op2 = *vm.stack_top;
                vm.stack_top--;
                uint64_t op1 = *vm.stack_top;
                *vm.stack_top = op1 ^ op2;
                vm.stack_top++;
                break;
            }
            case NOT: {
                printf("Doing NOT\n");
                vm.stack_top--;
                *vm.stack_top = ~(*vm.stack_top);
                vm.stack_top++;
                break;
            }
            case LSHIFT: {
                printf("Doing LSHIFT\n");
                vm.stack_top--;
                uint64_t op2 = *vm.stack_top;
                vm.stack_top--;
                uint64_t op1 = *vm.stack_top;
                *vm.stack_top = op1 << op2;
                vm.stack_top++;
                break;
            }
            case RSHIFT: {
                printf("Doing RSHIFT\n");
                vm.stack_top--;
                uint64_t op2 = *vm.stack_top;
                vm.stack_top--;
                uint64_t op1 = *vm.stack_top;
                *vm.stack_top = op1 >> op2;
                vm.stack_top++;
                break;
            }
            case JIF: {
                printf("Doing JIF\n");

                /*
                 * Peek at the top of the stack to determine if we jump or not.
                 */
                if (*(vm.stack_top - 1) != 0) {
                    printf("Jumping...\n");
                    uint8_t loc = *vm.instruction_ptr++;
                    printf("loc=%d\n", loc);

                    /*
                     * We have to subtract 1 to account for the increment of the
                     * instruction pointer at the beginning of the loop.
                     */
                    vm.instruction_ptr = bytecode + loc - 1;
                } else {
                    printf("Falling through...\n");
                    vm.instruction_ptr++;
                }
                break;
            }
            case POP_RES: {
                printf("Doing POP_RES\n");

                /*
                 * Pop and set that value as the return val.
                 */
                vm.stack_top--;
                vm.result = *vm.stack_top;
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

#endif