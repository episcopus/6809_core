#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "functions.h"
#include "types.h"

extern struct cpu_state e_cpu_context;

static void core_init_test(void **state) {
    (void) state; /* unused */

    core_init();
    assert_int_equal(e_cpu_context.x, 0);
    assert_int_equal(e_cpu_context.y, 0);
    assert_int_equal(e_cpu_context.u, 0);
    assert_int_equal(e_cpu_context.s, 0);
    assert_int_equal(e_cpu_context.pc, 0);
    assert_int_equal(e_cpu_context.d.d, 0);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(core_init_test)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
