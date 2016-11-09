#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

extern int test_setup(void ** state);
extern int test_teardown(void **state);

void adca_immediate_test(void **state);
void adcb_immediate_test(void **state);
void adca_immediate_carry_test(void **state);
void adcb_immediate_carry_test(void **state);
void adca_immediate_overflow_test(void **state);
void adcb_immediate_overflow_test(void **state);
void adca_immediate_carry_flag_test(void **state);
void adcb_immediate_carry_flag_test(void **state);
void adca_immediate_half_carry_test(void **state);
void adcb_immediate_half_carry_test(void **state);
void adca_immediate_zero_test(void **state);
void adcb_immediate_zero_test(void **state);

const struct CMUnitTest immediate_tests[] = {
    cmocka_unit_test_setup_teardown(adca_immediate_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(adcb_immediate_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(adca_immediate_carry_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(adcb_immediate_carry_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(adca_immediate_overflow_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(adcb_immediate_overflow_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(adca_immediate_carry_flag_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(adcb_immediate_carry_flag_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(adca_immediate_half_carry_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(adcb_immediate_half_carry_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(adca_immediate_zero_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(adcb_immediate_zero_test, test_setup, test_teardown)
};
