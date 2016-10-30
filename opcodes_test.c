#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "functions.h"

extern struct cpu_state e_cpu_context;
extern struct opcode_def opcode_table[];

void abx_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    int cycles = abx(OP_NOP, REG_NONE, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_NOP].cycle_count);
    assert_true(post_pc > pre_pc);
}

void abx_test_add(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.x = 1;
    e_cpu_context.d.byte_acc.b = 2;
    int cycles = abx(OP_ABX, REG_NONE, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_ABX].cycle_count);
    assert_int_equal(e_cpu_context.x, 3);
    assert_true(post_pc > pre_pc);
}

void abx_test_unsigned(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.x = 0x301B;
    /* Were this 0xFF be treated as signed, it would result as
       a -1 - but this case the operation is unsigned. */
    e_cpu_context.d.byte_acc.b = 0xFF;
    int cycles = abx(OP_ABX, REG_NONE, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_ABX].cycle_count);
    assert_int_equal(e_cpu_context.x, 0x311A);
    assert_true(post_pc > pre_pc);
}

void asla_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* Basic shift left by one bit */
    e_cpu_context.d.byte_acc.a = 2;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = asl(OP_ASLA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_ASLA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x4);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void asla_flags_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* Basic shift left by one bit */
    e_cpu_context.d.byte_acc.a = 0x80;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = asl(OP_ASLA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_ASLA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_true(post_pc > pre_pc);
}

void aslb_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* Basic shift left by one bit */
    e_cpu_context.d.byte_acc.b = 2;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = asl(OP_ASLB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_ASLB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x4);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void aslb_flags_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* Basic shift left by one bit */
    e_cpu_context.d.byte_acc.b = 0x80;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = asl(OP_ASLB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_ASLB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_true(post_pc > pre_pc);
}

void nop_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    int cycles = nop(OP_NOP, REG_NONE, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_NOP].cycle_count);
    assert_true(post_pc > pre_pc);
}

void asra_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* Basic shift right by one bit */
    e_cpu_context.d.byte_acc.a = 2;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = asr(OP_ASRA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_ASRA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x1);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void asra_flags_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* Basic shift right by one bit */
    e_cpu_context.d.byte_acc.a = 0x1;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = asr(OP_ASRA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_ASRA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_true(post_pc > pre_pc);
}

void asrb_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* Basic shift left by one bit */
    e_cpu_context.d.byte_acc.b = 4;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = asr(OP_ASRB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_ASRB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x2);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void asrb_flags_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* Basic shift left by one bit */
    e_cpu_context.d.byte_acc.b = 0x1;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = asr(OP_ASRB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_ASRB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_true(post_pc > pre_pc);
}

void clra_flags_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.a = 0x1;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = clr(OP_CLRA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_CLRA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_true(post_pc > pre_pc);
}

void clrb_flags_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.b = 0x1;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = clr(OP_CLRB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_CLRB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_true(post_pc > pre_pc);
}

void coma_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.a = 0x1;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = com(OP_COMA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_COMA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFE);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_true(post_pc > pre_pc);
}

void comb_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.b = 0x1;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = com(OP_COMB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_COMB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFE);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_true(post_pc > pre_pc);
}

void coma_zero_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.a = 0xFF;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = com(OP_COMA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_COMA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x0);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_true(post_pc > pre_pc);
}

void comb_zero_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.b = 0xFF;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = com(OP_COMB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_COMB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x0);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_true(post_pc > pre_pc);
}
