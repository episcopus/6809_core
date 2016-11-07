#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>

#include "functions.h"
#include "inherent_opcode_test.h"

extern struct cpu_state e_cpu_context;

int test_setup(void **state) {
    (void) state; /* unused */

    core_init();
    return 0;
}

int test_teardown(void **state) {
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
    assert_int_equal(e_cpu_context.pc, USER_SPACE_ROOT);
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
    assert_int_equal(e_cpu_context.cycle_count, 0);
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

static void test_load_memory(void **state) {
    (void) state; /* unused */

    uint8 test_bytes[] = {
        0x12, 0x34
    };

    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, test_bytes, 2}
    };

    load_memory(test_memory, 1);

    assert_int_equal(e_cpu_context.memory[USER_SPACE_ROOT], 0x12);
    assert_int_equal(e_cpu_context.memory[USER_SPACE_ROOT + 1], 0x34);
}

static void test_load_memory_too_big(void **state) {
    (void) state; /* unused */

    uint8 test_bytes[] = {
        0x12, 0x34
    };

    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT + 0xA000, test_bytes, MEMORY_SIZE / 2 }
    };

    expect_assert_failure(load_memory(test_memory, 1));
}

static void test_load_memory_too_far(void **state) {
    (void) state; /* unused */

    uint8 test_bytes[] = {
        0x12, 0x34
    };

    struct mem_loader_def test_memory[] = {
        { MEMORY_SIZE - 1, test_bytes, 2 }
    };

    expect_assert_failure(load_memory(test_memory, 1));
}

/* Run a single NOP instruction which should yield 2 cycles */
static void run_cycles_test(void **state) {
    uint8 code_bytes[] = {
        0x12
    };

    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1}
    };

    load_memory(test_memory, 1);

    uint32 completed_cycles = run_cycles(1);

    assert_int_equal(completed_cycles, 2);
    assert_int_equal(e_cpu_context.cycle_count, 2);
}

/* Run two NOP instructions which should yield 4 cycles */
static void run_cycles_multiple_test(void **state) {
    uint8 code_bytes[] = {
        0x12, 0x12
    };

    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2}
    };

    load_memory(test_memory, 1);

    uint32 completed_cycles = run_cycles(4);

    assert_int_equal(completed_cycles, 4);
    assert_int_equal(e_cpu_context.cycle_count, 4);
}

/* 0x1 on the 6809 is an invalid exception and we'll use it for this NOTIMPL
   assert test */
static void run_cycles_notimpl_test(void **state) {
    uint8 code_bytes[] = {
        0x1
    };

    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1}
    };

    load_memory(test_memory, 1);

    expect_assert_failure(run_cycles(1));
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
        cmocka_unit_test_setup_teardown(test_load_memory, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(test_load_memory_too_big, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(test_load_memory_too_far, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(run_cycles_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(run_cycles_multiple_test, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(run_cycles_notimpl_test, test_setup, test_teardown)
    };

    return cmocka_run_group_tests(tests, NULL, NULL) +
        cmocka_run_group_tests(inherent_tests, NULL, NULL);
}
