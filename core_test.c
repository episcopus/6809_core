#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "functions.h"
#include "functions_test.h"

extern struct cpu_state e_cpu_context;

static int test_setup(void **state) {
    (void) state; /* unused */

    core_init();
    return 0;
}

static void core_init_test(void **state) {
    (void) state; /* unused */

    assert_int_equal(e_cpu_context.x, 0);
    assert_int_equal(e_cpu_context.y, 0);
    assert_int_equal(e_cpu_context.u, 0);
    assert_int_equal(e_cpu_context.s, 0);
    assert_int_equal(e_cpu_context.pc, 0);
    assert_int_equal(e_cpu_context.d.d, 0);
    assert_int_equal(e_cpu_context.dp, 0);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(core_init_test, test_setup),
        cmocka_unit_test_setup(nop_test, test_setup),
        cmocka_unit_test_setup(abx_test, test_setup),
        cmocka_unit_test_setup(abx_test_add, test_setup),
        cmocka_unit_test_setup(abx_test_unsigned, test_setup)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
