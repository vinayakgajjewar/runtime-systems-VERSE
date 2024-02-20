#ifndef VERSE_STACK_INTERPRETER_H_
#define VERSE_STACK_INTERPRETER_H_

#define STACK_MAX 256

#include <stdint.h>

struct {
    uint8_t *instruction_ptr;
    uint64_t stack[STACK_MAX];
    uint64_t *stack_top;
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

#endif