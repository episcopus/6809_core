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
void adda_immediate_test(void **state);
void addb_immediate_test(void **state);
void adda_immediate_carry_test(void **state);
void addb_immediate_carry_test(void **state);
void adda_immediate_overflow_test(void **state);
void addb_immediate_overflow_test(void **state);
void adda_immediate_carry_flag_test(void **state);
void addb_immediate_carry_flag_test(void **state);
void adda_immediate_half_carry_test(void **state);
void addb_immediate_half_carry_test(void **state);
void adda_immediate_zero_test(void **state);
void addb_immediate_zero_test(void **state);
void addd_immediate_test(void **state);
void addd_immediate_overflow_test(void **state);
void addd_immediate_carry_flag_test(void **state);
void addd_immediate_zero_test(void **state);
void anda_immediate_test(void **state);
void andb_immediate_test(void **state);
void anda_immediate_zero_test(void **state);
void andb_immediate_zero_test(void **state);
void andcc_immediate_test(void **state);
void andcc_immediate_e_test(void **state);

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
    cmocka_unit_test_setup_teardown(adcb_immediate_zero_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(adda_immediate_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(addb_immediate_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(adda_immediate_carry_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(addb_immediate_carry_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(adda_immediate_overflow_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(addb_immediate_overflow_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(adda_immediate_carry_flag_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(addb_immediate_carry_flag_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(adda_immediate_half_carry_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(addb_immediate_half_carry_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(adda_immediate_zero_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(addb_immediate_zero_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(addd_immediate_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(addd_immediate_overflow_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(addd_immediate_carry_flag_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(addd_immediate_zero_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(anda_immediate_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(andb_immediate_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(anda_immediate_zero_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(andb_immediate_zero_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(andcc_immediate_test, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(andcc_immediate_e_test, test_setup, test_teardown)
}
;
