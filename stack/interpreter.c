#include <assert.h>
#include <inttypes.h>

#include "interpreter.h"

int main() {
    reset_vm();
    uint8_t code[] = {
            PUSH_IMM,
            10,
            PUSH_IMM,
            5,
            ADD,
            POP_RES,
            DONE
    };
    result r = interpret(code);
    assert(r == SUCCESS);
    printf("Result: %" PRIu64 "\n", vm.res);
}