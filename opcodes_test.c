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
    int cycles = abx(0x12, REG_NONE, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[0x12].cycle_count);
    assert_true(post_pc > pre_pc);
}

void abx_test_add(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.x = 1;
    e_cpu_context.d.byte_acc.b = 2;
    int cycles = abx(0x12, REG_NONE, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[0x12].cycle_count);
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
    int cycles = abx(0x12, REG_NONE, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[0x12].cycle_count);
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
    int cycles = asl(0x48, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[0x48].cycle_count);
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
    int cycles = asl(0x48, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[0x48].cycle_count);
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
    int cycles = asl(0x58, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[0x58].cycle_count);
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
    int cycles = asl(0x58, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[0x58].cycle_count);
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
    int cycles = nop(0x12, REG_NONE, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[0x12].cycle_count);
    assert_true(post_pc > pre_pc);
}
