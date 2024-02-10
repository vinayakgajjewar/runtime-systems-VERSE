#include <assert.h>

#include "interpreter.h"

int main() {
    assert(ENCODE_OP_REG_IMM(LOAD_IMM, 0, 6) == 0x0006);
    assert(ENCODE_OP_REG_IMM(LOAD_IMM, 1, 2) == 0x0102);
    assert(ENCODE_OP_REGS(DIV, 0, 1, 2) == 0x4012);
    assert(ENCODE_OP_REG(MOV_RES, 2) == 0x5200);
    assert(ENCODE_OP(DONE) == 0x6000);
}