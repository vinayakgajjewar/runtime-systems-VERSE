#include <assert.h>

#include "interpreter.h"

int main() {
    reset_vm();
    uint16_t code[] = {
            ENCODE_OP_REG_IMM(LOAD_IMM, 3, 5),
            ENCODE_OP_REG(MOV_RES, 3),
            ENCODE_OP(DONE)
    };
    result res = interpret(code);
    assert(res == SUCCESS);
    assert(vm.result == 5);
}