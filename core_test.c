#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>

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
    assert_int_equal(e_cpu_context.cc.e, 0);
    assert_int_equal(e_cpu_context.cc.f, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.i, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
}

static void test_e_flag(void **state) {
    (void) state; /* unused */

    e_cpu_context.cc.e = 1;
    /* printf("The cc register is 0x%X\n", (unsigned int) *((unsigned int*) &e_cpu_context.cc)); */
    unsigned int raw_cc = (unsigned int) *((unsigned int*) &e_cpu_context.cc);
    assert_int_equal(raw_cc, 0x80);
}

static void test_f_flag(void **state) {
    (void) state; /* unused */

    e_cpu_context.cc.f = 1;
    /* printf("The cc register is 0x%X\n", (unsigned int) *((unsigned int*) &e_cpu_context.cc)); */
    unsigned int raw_cc = (unsigned int) *((unsigned int*) &e_cpu_context.cc);
    assert_int_equal(raw_cc, 0x40);
}

static void test_h_flag(void **state) {
    (void) state; /* unused */

    e_cpu_context.cc.h = 1;
    /* printf("The cc register is 0x%X\n", (unsigned int) *((unsigned int*) &e_cpu_context.cc)); */
    unsigned int raw_cc = (unsigned int) *((unsigned int*) &e_cpu_context.cc);
    assert_int_equal(raw_cc, 0x20);
}

static void test_i_flag(void **state) {
    (void) state; /* unused */

    e_cpu_context.cc.i = 1;
    /* printf("The cc register is 0x%X\n", (unsigned int) *((unsigned int*) &e_cpu_context.cc)); */
    unsigned int raw_cc = (unsigned int) *((unsigned int*) &e_cpu_context.cc);
    assert_int_equal(raw_cc, 0x10);
}

static void test_n_flag(void **state) {
    (void) state; /* unused */

    e_cpu_context.cc.n = 1;
    /* printf("The cc register is 0x%X\n", (unsigned int) *((unsigned int*) &e_cpu_context.cc)); */
    unsigned int raw_cc = (unsigned int) *((unsigned int*) &e_cpu_context.cc);
    assert_int_equal(raw_cc, 0x8);
}

static void test_z_flag(void **state) {
    (void) state; /* unused */

    e_cpu_context.cc.z = 1;
    /* printf("The cc register is 0x%X\n", (unsigned int) *((unsigned int*) &e_cpu_context.cc)); */
    unsigned int raw_cc = (unsigned int) *((unsigned int*) &e_cpu_context.cc);
    assert_int_equal(raw_cc, 0x4);
}

static void test_v_flag(void **state) {
    (void) state; /* unused */

    e_cpu_context.cc.v = 1;
    /* printf("The cc register is 0x%X\n", (unsigned int) *((unsigned int*) &e_cpu_context.cc)); */
    unsigned int raw_cc = (unsigned int) *((unsigned int*) &e_cpu_context.cc);
    assert_int_equal(raw_cc, 0x2);
}

static void test_c_flag(void **state) {
    (void) state; /* unused */

    e_cpu_context.cc.c = 1;
    /* printf("The cc register is 0x%X\n", (unsigned int) *((unsigned int*) &e_cpu_context.cc)); */
    unsigned int raw_cc = (unsigned int) *((unsigned int*) &e_cpu_context.cc);
    assert_int_equal(raw_cc, 0x1);
}

static void test_all_flags(void **state) {
    (void) state; /* unused */

    unsigned int raw_cc = (unsigned int) *((unsigned int*) &e_cpu_context.cc);
    assert_int_equal(raw_cc, 0);

    e_cpu_context.cc.c = 1;
    e_cpu_context.cc.v = 1;
    e_cpu_context.cc.z = 1;
    e_cpu_context.cc.n = 1;
    e_cpu_context.cc.i = 1;
    e_cpu_context.cc.h = 1;
    e_cpu_context.cc.f = 1;
    e_cpu_context.cc.e = 1;

    raw_cc = (unsigned int) *((unsigned int*) &e_cpu_context.cc);
    /* printf("The cc register is 0x%X\n", (unsigned int) *((unsigned int*) &e_cpu_context.cc)); */
    assert_int_equal(raw_cc, 0xFF);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(core_init_test, test_setup),
        cmocka_unit_test_setup(test_e_flag, test_setup),
        cmocka_unit_test_setup(test_f_flag, test_setup),
        cmocka_unit_test_setup(test_h_flag, test_setup),
        cmocka_unit_test_setup(test_i_flag, test_setup),
        cmocka_unit_test_setup(test_n_flag, test_setup),
        cmocka_unit_test_setup(test_z_flag, test_setup),
        cmocka_unit_test_setup(test_v_flag, test_setup),
        cmocka_unit_test_setup(test_c_flag, test_setup),
        cmocka_unit_test_setup(test_all_flags, test_setup),
        cmocka_unit_test_setup(nop_test, test_setup),
        cmocka_unit_test_setup(abx_test, test_setup),
        cmocka_unit_test_setup(abx_test_add, test_setup),
        cmocka_unit_test_setup(abx_test_unsigned, test_setup),
        cmocka_unit_test_setup(asla_test, test_setup),
        cmocka_unit_test_setup(asla_flags_test, test_setup),
        cmocka_unit_test_setup(aslb_test, test_setup),
        cmocka_unit_test_setup(aslb_flags_test, test_setup),
        cmocka_unit_test_setup(asra_test, test_setup),
        cmocka_unit_test_setup(asra_flags_test, test_setup),
        cmocka_unit_test_setup(asrb_test, test_setup),
        cmocka_unit_test_setup(asrb_flags_test, test_setup),
        cmocka_unit_test_setup(clra_flags_test, test_setup),
        cmocka_unit_test_setup(clrb_flags_test, test_setup),
        cmocka_unit_test_setup(coma_test, test_setup),
        cmocka_unit_test_setup(comb_test, test_setup),
        cmocka_unit_test_setup(coma_zero_test, test_setup),
        cmocka_unit_test_setup(comb_zero_test, test_setup),
        cmocka_unit_test_setup(daa_test, test_setup),
        cmocka_unit_test_setup(daa_not_adjusted_test, test_setup),
        cmocka_unit_test_setup(deca_test, test_setup),
        cmocka_unit_test_setup(decb_test, test_setup),
        cmocka_unit_test_setup(deca_zero_test, test_setup),
        cmocka_unit_test_setup(decb_zero_test, test_setup),
        cmocka_unit_test_setup(deca_negative_test, test_setup),
        cmocka_unit_test_setup(decb_negative_test, test_setup)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
