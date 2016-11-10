#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "functions.h"

extern struct cpu_state e_cpu_context;
extern struct opcode_def opcode_table[];

void adca_immediate_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;

    uint8 code_bytes[] = {
        OP_ADCA,
        0x2
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 1;

    int cycles = run_cycles(opcode_table[OP_ADCA].cycle_count * 1);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x3);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADCA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void adcb_immediate_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;

    uint8 code_bytes[] = {
        OP_ADCB,
        0x2
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 1;

    int cycles = run_cycles(opcode_table[OP_ADCB].cycle_count * 1);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x3);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADCB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void adca_immediate_carry_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;

    uint8 code_bytes[] = {
        OP_ADCA,
        0x2
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 1;
    e_cpu_context.cc.c = 1;

    int cycles = run_cycles(opcode_table[OP_ADCA].cycle_count * 1);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x4);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADCA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void adcb_immediate_carry_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;

    uint8 code_bytes[] = {
        OP_ADCB,
        0x2
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 1;
    e_cpu_context.cc.c = 1;

    int cycles = run_cycles(opcode_table[OP_ADCB].cycle_count * 1);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x4);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADCB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void adca_immediate_overflow_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;

    /* 0x7F is the maximum signed value that a byte can handle hence
       the sum will overflow. */
    uint8 code_bytes[] = {
        OP_ADCA,
        0x7F
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 0x7F;

    int cycles = run_cycles(opcode_table[OP_ADCA].cycle_count * 1);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFE);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_table[OP_ADCA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void adcb_immediate_overflow_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;

    /* 0x7F is the maximum signed value that a byte can handle hence
       the sum will overflow. */
    uint8 code_bytes[] = {
        OP_ADCB,
        0x7F
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 0x7F;

    int cycles = run_cycles(opcode_table[OP_ADCB].cycle_count * 1);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFE);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_table[OP_ADCB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void adca_immediate_carry_flag_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;

    /* 0xAA and 0xBB are both negative number and their sum will both
       overflow and set carry. */
    uint8 code_bytes[] = {
        OP_ADCA,
        0xAA
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 0xBB;

    int cycles = run_cycles(opcode_table[OP_ADCA].cycle_count * 1);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x65);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_table[OP_ADCA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void adcb_immediate_carry_flag_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;

    /* 0xAA and 0xBB are both negative number and their sum will both
       overflow and set carry. */
    uint8 code_bytes[] = {
        OP_ADCB,
        0xAA
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 0xBB;

    int cycles = run_cycles(opcode_table[OP_ADCB].cycle_count * 1);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x65);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_table[OP_ADCB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void adca_immediate_half_carry_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;

    /* 9 + 8 = 17 will overflow the capacity of the half-byte (15)
       and set half carry. */
    uint8 code_bytes[] = {
        OP_ADCA,
        0x8
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 0x9;

    int cycles = run_cycles(opcode_table[OP_ADCA].cycle_count * 1);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x11);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADCA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void adcb_immediate_half_carry_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;

    /* 9 + 8 = 17 will overflow the capacity of the half-byte (15)
       and set half carry. */
    uint8 code_bytes[] = {
        OP_ADCB,
        0x8
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 0x9;

    int cycles = run_cycles(opcode_table[OP_ADCB].cycle_count * 1);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x11);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADCB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void adca_immediate_zero_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;

    uint8 code_bytes[] = {
        OP_ADCA,
        0
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 0;

    int cycles = run_cycles(opcode_table[OP_ADCA].cycle_count * 1);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADCA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void adcb_immediate_zero_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;

    uint8 code_bytes[] = {
        OP_ADCB,
        0
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 0;

    int cycles = run_cycles(opcode_table[OP_ADCB].cycle_count * 1);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADCB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void adda_immediate_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;

    uint8 code_bytes[] = {
        OP_ADDA,
        0x2
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 1;

    int cycles = run_cycles(opcode_table[OP_ADDA].cycle_count * 1);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x3);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADDA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void addb_immediate_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;

    uint8 code_bytes[] = {
        OP_ADDB,
        0x2
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 1;

    int cycles = run_cycles(opcode_table[OP_ADDB].cycle_count * 1);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x3);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADDB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void adda_immediate_carry_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;

    uint8 code_bytes[] = {
        OP_ADDA,
        0x2
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 1;
    /* The carry flag shouldn't impact the calculation for ADD */
    e_cpu_context.cc.c = 1;

    int cycles = run_cycles(opcode_table[OP_ADDA].cycle_count * 1);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x3);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADDA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void addb_immediate_carry_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;

    uint8 code_bytes[] = {
        OP_ADDB,
        0x2
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 1;
    /* The carry flag shouldn't impact the calculation for ADD */
    e_cpu_context.cc.c = 1;

    int cycles = run_cycles(opcode_table[OP_ADDB].cycle_count * 1);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x3);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADDB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void adda_immediate_overflow_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;

    /* 0x7F is the maximum signed value that a byte can handle hence
       the sum will overflow. */
    uint8 code_bytes[] = {
        OP_ADDA,
        0x7F
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 0x7F;

    int cycles = run_cycles(opcode_table[OP_ADDA].cycle_count * 1);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFE);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_table[OP_ADDA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void addb_immediate_overflow_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;

    /* 0x7F is the maximum signed value that a byte can handle hence
       the sum will overflow. */
    uint8 code_bytes[] = {
        OP_ADDB,
        0x7F
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 0x7F;

    int cycles = run_cycles(opcode_table[OP_ADDB].cycle_count * 1);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFE);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_table[OP_ADDB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void adda_immediate_carry_flag_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;

    /* 0xAA and 0xBB are both negative number and their sum will both
       overflow and set carry. */
    uint8 code_bytes[] = {
        OP_ADDA,
        0xAA
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 0xBB;

    int cycles = run_cycles(opcode_table[OP_ADDA].cycle_count * 1);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x65);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_table[OP_ADDA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void addb_immediate_carry_flag_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;

    /* 0xAA and 0xBB are both negative number and their sum will both
       overflow and set carry. */
    uint8 code_bytes[] = {
        OP_ADDB,
        0xAA
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 0xBB;

    int cycles = run_cycles(opcode_table[OP_ADDB].cycle_count * 1);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x65);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_table[OP_ADDB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void adda_immediate_half_carry_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;

    /* 9 + 8 = 17 will overflow the capacity of the half-byte (15)
       and set half carry. */
    uint8 code_bytes[] = {
        OP_ADDA,
        0x8
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 0x9;

    int cycles = run_cycles(opcode_table[OP_ADDA].cycle_count * 1);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x11);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADDA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void addb_immediate_half_carry_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;

    /* 9 + 8 = 17 will overflow the capacity of the half-byte (15)
       and set half carry. */
    uint8 code_bytes[] = {
        OP_ADDB,
        0x8
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 0x9;

    int cycles = run_cycles(opcode_table[OP_ADDB].cycle_count * 1);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x11);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADDB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void adda_immediate_zero_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;

    uint8 code_bytes[] = {
        OP_ADDA,
        0
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 0;

    int cycles = run_cycles(opcode_table[OP_ADDA].cycle_count * 1);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADDA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void addb_immediate_zero_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;

    uint8 code_bytes[] = {
        OP_ADDB,
        0
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 0;

    int cycles = run_cycles(opcode_table[OP_ADDB].cycle_count * 1);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADDB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void addd_immediate_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    uint8 code_bytes[] = {
        OP_ADDD,
        0x12,
        0x34
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.d = 1;

    int cycles = run_cycles(opcode_table[OP_ADDD].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.d, 0x1235);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADDD].cycle_count);
    assert_true(post_pc == pre_pc + 3);
}

void addd_immediate_overflow_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    /* 0x7FFF is the maximum signed value that a word can handle hence
       the sum will overflow. */
    uint8 code_bytes[] = {
        OP_ADDD,
        0x7F,
        0xFF
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.d = 0x7FFF;

    int cycles = run_cycles(opcode_table[OP_ADDD].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.d, 0xFFFE);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_table[OP_ADDD].cycle_count);
    assert_true(post_pc == pre_pc + 3);
}

void addd_immediate_carry_flag_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* 0xAAAA and 0xBBBB are both negative number and their sum will both
       overflow and set carry. */
    uint8 code_bytes[] = {
        OP_ADDD,
        0xAA,
        0xAA
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.d = 0xBBBB;

    int cycles = run_cycles(opcode_table[OP_ADDD].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.d, 0x6665);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_table[OP_ADDD].cycle_count);
    assert_true(post_pc == pre_pc + 3);
}

void addd_immediate_zero_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_ADDD,
        0,
        0
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.d = 0;

    int cycles = run_cycles(opcode_table[OP_ADDD].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.d, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADDD].cycle_count);
    assert_true(post_pc == pre_pc + 3);
}
