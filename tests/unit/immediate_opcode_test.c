#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "headers.h"

extern struct cpu_state e_cpu_context;
extern struct opcode_def opcode_table[];
extern struct opcode_def opcode_ext_x10_table[];
extern struct opcode_def opcode_ext_x11_table[];

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

    int cycles = run_cycles(opcode_table[OP_ADCA].cycle_count);
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

    int cycles = run_cycles(opcode_table[OP_ADCB].cycle_count);
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

    int cycles = run_cycles(opcode_table[OP_ADCA].cycle_count);
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

    int cycles = run_cycles(opcode_table[OP_ADCB].cycle_count);
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

    int cycles = run_cycles(opcode_table[OP_ADCA].cycle_count);
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

    int cycles = run_cycles(opcode_table[OP_ADCB].cycle_count);
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

    int cycles = run_cycles(opcode_table[OP_ADCA].cycle_count);
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

    int cycles = run_cycles(opcode_table[OP_ADCB].cycle_count);
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

    int cycles = run_cycles(opcode_table[OP_ADCA].cycle_count);
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

    int cycles = run_cycles(opcode_table[OP_ADCB].cycle_count);
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

    int cycles = run_cycles(opcode_table[OP_ADCA].cycle_count);
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

    int cycles = run_cycles(opcode_table[OP_ADCB].cycle_count);
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

    int cycles = run_cycles(opcode_table[OP_ADDA].cycle_count);
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

    int cycles = run_cycles(opcode_table[OP_ADDB].cycle_count);
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

    int cycles = run_cycles(opcode_table[OP_ADDA].cycle_count);
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

    int cycles = run_cycles(opcode_table[OP_ADDB].cycle_count);
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

    int cycles = run_cycles(opcode_table[OP_ADDA].cycle_count);
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

    int cycles = run_cycles(opcode_table[OP_ADDB].cycle_count);
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

    int cycles = run_cycles(opcode_table[OP_ADDA].cycle_count);
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

    int cycles = run_cycles(opcode_table[OP_ADDB].cycle_count);
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

    int cycles = run_cycles(opcode_table[OP_ADDA].cycle_count);
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

    int cycles = run_cycles(opcode_table[OP_ADDB].cycle_count);
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

    int cycles = run_cycles(opcode_table[OP_ADDA].cycle_count);
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

    int cycles = run_cycles(opcode_table[OP_ADDB].cycle_count);
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

void anda_immediate_test(void **state) {
    (void) state; /* unused */

    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;
    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_ANDA,
        0x5
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 6;

    int cycles = run_cycles(opcode_table[OP_ANDA].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 4);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ANDA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void andb_immediate_test(void **state) {
    (void) state; /* unused */

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;
    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_ANDB,
        0x5
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 6;

    int cycles = run_cycles(opcode_table[OP_ANDB].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 4);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ANDB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void anda_immediate_zero_test(void **state) {
    (void) state; /* unused */

    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;
    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_ANDA,
        0
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 6;

    int cycles = run_cycles(opcode_table[OP_ANDA].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ANDA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void andb_immediate_zero_test(void **state) {
    (void) state; /* unused */

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;
    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_ANDB,
        0
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 6;

    int cycles = run_cycles(opcode_table[OP_ANDB].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ANDB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void andcc_immediate_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* We're looking to clear all but the carry flag */
    uint8 code_bytes[] = {
        OP_ANDCC,
        1
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);

    e_cpu_context.cc.n = 1;
    e_cpu_context.cc.e = 1;
    e_cpu_context.cc.c = 1;

    int cycles = run_cycles(opcode_table[OP_ANDCC].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.e, 0);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(cycles, opcode_table[OP_ANDCC].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void andcc_immediate_e_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* We're looking to clear all but the entire flag */
    uint8 code_bytes[] = {
        OP_ANDCC,
        0x80
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);

    e_cpu_context.cc.n = 1;
    e_cpu_context.cc.e = 1;
    e_cpu_context.cc.c = 1;

    int cycles = run_cycles(opcode_table[OP_ANDCC].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.e, 1);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(cycles, opcode_table[OP_ANDCC].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void bita_immediate_test(void **state) {
    (void) state; /* unused */

    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;
    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_BITA,
        0x5
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 6;

    int cycles = run_cycles(opcode_table[OP_BITA].cycle_count);
    int post_pc = e_cpu_context.pc;
    /* The accumulator should not get changed by this operation */
    assert_int_equal(e_cpu_context.d.byte_acc.a, 6);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_BITA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void bitb_immediate_test(void **state) {
    (void) state; /* unused */

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;
    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_BITB,
        0x5
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 6;

    int cycles = run_cycles(opcode_table[OP_BITB].cycle_count);
    int post_pc = e_cpu_context.pc;
    /* The accumulator should not get changed by this operation */
    assert_int_equal(e_cpu_context.d.byte_acc.b, 6);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_BITB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void bita_immediate_zero_test(void **state) {
    (void) state; /* unused */

    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;
    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_BITA,
        0
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 6;

    int cycles = run_cycles(opcode_table[OP_BITA].cycle_count);
    int post_pc = e_cpu_context.pc;
    /* The accumulator should not get changed by this operation */
    assert_int_equal(e_cpu_context.d.byte_acc.a, 6);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_BITA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void bitb_immediate_zero_test(void **state) {
    (void) state; /* unused */

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;
    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_BITB,
        0
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 6;

    int cycles = run_cycles(opcode_table[OP_BITB].cycle_count);
    int post_pc = e_cpu_context.pc;
    /* The accumulator should not get changed by this operation */
    assert_int_equal(e_cpu_context.d.byte_acc.b, 6);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_BITB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void bita_immediate_negative_test(void **state) {
    (void) state; /* unused */

    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;
    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_BITA,
        0xFF
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 0x80;

    int cycles = run_cycles(opcode_table[OP_BITA].cycle_count);
    int post_pc = e_cpu_context.pc;
    /* The accumulator should not get changed by this operation */
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x80);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_BITA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void bitb_immediate_negative_test(void **state) {
    (void) state; /* unused */

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;
    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_BITB,
        0xFF
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 0x80;

    int cycles = run_cycles(opcode_table[OP_BITB].cycle_count);
    int post_pc = e_cpu_context.pc;
    /* The accumulator should not get changed by this operation */
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x80);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_BITB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void cmpa_immediate_nocarry(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;

    uint8 code_bytes[] = {
        OP_CMPA,
        0x11
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 0xFF;

    int cycles = run_cycles(opcode_table[OP_CMPA].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_CMPA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void cmpb_immediate_nocarry(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;

    uint8 code_bytes[] = {
        OP_CMPB,
        0x11
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 0xFF;

    int cycles = run_cycles(opcode_table[OP_CMPB].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_CMPB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void cmpa_immediate_carry(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;

    uint8 code_bytes[] = {
        OP_CMPA,
        0x7A
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 0x27;

    int cycles = run_cycles(opcode_table[OP_CMPA].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x27);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_table[OP_CMPA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void cmpb_immediate_carry(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;

    uint8 code_bytes[] = {
        OP_CMPB,
        0x7A
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 0x27;

    int cycles = run_cycles(opcode_table[OP_CMPB].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x27);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_table[OP_CMPB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void cmpa_immediate_overflow(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;

    uint8 code_bytes[] = {
        OP_CMPA,
        0xEE
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 0xFF;

    int cycles = run_cycles(opcode_table[OP_CMPA].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_table[OP_CMPA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void cmpb_immediate_overflow(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;

    uint8 code_bytes[] = {
        OP_CMPB,
        0xEE
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 0xFF;

    int cycles = run_cycles(opcode_table[OP_CMPB].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_table[OP_CMPB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void cmpa_immediate_zero(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;

    uint8 code_bytes[] = {
        OP_CMPA,
        0x69
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 0x69;

    int cycles = run_cycles(opcode_table[OP_CMPA].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x69);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_CMPA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void cmpb_immediate_zero(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;

    uint8 code_bytes[] = {
        OP_CMPB,
        0x69
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 0x69;

    int cycles = run_cycles(opcode_table[OP_CMPB].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x69);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_CMPB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void cmpd_immediate_nocarry(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_CMPD,
        0x12,
        0x34
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.d = 0xFFFF;

    int cycles = run_cycles(opcode_table[OP_CMPD].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.d, 0xFFFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_CMPD].cycle_count);
    assert_true(post_pc == pre_pc + 4);
}

void cmpd_immediate_carry(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_CMPD,
        0x7A,
        0x7A
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.d = 0x2727;

    int cycles = run_cycles(opcode_table[OP_CMPD].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.d, 0x2727);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_CMPD].cycle_count);
    assert_true(post_pc == pre_pc + 4);
}

void cmpd_immediate_overflow(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_CMPD,
        0xEE,
        0xEE
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.d = 0xFFFF;

    int cycles = run_cycles(opcode_table[OP_CMPD].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.d, 0xFFFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_CMPD].cycle_count);
    assert_true(post_pc == pre_pc + 4);
}

void cmpd_immediate_zero(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_CMPD,
        0x69,
        0x69
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.d = 0x6969;

    int cycles = run_cycles(opcode_table[OP_CMPD].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.d, 0x6969);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_CMPD].cycle_count);
    assert_true(post_pc == pre_pc + 4);
}

void cmps_immediate_nocarry(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    uint8 code_bytes[] = {
        OP_EXTENDED_X11,
        OP_CMPS,
        0x12,
        0x34
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.s = 0xFFFF;

    int cycles = run_cycles(opcode_table[OP_CMPS].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.s, 0xFFFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_ext_x11_table[OP_CMPS].cycle_count);
    assert_true(post_pc == pre_pc + 4);
}

void cmps_immediate_carry(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    uint8 code_bytes[] = {
        OP_EXTENDED_X11,
        OP_CMPS,
        0x7A,
        0x7A
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.s = 0x2727;

    int cycles = run_cycles(opcode_table[OP_CMPS].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.s, 0x2727);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_ext_x11_table[OP_CMPS].cycle_count);
    assert_true(post_pc == pre_pc + 4);
}

void cmps_immediate_overflow(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    uint8 code_bytes[] = {
        OP_EXTENDED_X11,
        OP_CMPS,
        0xEE,
        0xEE
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.s = 0xFFFF;

    int cycles = run_cycles(opcode_table[OP_CMPS].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.s, 0xFFFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_ext_x11_table[OP_CMPS].cycle_count);
    assert_true(post_pc == pre_pc + 4);
}

void cmps_immediate_zero(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    uint8 code_bytes[] = {
        OP_EXTENDED_X11,
        OP_CMPS,
        0x69,
        0x69
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.s = 0x6969;

    int cycles = run_cycles(opcode_table[OP_CMPS].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.s, 0x6969);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_ext_x11_table[OP_CMPS].cycle_count);
    assert_true(post_pc == pre_pc + 4);
}

void cmpu_immediate_nocarry(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    uint8 code_bytes[] = {
        OP_EXTENDED_X11,
        OP_CMPU,
        0x12,
        0x34
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.u = 0xFFFF;

    int cycles = run_cycles(opcode_ext_x11_table[OP_CMPU].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.u, 0xFFFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_ext_x11_table[OP_CMPU].cycle_count);
    assert_true(post_pc == pre_pc + 4);
}

void cmpu_immediate_carry(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    uint8 code_bytes[] = {
        OP_EXTENDED_X11,
        OP_CMPU,
        0x7A,
        0x7A
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.u = 0x2727;

    int cycles = run_cycles(opcode_ext_x11_table[OP_CMPU].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.u, 0x2727);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_ext_x11_table[OP_CMPU].cycle_count);
    assert_true(post_pc == pre_pc + 4);
}

void cmpu_immediate_overflow(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    uint8 code_bytes[] = {
        OP_EXTENDED_X11,
        OP_CMPU,
        0xEE,
        0xEE
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.u = 0xFFFF;

    int cycles = run_cycles(opcode_ext_x11_table[OP_CMPU].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.u, 0xFFFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_ext_x11_table[OP_CMPU].cycle_count);
    assert_true(post_pc == pre_pc + 4);
}

void cmpu_immediate_zero(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    uint8 code_bytes[] = {
        OP_EXTENDED_X11,
        OP_CMPU,
        0x69,
        0x69
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.u = 0x6969;

    int cycles = run_cycles(opcode_ext_x11_table[OP_CMPU].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.u, 0x6969);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_ext_x11_table[OP_CMPU].cycle_count);
    assert_true(post_pc == pre_pc + 4);
}

void cmpx_immediate_nocarry(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    uint8 code_bytes[] = {
        OP_CMPX,
        0x12,
        0x34
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.x = 0xFFFF;

    int cycles = run_cycles(opcode_table[OP_CMPX].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.x, 0xFFFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_CMPX].cycle_count);
    assert_true(post_pc == pre_pc + 3);
}

void cmpx_immediate_carry(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    uint8 code_bytes[] = {
        OP_CMPX,
        0x7A,
        0x7A
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.x = 0x2727;

    int cycles = run_cycles(opcode_table[OP_CMPX].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.x, 0x2727);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_table[OP_CMPX].cycle_count);
    assert_true(post_pc == pre_pc + 3);
}

void cmpx_immediate_overflow(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    uint8 code_bytes[] = {
        OP_CMPX,
        0xEE,
        0xEE
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.x = 0xFFFF;

    int cycles = run_cycles(opcode_table[OP_CMPX].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.x, 0xFFFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_table[OP_CMPX].cycle_count);
    assert_true(post_pc == pre_pc + 3);
}

void cmpx_immediate_zero(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    uint8 code_bytes[] = {
        OP_CMPX,
        0x69,
        0x69
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.x = 0x6969;

    int cycles = run_cycles(opcode_table[OP_CMPX].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.x, 0x6969);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_CMPX].cycle_count);
    assert_true(post_pc == pre_pc + 3);
}

void cmpy_immediate_nocarry(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_CMPY,
        0x12,
        0x34
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.y = 0xFFFF;

    int cycles = run_cycles(opcode_table[OP_CMPY].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.y, 0xFFFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_CMPY].cycle_count);
    assert_true(post_pc == pre_pc + 4);
}

void cmpy_immediate_carry(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_CMPY,
        0x7A,
        0x7A
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.y = 0x2727;

    int cycles = run_cycles(opcode_table[OP_CMPY].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.y, 0x2727);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_CMPY].cycle_count);
    assert_true(post_pc == pre_pc + 4);
}

void cmpy_immediate_overflow(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_CMPY,
        0xEE,
        0xEE
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.y = 0xFFFF;

    int cycles = run_cycles(opcode_table[OP_CMPY].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.y, 0xFFFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_CMPY].cycle_count);
    assert_true(post_pc == pre_pc + 4);
}

void cmpy_immediate_zero(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_CMPY,
        0x69,
        0x69
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.y = 0x6969;

    int cycles = run_cycles(opcode_table[OP_CMPY].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.y, 0x6969);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_CMPY].cycle_count);
    assert_true(post_pc == pre_pc + 4);
}

void eora_immediate_test(void **state) {
    (void) state; /* unused */

    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;
    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_EORA,
        0x5
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 4;

    int cycles = run_cycles(opcode_table[OP_EORA].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 1);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_EORA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void eorb_immediate_test(void **state) {
    (void) state; /* unused */

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;
    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_EORB,
        0x5
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 4;

    int cycles = run_cycles(opcode_table[OP_EORB].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 1);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_EORB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void eora_immediate_zero_test(void **state) {
    (void) state; /* unused */

    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;
    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_EORA,
        0xEE
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 0xEE;

    int cycles = run_cycles(opcode_table[OP_EORA].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_EORA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void eorb_immediate_zero_test(void **state) {
    (void) state; /* unused */

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;
    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_EORB,
        0xEE
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 0xEE;

    int cycles = run_cycles(opcode_table[OP_EORB].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_EORB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void exg_basic_test(void **state) {
    (void) state; /* unused */

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;
    int pre_pc = e_cpu_context.pc;
    /* 0x9A = B to CC */
    uint8 code_bytes[] = {
        OP_EXG,
        0x9A
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 0x78;

    int cycles = run_cycles(opcode_table[OP_EXG].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(0x78, *((uint8*) (&e_cpu_context.cc)));
    assert_int_equal(cycles, opcode_table[OP_EXG].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void exg_basic_16_test(void **state) {
    (void) state; /* unused */

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;
    int pre_pc = e_cpu_context.pc;
    /* 0x12 = X to Y */
    uint8 code_bytes[] = {
        OP_EXG,
        0x12
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.x = 0x7812;
    e_cpu_context.y = 0x6969;

    int cycles = run_cycles(opcode_table[OP_EXG].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0);
    assert_int_equal(e_cpu_context.x, 0x6969);
    assert_int_equal(e_cpu_context.y, 0x7812);
    assert_int_equal(cycles, opcode_table[OP_EXG].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void exg_basic_16_to_8_test(void **state) {
    (void) state; /* unused */

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;
    int pre_pc = e_cpu_context.pc;
    /* 0x19 = X to B */
    uint8 code_bytes[] = {
        OP_EXG,
        0x19
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.x = 0x7812;
    e_cpu_context.d.byte_acc.b = 0x69;

    int cycles = run_cycles(opcode_table[OP_EXG].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x12);
    assert_int_equal(e_cpu_context.x, 0xFF69);
    assert_int_equal(cycles, opcode_table[OP_EXG].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void exg_basic_8_to_16_test(void **state) {
    (void) state; /* unused */

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;
    int pre_pc = e_cpu_context.pc;
    /* 0x91 = B to X */
    uint8 code_bytes[] = {
        OP_EXG,
        0x91
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.x = 0x7812;
    e_cpu_context.d.byte_acc.b = 0x69;

    int cycles = run_cycles(opcode_table[OP_EXG].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x12);
    assert_int_equal(e_cpu_context.x, 0xFF69);
    assert_int_equal(cycles, opcode_table[OP_EXG].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void exg_a_d_8_to_16_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* 0x80 = A to D */
    uint8 code_bytes[] = {
        OP_EXG,
        0x80
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 0x12;
    e_cpu_context.d.byte_acc.b = 0x34;

    int cycles = run_cycles(opcode_table[OP_EXG].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x34);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x12);
    assert_int_equal(cycles, opcode_table[OP_EXG].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void exg_cc_x_8_to_16_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* 0xA1 = CC to X */
    uint8 code_bytes[] = {
        OP_EXG,
        0xA1
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    *((uint8*) (&e_cpu_context.cc)) = 0x69;
    e_cpu_context.x = 0x1234;

    int cycles = run_cycles(opcode_table[OP_EXG].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(*((uint8*) (&e_cpu_context.cc)), 0x34);
    assert_int_equal(e_cpu_context.x, 0x6969);
    assert_int_equal(cycles, opcode_table[OP_EXG].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void exg_cc_invalid_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* 0xAF = CC to ? */
    uint8 code_bytes[] = {
        OP_EXG,
        0xAF
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    *((uint8*) (&e_cpu_context.cc)) = 0x69;

    int cycles = run_cycles(opcode_table[OP_EXG].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(*((uint8*) (&e_cpu_context.cc)), 0xFF);
    assert_int_equal(cycles, opcode_table[OP_EXG].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void exg_x_invalid_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* 0xAF = X to ? */
    uint8 code_bytes[] = {
        OP_EXG,
        0x1F
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.x = 0x1234;

    int cycles = run_cycles(opcode_table[OP_EXG].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.x, 0xFFFF);
    assert_int_equal(cycles, opcode_table[OP_EXG].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void lda_basic_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_LDA,
        0x1F
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);

    int cycles = run_cycles(opcode_table[OP_LDA].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x1F);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0);
    assert_int_equal(cycles, opcode_table[OP_LDA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void ldb_basic_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_LDB,
        0x1F
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);

    int cycles = run_cycles(opcode_table[OP_LDB].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x1F);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0);
    assert_int_equal(cycles, opcode_table[OP_LDB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void ldd_basic_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_LDD,
        0xF0,
        0x0D
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 }
    };
    load_memory(test_memory, 1);

    int cycles = run_cycles(opcode_table[OP_LDD].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.d, 0xF00D);
    assert_int_equal(cycles, opcode_table[OP_LDD].cycle_count);
    assert_true(post_pc == pre_pc + 3);
}

void lds_basic_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LDS,
        0xF0,
        0x0D
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);

    int cycles = run_cycles(opcode_ext_x10_table[OP_LDS].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.s, 0xF00D);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LDS].cycle_count);
    assert_true(post_pc == pre_pc + 4);
}

void ldu_basic_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_LDU,
        0xF0,
        0x0D
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 }
    };
    load_memory(test_memory, 1);

    int cycles = run_cycles(opcode_table[OP_LDU].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.u, 0xF00D);
    assert_int_equal(cycles, opcode_table[OP_LDU].cycle_count);
    assert_true(post_pc == pre_pc + 3);
}

void ldx_basic_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_LDX,
        0xF0,
        0x0D
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 }
    };
    load_memory(test_memory, 1);

    int cycles = run_cycles(opcode_table[OP_LDX].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.x, 0xF00D);
    assert_int_equal(cycles, opcode_table[OP_LDX].cycle_count);
    assert_true(post_pc == pre_pc + 3);
}

void ldy_basic_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LDY,
        0xF0,
        0x0D
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);

    int cycles = run_cycles(opcode_ext_x10_table[OP_LDY].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.y, 0xF00D);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LDY].cycle_count);
    assert_true(post_pc == pre_pc + 4);
}

void ora_immediate_test(void **state) {
    (void) state; /* unused */

    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;
    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_ORA,
        0x5
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 8;

    int cycles = run_cycles(opcode_table[OP_ORA].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xD);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ORA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void orb_immediate_test(void **state) {
    (void) state; /* unused */

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;
    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_ORB,
        0x5
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 8;

    int cycles = run_cycles(opcode_table[OP_ORB].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xD);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ORB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void orcc_immediate_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_ORCC,
        0x5
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);

    int cycles = run_cycles(opcode_table[OP_ORCC].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.i, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.f, 0);
    assert_int_equal(e_cpu_context.cc.e, 0);
    assert_int_equal(cycles, opcode_table[OP_ORCC].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void pshs_immediate_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_PSHS,
        0xFF
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);

    uint16 old_sp = get_reg_value_16(REG_S);
    set_reg_value_16(REG_U, USER_SPACE_ROOT + 0x100);
    set_reg_value_16(REG_Y, 0x1234);
    set_reg_value_16(REG_X, 0x5678);
    set_reg_value_8(REG_DP, 0x99);
    set_reg_value_8(REG_B, 0xAB);
    set_reg_value_8(REG_A, 0xCD);
    set_reg_value_8(REG_CC, 0xEF);

    int cycles = run_cycles(opcode_table[OP_PSHS].cycle_count);
    int post_pc = e_cpu_context.pc;
    int new_sp = get_reg_value_16(REG_S);

    assert_int_equal(new_sp, old_sp - 12);
    assert_int_equal(cycles, opcode_table[OP_PSHS].cycle_count + 12);
    assert_int_equal(read_byte_from_memory(new_sp), 0xEF);
    assert_int_equal(read_byte_from_memory(new_sp + 1), 0xCD);
    assert_int_equal(read_byte_from_memory(new_sp + 2), 0xAB);
    assert_int_equal(read_byte_from_memory(new_sp + 3), 0x99);
    assert_int_equal(read_byte_from_memory(new_sp + 4), 0x56);
    assert_int_equal(read_byte_from_memory(new_sp + 5), 0x78);
    assert_int_equal(read_byte_from_memory(new_sp + 6), 0x12);
    assert_int_equal(read_byte_from_memory(new_sp + 7), 0x34);
    assert_int_equal(read_word_from_memory(new_sp + 8), USER_SPACE_ROOT + 0x100);
    assert_int_equal(read_word_from_memory(new_sp + 10), post_pc);
    assert_true(post_pc == pre_pc + 2);
}

void pshu_immediate_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_PSHU,
        0xFF
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);

    uint16 old_sp = get_reg_value_16(REG_U);
    set_reg_value_16(REG_S, USER_SPACE_ROOT + 0x100);
    set_reg_value_16(REG_Y, 0x1234);
    set_reg_value_16(REG_X, 0x5678);
    set_reg_value_8(REG_DP, 0x99);
    set_reg_value_8(REG_B, 0xAB);
    set_reg_value_8(REG_A, 0xCD);
    set_reg_value_8(REG_CC, 0xEF);

    int cycles = run_cycles(opcode_table[OP_PSHU].cycle_count);
    int post_pc = e_cpu_context.pc;
    int new_sp = get_reg_value_16(REG_U);

    assert_int_equal(new_sp, old_sp - 12);
    assert_int_equal(cycles, opcode_table[OP_PSHU].cycle_count + 12);
    assert_int_equal(read_byte_from_memory(new_sp), 0xEF);
    assert_int_equal(read_byte_from_memory(new_sp + 1), 0xCD);
    assert_int_equal(read_byte_from_memory(new_sp + 2), 0xAB);
    assert_int_equal(read_byte_from_memory(new_sp + 3), 0x99);
    assert_int_equal(read_byte_from_memory(new_sp + 4), 0x56);
    assert_int_equal(read_byte_from_memory(new_sp + 5), 0x78);
    assert_int_equal(read_byte_from_memory(new_sp + 6), 0x12);
    assert_int_equal(read_byte_from_memory(new_sp + 7), 0x34);
    assert_int_equal(read_word_from_memory(new_sp + 8), USER_SPACE_ROOT + 0x100);
    assert_int_equal(read_word_from_memory(new_sp + 10), post_pc);
    assert_true(post_pc == pre_pc + 2);
}

void pshs_nothing_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_PSHS,
        0
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);

    uint16 old_sp = get_reg_value_16(REG_S);
    set_reg_value_16(REG_U, USER_SPACE_ROOT + 0x100);
    set_reg_value_16(REG_Y, 0x1234);
    set_reg_value_16(REG_X, 0x5678);
    set_reg_value_8(REG_DP, 0x99);
    set_reg_value_8(REG_B, 0xAB);
    set_reg_value_8(REG_A, 0xCD);
    set_reg_value_8(REG_CC, 0xEF);

    int cycles = run_cycles(opcode_table[OP_PSHS].cycle_count);
    int post_pc = e_cpu_context.pc;
    int new_sp = get_reg_value_16(REG_S);

    assert_int_equal(new_sp, old_sp);
    assert_int_equal(cycles, opcode_table[OP_PSHS].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void pshu_nothing_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_PSHU,
        0
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);

    uint16 old_sp = get_reg_value_16(REG_U);
    set_reg_value_16(REG_S, USER_SPACE_ROOT + 0x100);
    set_reg_value_16(REG_Y, 0x1234);
    set_reg_value_16(REG_X, 0x5678);
    set_reg_value_8(REG_DP, 0x99);
    set_reg_value_8(REG_B, 0xAB);
    set_reg_value_8(REG_A, 0xCD);
    set_reg_value_8(REG_CC, 0xEF);

    int cycles = run_cycles(opcode_table[OP_PSHU].cycle_count);
    int post_pc = e_cpu_context.pc;
    int new_sp = get_reg_value_16(REG_U);

    assert_int_equal(new_sp, old_sp);
    assert_int_equal(cycles, opcode_table[OP_PSHU].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void pshs_one_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_PSHS,
        0x10
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);

    uint16 old_sp = get_reg_value_16(REG_S);
    set_reg_value_16(REG_U, USER_SPACE_ROOT + 0x100);
    set_reg_value_16(REG_Y, 0x1234);
    set_reg_value_16(REG_X, 0x5678);
    set_reg_value_8(REG_DP, 0x99);
    set_reg_value_8(REG_B, 0xAB);
    set_reg_value_8(REG_A, 0xCD);
    set_reg_value_8(REG_CC, 0xEF);

    int cycles = run_cycles(opcode_table[OP_PSHS].cycle_count);
    int post_pc = e_cpu_context.pc;
    int new_sp = get_reg_value_16(REG_S);

    assert_int_equal(new_sp, old_sp - 2);
    assert_int_equal(cycles, opcode_table[OP_PSHS].cycle_count + 2);
    assert_int_equal(read_word_from_memory(new_sp), 0x5678);
    assert_true(post_pc == pre_pc + 2);
}

void pshu_one_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    uint8 code_bytes[] = {
        OP_PSHU,
        0x10
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);

    uint16 old_sp = get_reg_value_16(REG_U);
    set_reg_value_16(REG_S, USER_SPACE_ROOT + 0x100);
    set_reg_value_16(REG_Y, 0x1234);
    set_reg_value_16(REG_X, 0x5678);
    set_reg_value_8(REG_DP, 0x99);
    set_reg_value_8(REG_B, 0xAB);
    set_reg_value_8(REG_A, 0xCD);
    set_reg_value_8(REG_CC, 0xEF);

    int cycles = run_cycles(opcode_table[OP_PSHU].cycle_count);
    int post_pc = e_cpu_context.pc;
    int new_sp = get_reg_value_16(REG_U);

    assert_int_equal(new_sp, old_sp - 2);
    assert_int_equal(cycles, opcode_table[OP_PSHU].cycle_count + 2);
    assert_int_equal(read_word_from_memory(new_sp), 0x5678);
    assert_true(post_pc == pre_pc + 2);
}

void puls_immediate_test(void **state) {
    (void) state; /* unused */

    uint8 code_bytes[] = {
        OP_PSHS,
        0xFF,
        OP_PULS,
        0xFF
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);

    uint16 old_sp = get_reg_value_16(REG_S);
    set_reg_value_16(REG_U, USER_SPACE_ROOT + 0x100);
    set_reg_value_16(REG_Y, 0x1234);
    set_reg_value_16(REG_X, 0x5678);
    set_reg_value_8(REG_DP, 0x99);
    set_reg_value_8(REG_B, 0xAB);
    set_reg_value_8(REG_A, 0xCD);
    set_reg_value_8(REG_CC, 0xEF);

    run_cycles(opcode_table[OP_PSHS].cycle_count);

    set_reg_value_16(REG_U, 0);
    set_reg_value_16(REG_Y, 0);
    set_reg_value_16(REG_X, 0);
    set_reg_value_8(REG_DP, 0);
    set_reg_value_8(REG_B, 0);
    set_reg_value_8(REG_A, 0);
    set_reg_value_8(REG_CC, 0);

    assert_int_equal(get_reg_value_8(REG_B), 0);

    int pre_pc = e_cpu_context.pc;
    int cycles = run_cycles(opcode_table[OP_PULS].cycle_count);
    int new_sp = get_reg_value_16(REG_S);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(new_sp, old_sp);
    assert_int_equal(cycles, opcode_table[OP_PULS].cycle_count + 12);

    assert_int_equal(get_reg_value_16(REG_U), USER_SPACE_ROOT + 0x100);
    assert_int_equal(get_reg_value_16(REG_Y), 0x1234);
    assert_int_equal(get_reg_value_16(REG_X), 0x5678);
    assert_int_equal(get_reg_value_8(REG_DP), 0x99);
    assert_int_equal(get_reg_value_8(REG_B), 0xAB);
    assert_int_equal(get_reg_value_8(REG_A), 0xCD);
    assert_int_equal(get_reg_value_8(REG_CC), 0xEF);

    /* pc was restored to its value pre pul */
    assert_int_equal(post_pc, pre_pc);
}

void pulu_immediate_test(void **state) {
    (void) state; /* unused */

    uint8 code_bytes[] = {
        OP_PSHU,
        0xFF,
        OP_PULU,
        0xFF
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);

    uint16 old_sp = get_reg_value_16(REG_U);
    set_reg_value_16(REG_S, USER_SPACE_ROOT + 0x100);
    set_reg_value_16(REG_Y, 0x1234);
    set_reg_value_16(REG_X, 0x5678);
    set_reg_value_8(REG_DP, 0x99);
    set_reg_value_8(REG_B, 0xAB);
    set_reg_value_8(REG_A, 0xCD);
    set_reg_value_8(REG_CC, 0xEF);

    run_cycles(opcode_table[OP_PSHU].cycle_count);

    set_reg_value_16(REG_S, 0);
    set_reg_value_16(REG_Y, 0);
    set_reg_value_16(REG_X, 0);
    set_reg_value_8(REG_DP, 0);
    set_reg_value_8(REG_B, 0);
    set_reg_value_8(REG_A, 0);
    set_reg_value_8(REG_CC, 0);

    assert_int_equal(get_reg_value_8(REG_B), 0);

    int pre_pc = e_cpu_context.pc;
    int cycles = run_cycles(opcode_table[OP_PULU].cycle_count);
    int new_sp = get_reg_value_16(REG_U);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(new_sp, old_sp);
    assert_int_equal(cycles, opcode_table[OP_PULU].cycle_count + 12);

    assert_int_equal(get_reg_value_16(REG_S), USER_SPACE_ROOT + 0x100);
    assert_int_equal(get_reg_value_16(REG_Y), 0x1234);
    assert_int_equal(get_reg_value_16(REG_X), 0x5678);
    assert_int_equal(get_reg_value_8(REG_DP), 0x99);
    assert_int_equal(get_reg_value_8(REG_B), 0xAB);
    assert_int_equal(get_reg_value_8(REG_A), 0xCD);
    assert_int_equal(get_reg_value_8(REG_CC), 0xEF);

    /* pc was restored to its value pre pul */
    assert_int_equal(post_pc, pre_pc);
}

void puls_one_test(void **state) {
    (void) state; /* unused */

    uint8 code_bytes[] = {
        OP_PSHS,
        0x20, /* push y */
        OP_PULS,
        0x20 /* pull y */
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);

    uint16 old_sp = get_reg_value_16(REG_S);
    set_reg_value_16(REG_U, USER_SPACE_ROOT + 0x100);
    set_reg_value_16(REG_Y, 0x1234);
    set_reg_value_16(REG_X, 0x5678);
    set_reg_value_8(REG_DP, 0x99);
    set_reg_value_8(REG_B, 0xAB);
    set_reg_value_8(REG_A, 0xCD);
    set_reg_value_8(REG_CC, 0xEF);

    run_cycles(opcode_table[OP_PSHS].cycle_count);

    set_reg_value_16(REG_U, 0);
    set_reg_value_16(REG_Y, 0);
    set_reg_value_16(REG_X, 0);
    set_reg_value_8(REG_DP, 0);
    set_reg_value_8(REG_B, 0);
    set_reg_value_8(REG_A, 0);
    set_reg_value_8(REG_CC, 0);

    assert_int_equal(get_reg_value_8(REG_B), 0);
    assert_int_equal(get_reg_value_16(REG_Y), 0);

    int pre_pc = e_cpu_context.pc;
    int cycles = run_cycles(opcode_table[OP_PULS].cycle_count);
    int new_sp = get_reg_value_16(REG_S);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(new_sp, old_sp);
    assert_int_equal(cycles, opcode_table[OP_PULS].cycle_count + 2);

    assert_int_equal(get_reg_value_16(REG_U), 0);
    assert_int_equal(get_reg_value_16(REG_Y), 0x1234);
    assert_int_equal(get_reg_value_16(REG_X), 0);
    assert_int_equal(get_reg_value_8(REG_DP), 0);
    assert_int_equal(get_reg_value_8(REG_B), 0);
    assert_int_equal(get_reg_value_8(REG_A), 0);
    assert_int_equal(get_reg_value_8(REG_CC), 0);

    assert_int_equal(post_pc, pre_pc + 2);
}

void pulu_one_test(void **state) {
    (void) state; /* unused */

    uint8 code_bytes[] = {
        OP_PSHU,
        0x20, /* push y */
        OP_PULU,
        0x20 /* pull y */
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);

    uint16 old_sp = get_reg_value_16(REG_U);
    set_reg_value_16(REG_S, USER_SPACE_ROOT + 0x100);
    set_reg_value_16(REG_Y, 0x1234);
    set_reg_value_16(REG_X, 0x5678);
    set_reg_value_8(REG_DP, 0x99);
    set_reg_value_8(REG_B, 0xAB);
    set_reg_value_8(REG_A, 0xCD);
    set_reg_value_8(REG_CC, 0xEF);

    run_cycles(opcode_table[OP_PSHU].cycle_count);

    set_reg_value_16(REG_S, 0);
    set_reg_value_16(REG_Y, 0);
    set_reg_value_16(REG_X, 0);
    set_reg_value_8(REG_DP, 0);
    set_reg_value_8(REG_B, 0);
    set_reg_value_8(REG_A, 0);
    set_reg_value_8(REG_CC, 0);

    assert_int_equal(get_reg_value_8(REG_B), 0);
    assert_int_equal(get_reg_value_16(REG_Y), 0);

    int pre_pc = e_cpu_context.pc;
    int cycles = run_cycles(opcode_table[OP_PULU].cycle_count);
    int new_sp = get_reg_value_16(REG_U);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(new_sp, old_sp);
    assert_int_equal(cycles, opcode_table[OP_PULU].cycle_count + 2);

    assert_int_equal(get_reg_value_16(REG_S), 0);
    assert_int_equal(get_reg_value_16(REG_Y), 0x1234);
    assert_int_equal(get_reg_value_16(REG_X), 0);
    assert_int_equal(get_reg_value_8(REG_DP), 0);
    assert_int_equal(get_reg_value_8(REG_B), 0);
    assert_int_equal(get_reg_value_8(REG_A), 0);
    assert_int_equal(get_reg_value_8(REG_CC), 0);

    assert_int_equal(post_pc, pre_pc + 2);
}

void puls_nothing_test(void **state) {
    (void) state; /* unused */

    uint8 code_bytes[] = {
        OP_PSHS,
        0xFF, /* push all */
        OP_PULS,
        0 /* pull nothing */
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);

    uint16 old_sp = get_reg_value_16(REG_S);
    set_reg_value_16(REG_U, USER_SPACE_ROOT + 0x100);
    set_reg_value_16(REG_Y, 0x1234);
    set_reg_value_16(REG_X, 0x5678);
    set_reg_value_8(REG_DP, 0x99);
    set_reg_value_8(REG_B, 0xAB);
    set_reg_value_8(REG_A, 0xCD);
    set_reg_value_8(REG_CC, 0xEF);

    run_cycles(opcode_table[OP_PSHS].cycle_count);

    set_reg_value_16(REG_U, 0);
    set_reg_value_16(REG_Y, 0);
    set_reg_value_16(REG_X, 0);
    set_reg_value_8(REG_DP, 0);
    set_reg_value_8(REG_B, 0);
    set_reg_value_8(REG_A, 0);
    set_reg_value_8(REG_CC, 0);

    assert_int_equal(get_reg_value_8(REG_B), 0);
    assert_int_equal(get_reg_value_16(REG_Y), 0);

    int pre_pc = e_cpu_context.pc;
    int cycles = run_cycles(opcode_table[OP_PULS].cycle_count);
    int new_sp = get_reg_value_16(REG_S);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(new_sp, old_sp - 12);
    assert_int_equal(cycles, opcode_table[OP_PULS].cycle_count);

    assert_int_equal(get_reg_value_16(REG_U), 0);
    assert_int_equal(get_reg_value_16(REG_Y), 0);
    assert_int_equal(get_reg_value_16(REG_X), 0);
    assert_int_equal(get_reg_value_8(REG_DP), 0);
    assert_int_equal(get_reg_value_8(REG_B), 0);
    assert_int_equal(get_reg_value_8(REG_A), 0);
    assert_int_equal(get_reg_value_8(REG_CC), 0);

    assert_int_equal(post_pc, pre_pc + 2);
}

void pulu_nothing_test(void **state) {
    (void) state; /* unused */

    uint8 code_bytes[] = {
        OP_PSHU,
        0xFF, /* push all */
        OP_PULU,
        0 /* pull nothing */
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 }
    };
    load_memory(test_memory, 1);

    uint16 old_sp = get_reg_value_16(REG_U);
    set_reg_value_16(REG_S, USER_SPACE_ROOT + 0x100);
    set_reg_value_16(REG_Y, 0x1234);
    set_reg_value_16(REG_X, 0x5678);
    set_reg_value_8(REG_DP, 0x99);
    set_reg_value_8(REG_B, 0xAB);
    set_reg_value_8(REG_A, 0xCD);
    set_reg_value_8(REG_CC, 0xEF);

    run_cycles(opcode_table[OP_PSHU].cycle_count);

    set_reg_value_16(REG_S, 0);
    set_reg_value_16(REG_Y, 0);
    set_reg_value_16(REG_X, 0);
    set_reg_value_8(REG_DP, 0);
    set_reg_value_8(REG_B, 0);
    set_reg_value_8(REG_A, 0);
    set_reg_value_8(REG_CC, 0);

    assert_int_equal(get_reg_value_8(REG_B), 0);
    assert_int_equal(get_reg_value_16(REG_Y), 0);

    int pre_pc = e_cpu_context.pc;
    int cycles = run_cycles(opcode_table[OP_PULU].cycle_count);
    int new_sp = get_reg_value_16(REG_U);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(new_sp, old_sp - 12);
    assert_int_equal(cycles, opcode_table[OP_PULU].cycle_count);

    assert_int_equal(get_reg_value_16(REG_S), 0);
    assert_int_equal(get_reg_value_16(REG_Y), 0);
    assert_int_equal(get_reg_value_16(REG_X), 0);
    assert_int_equal(get_reg_value_8(REG_DP), 0);
    assert_int_equal(get_reg_value_8(REG_B), 0);
    assert_int_equal(get_reg_value_8(REG_A), 0);
    assert_int_equal(get_reg_value_8(REG_CC), 0);

    assert_int_equal(post_pc, pre_pc + 2);
}

void sbca_immediate_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    set_reg_value_8(REG_B, 0xFF);

    uint8 code_bytes[] = {
        OP_SBCA,
        0x4
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.cc.c = 1;
    set_reg_value_8(REG_A, 6);

    int cycles = run_cycles(opcode_table[OP_SBCA].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(get_reg_value_8(REG_A), 1);
    assert_int_equal(get_reg_value_8(REG_B), 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_SBCA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void sbcb_immediate_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* a shouldn't be messed with */
    set_reg_value_8(REG_A, 0xFF);

    uint8 code_bytes[] = {
        OP_SBCB,
        0x4
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.cc.c = 1;
    set_reg_value_8(REG_B, 6);

    int cycles = run_cycles(opcode_table[OP_SBCB].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(get_reg_value_8(REG_B), 1);
    assert_int_equal(get_reg_value_8(REG_A), 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_SBCB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void sbca_carry_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    set_reg_value_8(REG_B, 0xFF);

    uint8 code_bytes[] = {
        OP_SBCA,
        0x40
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    set_reg_value_8(REG_A, 0);

    int cycles = run_cycles(opcode_table[OP_SBCA].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(get_reg_value_8(REG_A), 0xC0);
    assert_int_equal(get_reg_value_8(REG_B), 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_SBCA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void sbcb_carry_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    set_reg_value_8(REG_A, 0xFF);

    /* 0 - 0x40 = 0xC0 borrowing one */
    uint8 code_bytes[] = {
        OP_SBCB,
        0x40
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    set_reg_value_8(REG_B, 0);

    int cycles = run_cycles(opcode_table[OP_SBCB].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(get_reg_value_8(REG_B), 0xC0);
    assert_int_equal(get_reg_value_8(REG_A), 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_SBCB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void sbca_overflow_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    set_reg_value_8(REG_B, 0xFF);

    uint8 code_bytes[] = {
        OP_SBCA,
        0x80
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    set_reg_value_8(REG_A, 0x40);

    int cycles = run_cycles(opcode_table[OP_SBCA].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(get_reg_value_8(REG_A), 0xC0);
    assert_int_equal(get_reg_value_8(REG_B), 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_table[OP_SBCA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void sbcb_overflow_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* a shouldn't be messed with */
    set_reg_value_8(REG_A, 0xFF);

    uint8 code_bytes[] = {
        OP_SBCB,
        0x80
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    set_reg_value_8(REG_B, 0x40);

    int cycles = run_cycles(opcode_table[OP_SBCB].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(get_reg_value_8(REG_B), 0xC0);
    assert_int_equal(get_reg_value_8(REG_A), 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_table[OP_SBCB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void suba_immediate_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    set_reg_value_8(REG_B, 0xFF);

    uint8 code_bytes[] = {
        OP_SUBA,
        0x4
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    set_reg_value_8(REG_A, 6);

    int cycles = run_cycles(opcode_table[OP_SUBA].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(get_reg_value_8(REG_A), 2);
    assert_int_equal(get_reg_value_8(REG_B), 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_SUBA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void subb_immediate_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    set_reg_value_8(REG_A, 0xFF);

    uint8 code_bytes[] = {
        OP_SUBB,
        0x4
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    set_reg_value_8(REG_B, 6);

    int cycles = run_cycles(opcode_table[OP_SUBB].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(get_reg_value_8(REG_B), 2);
    assert_int_equal(get_reg_value_8(REG_A), 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_SUBB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void suba_carry_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    set_reg_value_8(REG_B, 0xFF);

    uint8 code_bytes[] = {
        OP_SUBA,
        0x40
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    set_reg_value_8(REG_A, 0);

    int cycles = run_cycles(opcode_table[OP_SUBA].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(get_reg_value_8(REG_A), 0xC0);
    assert_int_equal(get_reg_value_8(REG_B), 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_SUBA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void subb_carry_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    set_reg_value_8(REG_A, 0xFF);

    /* 0 - 0x40 = 0xC0 borrowing one */
    uint8 code_bytes[] = {
        OP_SUBB,
        0x40
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    set_reg_value_8(REG_B, 0);

    int cycles = run_cycles(opcode_table[OP_SUBB].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(get_reg_value_8(REG_B), 0xC0);
    assert_int_equal(get_reg_value_8(REG_A), 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_SUBB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void suba_overflow_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    set_reg_value_8(REG_B, 0xFF);

    uint8 code_bytes[] = {
        OP_SUBA,
        0x80
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    set_reg_value_8(REG_A, 0x40);

    int cycles = run_cycles(opcode_table[OP_SUBA].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(get_reg_value_8(REG_A), 0xC0);
    assert_int_equal(get_reg_value_8(REG_B), 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_table[OP_SUBA].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void subb_overflow_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* a shouldn't be messed with */
    set_reg_value_8(REG_A, 0xFF);

    uint8 code_bytes[] = {
        OP_SUBB,
        0x80
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    set_reg_value_8(REG_B, 0x40);

    int cycles = run_cycles(opcode_table[OP_SUBB].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(get_reg_value_8(REG_B), 0xC0);
    assert_int_equal(get_reg_value_8(REG_A), 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_table[OP_SUBB].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void sub16d_immediate_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    uint8 code_bytes[] = {
        OP_SUBD,
        0x00,
        0x04
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 }
    };
    load_memory(test_memory, 1);
    set_reg_value_16(REG_D, 6);

    int cycles = run_cycles(opcode_table[OP_SUBD].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(get_reg_value_16(REG_D), 2);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_SUBD].cycle_count);
    assert_true(post_pc == pre_pc + 3);
}

void sub16d_carry_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    uint8 code_bytes[] = {
        OP_SUBD,
        0x40,
        0x00
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 }
    };
    load_memory(test_memory, 1);
    set_reg_value_16(REG_D, 0);

    int cycles = run_cycles(opcode_table[OP_SUBD].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(get_reg_value_16(REG_D), 0xC000);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_SUBD].cycle_count);
    assert_true(post_pc == pre_pc + 3);
}

void sub16d_overflow_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;

    uint8 code_bytes[] = {
        OP_SUBD,
        0x80,
        0x00
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 }
    };
    load_memory(test_memory, 1);
    set_reg_value_16(REG_D, 0x4000);

    int cycles = run_cycles(opcode_table[OP_SUBD].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(get_reg_value_16(REG_D), 0xC000);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_table[OP_SUBD].cycle_count);
    assert_true(post_pc == pre_pc + 3);
}

void tfr_basic_test(void **state) {
    (void) state; /* unused */

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;
    int pre_pc = e_cpu_context.pc;
    /* 0x9A = B to CC */
    uint8 code_bytes[] = {
        OP_TFR,
        0x9A
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.b = 0x78;

    int cycles = run_cycles(opcode_table[OP_TFR].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x78);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(*((uint8*) (&e_cpu_context.cc)), 0x78);
    assert_int_equal(cycles, opcode_table[OP_TFR].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void tfr_basic_16_test(void **state) {
    (void) state; /* unused */

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;
    int pre_pc = e_cpu_context.pc;
    /* 0x12 = X to Y */
    uint8 code_bytes[] = {
        OP_TFR,
        0x12
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.x = 0x7812;
    e_cpu_context.y = 0x6969;

    int cycles = run_cycles(opcode_table[OP_TFR].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0);
    assert_int_equal(e_cpu_context.x, 0x7812);
    assert_int_equal(e_cpu_context.y, 0x7812);
    assert_int_equal(cycles, opcode_table[OP_TFR].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void tfr_basic_16_to_8_test(void **state) {
    (void) state; /* unused */

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;
    int pre_pc = e_cpu_context.pc;
    /* 0x19 = X to B */
    uint8 code_bytes[] = {
        OP_TFR,
        0x19
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.x = 0x7812;
    e_cpu_context.d.byte_acc.b = 0x69;

    int cycles = run_cycles(opcode_table[OP_TFR].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x12);
    assert_int_equal(e_cpu_context.x, 0x7812);
    assert_int_equal(cycles, opcode_table[OP_TFR].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void tfr_basic_8_to_16_test(void **state) {
    (void) state; /* unused */

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xEE;
    int pre_pc = e_cpu_context.pc;
    /* 0x91 = B to X */
    uint8 code_bytes[] = {
        OP_TFR,
        0x91
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.x = 0x7812;
    e_cpu_context.d.byte_acc.b = 0x69;

    int cycles = run_cycles(opcode_table[OP_TFR].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xEE);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x69);
    assert_int_equal(e_cpu_context.x, 0xFF69);
    assert_int_equal(cycles, opcode_table[OP_TFR].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void tfr_a_d_8_to_16_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* 0x80 = A to D */
    uint8 code_bytes[] = {
        OP_TFR,
        0x80
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    e_cpu_context.d.byte_acc.a = 0x12;
    e_cpu_context.d.byte_acc.b = 0x34;

    int cycles = run_cycles(opcode_table[OP_TFR].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x12);
    assert_int_equal(cycles, opcode_table[OP_TFR].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void tfr_cc_x_8_to_16_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* 0xA1 = CC to X */
    uint8 code_bytes[] = {
        OP_TFR,
        0xA1
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    *((uint8*) (&e_cpu_context.cc)) = 0x69;
    e_cpu_context.x = 0x1234;

    int cycles = run_cycles(opcode_table[OP_TFR].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(*((uint8*) (&e_cpu_context.cc)), 0x69);
    assert_int_equal(e_cpu_context.x, 0x6969);
    assert_int_equal(cycles, opcode_table[OP_TFR].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void tfr_cc_invalid_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* 0xAF = CC to ? */
    uint8 code_bytes[] = {
        OP_TFR,
        0xAF
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    *((uint8*) (&e_cpu_context.cc)) = 0x69;

    int cycles = run_cycles(opcode_table[OP_TFR].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(*((uint8*) (&e_cpu_context.cc)), 0x69);
    assert_int_equal(cycles, opcode_table[OP_TFR].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}
