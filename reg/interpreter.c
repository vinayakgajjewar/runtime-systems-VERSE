#include <assert.h>
#include <time.h>

#include "interpreter.h"

int main() {

    /*
     * Start the clock.
     */
    clock_t begin = clock();

    reset_vm();
    uint16_t code[] = {
            ENCODE_OP_REG_IMM(LOAD_IMM, 3, 5),
            ENCODE_OP_REG(MOV_RES, 3),
            ENCODE_OP(DONE)
    };
    result res = interpret(code);
    assert(res == SUCCESS);
    assert(vm.result == 5);

    /*
     * Stop the clock.
     */
    clock_t end = clock();
    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
    printf("Execution took %lf seconds\n", time_spent);
}