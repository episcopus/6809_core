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

static int test_teardown(void **state) {
    (void) state; /* unused */

    core_destroy();
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
        cmocka_unit_test_setup_teardown(core_init_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(test_e_flag, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(test_f_flag, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(test_h_flag, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(test_i_flag, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(test_n_flag, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(test_z_flag, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(test_v_flag, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(test_c_flag, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(test_all_flags, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(nop_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(abx_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(abx_test_add, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(abx_test_unsigned, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(asla_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(asla_flags_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(aslb_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(aslb_flags_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(asra_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(asra_flags_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(asra_negative_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(asrb_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(asrb_flags_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(asrb_negative_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(clra_flags_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(clrb_flags_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(coma_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(comb_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(coma_zero_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(comb_zero_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(daa_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(daa_not_adjusted_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(deca_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(decb_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(deca_zero_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(decb_zero_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(deca_negative_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(decb_negative_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(inca_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(incb_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(inca_overflow_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(incb_overflow_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(inca_zero_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(incb_zero_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(lsra_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(lsra_flags_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(lsra_negative_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(lsrb_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(lsrb_flags_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(lsrb_negative_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(mul_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(mul_signed_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(mul_zero_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(nega_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(negb_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(nega_minusone_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(negb_minusone_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(rola_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(rolb_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(rola_carry_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(rolb_carry_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(rola_rotate_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(rolb_rotate_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(rora_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(rorb_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(rora_carry_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(rorb_carry_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(rora_rotate_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(rorb_rotate_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(sex_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(sex_negative_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(sex_zero_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(tsta_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(tstb_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(tsta_negative_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(tstb_negative_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(tsta_zero_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(tstb_zero_test, test_setup, test_teardown)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
