#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "functions.h"

extern struct cpu_state e_cpu_context;
extern struct opcode_def opcode_table[];

void nop_test(void **state) {
    (void) state; /* unused */

    core_init();
    int cycles = nop(0x12, REG_NONE, INHERENT);

    assert_int_equal(cycles, opcode_table[0x12].cycle_count);
}
