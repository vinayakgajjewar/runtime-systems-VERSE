#include <assert.h>
#include <stdio.h>

#include "interpreter.h"

int main() {

    /*
     * Test encoding macros.
     */
    assert(ENCODE_OP_REG_IMM(LOAD_IMM, 0, 6) == 0x0006);
    assert(ENCODE_OP_REG_IMM(LOAD_IMM, 1, 2) == 0x0102);
    assert(ENCODE_OP_REGS(DIV, 0, 1, 2) == 0x4012);
    assert(ENCODE_OP_REG(MOV_RES, 2) == 0x5200);
    assert(ENCODE_OP(DONE) == 0x6000);

    /*
     * Test decoding macros.
     * 0x4012 = 0100 0000 00001 0010
     *        = DIV  R0   R1    R2
     */
    assert(DECODE_OP(0x4012) == DIV);
    assert(DECODE_R0(0x4012) == 0);
    assert(DECODE_R1(0x4012) == 1);
    assert(DECODE_R2(0x4012) == 2);

    printf("All tests passed\n");
    fflush(stdout);
}