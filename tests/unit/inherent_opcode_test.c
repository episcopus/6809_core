#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "opcodes.h"

extern struct cpu_state e_cpu_context;
extern struct opcode_def opcode_table[];
extern struct opcode_def opcode_ext_x10_table[];
extern struct opcode_def opcode_ext_x11_table[];

void abx_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    int cycles = abx(OP_ABX, REG_NONE, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_ABX].cycle_count);
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

void nop_pc_test(void **state) {
    (void) state; /* unused */

    uint8 code_bytes[] = {
        0x12
    };

    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1}
    };

    load_memory(test_memory, 1);

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

void asra_negative_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* Basic shift right by one bit, the most significant bit (sign)
       should be preserved. */
    e_cpu_context.d.byte_acc.a = 0x81;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = asr(OP_ASRA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_ASRA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x80);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_true(post_pc > pre_pc);
}

void asrb_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* Basic shift right by one bit */
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
    /* Basic shift right by one bit */
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

void asrb_negative_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* Basic shift right by one bit, the most significant bit (sign)
       should be preserved. */
    e_cpu_context.d.byte_acc.b = 0x81;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = asr(OP_ASRB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_ASRB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x80);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
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

void cwai_basic_test(void **state) {
    (void) state; /* unused */

    set_reg_value_16(REG_PC, 0x100);
    set_reg_value_8(REG_A, 0x69);
    uint8 code_bytes[] = {
        OP_CWAI,
        0xEF, /* Ensures the IRQ vector is not suppressed */
        OP_CLRA
    };
    struct mem_loader_def test_memory[] = {
        { 0x100, code_bytes, 3 },
    };
    load_memory(test_memory, 1);

    int cycles = run_cycles(0xFF);

    /* The processor is hanging on a cwai instruction hence only processed that
       instruction's cycles */
    assert_int_equal(cycles, opcode_table[OP_CWAI].cycle_count);
    assert_int_equal(get_reg_value_16(REG_PC), 0x102);
    assert_int_equal(get_reg_value_8(REG_A), 0x69);
    assert_int_equal(e_cpu_context.halted_state, HS_CWAI);
}

void cwai_basic_irq_test(void **state) {
    (void) state; /* unused */

    set_reg_value_16(REG_PC, 0x100);
    set_reg_value_8(REG_A, 0x69);
    e_cpu_context.cc.i = 0;
    uint8 code_bytes[] = {
        OP_CWAI,
        0xEF, /* Ensures the IRQ vector is not suppressed */
        OP_CLRA
    };
    uint8 irq_bytes[] = {
        OP_INCA,
        OP_RTI
    };
    write_word_to_memory(IRQ_VECTOR, 0x200);
    struct mem_loader_def test_memory[] = {
        { 0x100, code_bytes, 3 },
        { 0x200, irq_bytes, 2 },
    };
    load_memory(test_memory, 2);

    int cycles = run_cycles(0xFF);
    assert_int_equal(cycles, opcode_table[OP_CWAI].cycle_count);

    /* The second execution should yield 0 cycles since the CPU is in
       cwai state. */
    cycles += run_cycles(0xFF);
    assert_int_equal(cycles, opcode_table[OP_CWAI].cycle_count);
    assert_int_equal(get_reg_value_16(REG_PC), 0x102);
    assert_int_equal(get_reg_value_8(REG_A), 0x69);
    assert_int_equal(e_cpu_context.halted_state, HS_CWAI);

    /* Now send interrupt, non suppressed, so that CPU execution is continued
       at the IRQ */
    e_cpu_context.irq = 1;
    cycles += run_cycles(opcode_table[OP_INCA].cycle_count);
    assert_int_equal(get_reg_value_16(REG_PC), 0x201);
    assert_int_equal(get_reg_value_8(REG_A), 0x6A);
    assert_int_equal(e_cpu_context.halted_state, HS_NONE);
    assert_int_equal(cycles, opcode_table[OP_CWAI].cycle_count +
                     opcode_table[OP_INCA].cycle_count);
}

void daa_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* 0x8B should be adjusted to 0x91 per BCD logic. */
    e_cpu_context.d.byte_acc.a = 0x8B;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = daa(OP_DAA, REG_NONE, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_DAA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x91);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_true(post_pc > pre_pc);
}

void daa_not_adjusted_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* 0x45 is already compliant with BCD, shouldn't get adjusted. */
    e_cpu_context.d.byte_acc.a = 0x45;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = daa(OP_DAA, REG_NONE, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_DAA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x45);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_true(post_pc > pre_pc);
}

void deca_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.a = 0x45;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = dec(OP_DECA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_DECA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x44);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void decb_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.b = 0x45;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = dec(OP_DECB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_DECB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x44);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void deca_zero_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.a = 0x1;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = dec(OP_DECA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_DECA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void decb_zero_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.b = 0x1;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = dec(OP_DECB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_DECB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void deca_negative_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.a = 0;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = dec(OP_DECA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_DECA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void decb_negative_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.b = 0;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = dec(OP_DECB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_DECB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void inca_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.a = 0x45;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = inc(OP_INCA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_INCA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x46);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void incb_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.b = 0x45;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = inc(OP_INCB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_INCB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x46);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void inca_overflow_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* This value will overflow signed byte capacity, should set flag. */
    e_cpu_context.d.byte_acc.a = 0x7F;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = inc(OP_INCA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_INCA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x80);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_true(post_pc > pre_pc);
}

void incb_overflow_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* This value will overflow signed byte capacity, should set flag. */
    e_cpu_context.d.byte_acc.b = 0x7F;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = inc(OP_INCB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_INCB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x80);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_true(post_pc > pre_pc);
}

void inca_zero_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.a = 0xFF;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = inc(OP_INCA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_INCA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void incb_zero_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.b = 0xFF;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = inc(OP_INCB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_INCB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void inca_multiple_pc_test(void **state) {
    (void) state; /* unused */

    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;

    /* This should yield 3 in register a */
    uint8 code_bytes[] = {
        OP_INCA,
        OP_INCA,
        OP_INCA
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 }
    };
    load_memory(test_memory, 1);

    int cycles = run_cycles(opcode_table[OP_INCA].cycle_count * 3);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_INCA].cycle_count * 3);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x3);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(post_pc, USER_SPACE_ROOT + 3);
}

void lsra_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* Basic shift right by one bit */
    e_cpu_context.d.byte_acc.a = 2;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = lsr(OP_LSRA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_LSRA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x1);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void lsra_flags_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* Basic shift right by one bit */
    e_cpu_context.d.byte_acc.a = 0x1;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = lsr(OP_LSRA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_LSRA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_true(post_pc > pre_pc);
}

void lsra_negative_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* Basic shift right by one bit, the most significant bit (sign)
       should not be preserved. */
    e_cpu_context.d.byte_acc.a = 0x81;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = lsr(OP_LSRA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_LSRA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x40);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_true(post_pc > pre_pc);
}

void lsrb_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* Basic shift right by one bit */
    e_cpu_context.d.byte_acc.b = 4;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = lsr(OP_LSRB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_LSRB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x2);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void lsrb_flags_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* Basic shift right by one bit */
    e_cpu_context.d.byte_acc.b = 0x1;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = lsr(OP_LSRB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_LSRB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_true(post_pc > pre_pc);
}

void lsrb_negative_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* Basic shift right by one bit, the most significant bit (sign)
       should not be preserved. */
    e_cpu_context.d.byte_acc.b = 0x81;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = lsr(OP_LSRB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_LSRB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x40);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_true(post_pc > pre_pc);
}

void mul_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.a = 8;
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = mul(OP_MUL, REG_NONE, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_MUL].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x40);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x0);
    assert_int_equal(e_cpu_context.d.d, 0x40 /* 64 */);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_true(post_pc > pre_pc);
}

void mul_signed_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.a = 0xFF;
    e_cpu_context.d.byte_acc.b = 0xFF;
    int cycles = mul(OP_MUL, REG_NONE, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_MUL].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x01);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFE);
    assert_int_equal(e_cpu_context.d.d, 0xFE01 /* 64 */);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_true(post_pc > pre_pc);
}

void mul_zero_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.a = 0xFF;
    e_cpu_context.d.byte_acc.b = 0x0;
    int cycles = mul(OP_MUL, REG_NONE, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_MUL].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x0);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x0);
    assert_int_equal(e_cpu_context.d.d, 0x0 /* 64 */);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_true(post_pc > pre_pc);
}

void nega_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.a = 0x1;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = neg(OP_NEGA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_NEGA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_true(post_pc > pre_pc);
}

void negb_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.b = 0x1;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = neg(OP_NEGB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_NEGB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_true(post_pc > pre_pc);
}

void nega_minusone_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.a = 0xFF;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = neg(OP_NEGA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_NEGA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x01);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_true(post_pc > pre_pc);
}

void negb_minusone_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.b = 0xFF;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = neg(OP_NEGB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_NEGB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x01);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x08);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_true(post_pc > pre_pc);
}

void rola_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* Basic shift left by one bit */
    e_cpu_context.d.byte_acc.a = 0x2;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = rol(OP_ROLA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_ROLA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x4);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void rolb_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* Basic shift left by one bit */
    e_cpu_context.d.byte_acc.b = 2;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = rol(OP_ROLB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_ROLB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x4);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void rola_carry_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* this bit should end up in carry and set overflow as well */
    e_cpu_context.d.byte_acc.a = 0x80;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = rol(OP_ROLA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_ROLA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x0);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_true(post_pc > pre_pc);
}

void rolb_carry_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* this bit should end up in carry and set overflow as well */
    e_cpu_context.d.byte_acc.b = 0x80;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = rol(OP_ROLB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_ROLB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x0);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_true(post_pc > pre_pc);
}

void rola_rotate_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* this bit should end up in carry and set overflow as well */
    e_cpu_context.d.byte_acc.a = 0x80;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    /* this carry flag should end up in bit zero */
    e_cpu_context.cc.c = 1;
    int cycles = rol(OP_ROLA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_ROLA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x1);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_true(post_pc > pre_pc);
}

void rolb_rotate_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* this bit should end up in carry and set overflow as well */
    e_cpu_context.d.byte_acc.b = 0x80;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    /* this carry flag should end up in bit zero */
    e_cpu_context.cc.c = 1;
    int cycles = rol(OP_ROLA, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_ROLB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x1);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_true(post_pc > pre_pc);
}

void rora_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* Basic shift right by one bit */
    e_cpu_context.d.byte_acc.a = 0x2;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = ror(OP_RORA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_RORA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x1);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void rora_basic_test(void **state) {
    (void) state; /* unused */

    set_reg_value_16(REG_PC, 0x5000);
    int pre_pc = e_cpu_context.pc;
    /* Basic shift right by one bit */
    e_cpu_context.d.byte_acc.a = 0x2;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;

    uint8 code_bytes[] = {
        OP_RORA
    };
    struct mem_loader_def test_memory[] = {
        { 0x5000, code_bytes, 1 },
    };
    load_memory(test_memory, 1);

    int cycles = run_cycles(opcode_table[OP_RORA].cycle_count);
    uint16 post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_RORA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x1);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(post_pc, pre_pc + 1);
}

void rorb_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* Basic shift right by one bit */
    e_cpu_context.d.byte_acc.b = 2;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = ror(OP_RORB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_RORB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x1);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void rorb_basic_test(void **state) {
    (void) state; /* unused */

    set_reg_value_16(REG_PC, 0x5000);
    int pre_pc = e_cpu_context.pc;
    /* Basic shift right by one bit */
    e_cpu_context.d.byte_acc.b = 0x2;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;

    uint8 code_bytes[] = {
        OP_RORB
    };
    struct mem_loader_def test_memory[] = {
        { 0x5000, code_bytes, 1 },
    };
    load_memory(test_memory, 1);

    int cycles = run_cycles(opcode_table[OP_RORB].cycle_count);
    uint16 post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_RORB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x1);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(post_pc, pre_pc + 1);
}

void rora_carry_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* this bit should end up in carry */
    e_cpu_context.d.byte_acc.a = 0x1;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = ror(OP_RORA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_RORA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x0);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void rorb_carry_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* this bit should end up in carry */
    e_cpu_context.d.byte_acc.b = 1;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = ror(OP_RORB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_RORB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x0);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void rora_rotate_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* this bit should end up in carry  */
    e_cpu_context.d.byte_acc.a = 0x1;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    /* this carry flag should end up in bit seven */
    e_cpu_context.cc.c = 1;
    int cycles = ror(OP_RORB, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_RORB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x80);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_true(post_pc > pre_pc);
}

void rorb_rotate_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* this bit should end up in carry */
    e_cpu_context.d.byte_acc.b = 0x1;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 8;
    /* this carry flag should end up in bit seven */
    e_cpu_context.cc.c = 1;
    int cycles = ror(OP_RORB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_RORB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x80);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_true(post_pc > pre_pc);
}

void rti_basic_test(void ** state) {
    (void) state; /* unused */

    set_reg_value_8(REG_A, 0x12);
    set_reg_value_8(REG_B, 0x34);
    set_reg_value_8(REG_DP, 0x56);
    /* c, z, h are set by this */
    set_reg_value_8(REG_CC, 0x25);
    set_reg_value_16(REG_X, 0x1234);
    set_reg_value_16(REG_Y, 0x5678);
    set_reg_value_16(REG_U, 0x200);
    set_reg_value_16(REG_S, 0x100);
    set_reg_value_16(REG_PC, 0);
    /* Save pre PC before IRQ simulation */
    int pre_pc = e_cpu_context.pc;

    /* fake IRQ vector / pointer */
    write_word_to_memory(0xFFF8, 0x5000);

    /* Now simulate an IRQ interrupt */
    /* Forget about the masking for now  */
    /* if (e_cpu_context.cc.i) { */
    /*     return; */
    /* } */
    e_cpu_context.cc.e = 1;
    push_word_to_stack(REG_S, get_reg_value_16(REG_PC));
    push_word_to_stack(REG_S, get_reg_value_16(REG_U));
    push_word_to_stack(REG_S, get_reg_value_16(REG_Y));
    push_word_to_stack(REG_S, get_reg_value_16(REG_X));
    push_byte_to_stack(REG_S, get_reg_value_8(REG_DP));
    push_byte_to_stack(REG_S, get_reg_value_8(REG_B));
    push_byte_to_stack(REG_S, get_reg_value_8(REG_A));
    push_byte_to_stack(REG_S, get_reg_value_8(REG_CC));
    set_reg_value_16(REG_PC, read_word_from_memory(0xFFF8));

    /* simulate IRQ code execution, it will mess with the registers */
    set_reg_value_8(REG_A, 0xFE);
    set_reg_value_8(REG_B, 0xDC);
    set_reg_value_8(REG_DP, 0xBA);
    /* v, n are set by this */
    set_reg_value_8(REG_CC, 0x8A);
    set_reg_value_16(REG_X, 0xFEDC);
    set_reg_value_16(REG_Y, 0xBA98);
    set_reg_value_16(REG_U, 0x300);

    uint8 code_bytes[] = {
        OP_RTI
    };
    struct mem_loader_def test_memory[] = {
        { 0x5000, code_bytes, 1 },
    };
    load_memory(test_memory, 1);

    int cycles = run_cycles(opcode_table[OP_RTI].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    /* Same PC since fake IRQ occurred before we ran any instructions
       from the original PC of 0x0 */
    assert_int_equal(post_pc, pre_pc);
    assert_int_equal(cycles, opcode_table[OP_RTI].cycle_count + 9);
    assert_int_equal(0x12, get_reg_value_8(REG_A));
    assert_int_equal(0x34, get_reg_value_8(REG_B));
    assert_int_equal(0x56, get_reg_value_8(REG_DP));
    assert_int_equal(0x25 | 0x80, get_reg_value_8(REG_CC));
    assert_int_equal(0x1234, get_reg_value_16(REG_X));
    assert_int_equal(0x5678, get_reg_value_16(REG_Y));
    assert_int_equal(0x200, get_reg_value_16(REG_U));
    assert_int_equal(0x100, get_reg_value_16(REG_S));
}

void rti_basic_firq_test(void ** state) {
    (void) state; /* unused */

    set_reg_value_8(REG_A, 0x12);
    set_reg_value_8(REG_B, 0x34);
    set_reg_value_8(REG_DP, 0x56);
    /* c, z, h are set by this */
    set_reg_value_8(REG_CC, 0x25);
    set_reg_value_16(REG_X, 0x1234);
    set_reg_value_16(REG_Y, 0x5678);
    set_reg_value_16(REG_U, 0x200);
    set_reg_value_16(REG_S, 0x100);
    set_reg_value_16(REG_PC, 0);
    /* Save pre PC before IRQ simulation */
    int pre_pc = e_cpu_context.pc;

    /* fake IRQ vector / pointer */
    write_word_to_memory(0xFFF6, 0x5000);

    /* Now simulate an FIRQ interrupt */
    /* Forget about the masking for now  */
    /* if (e_cpu_context.cc.i) { */
    /*     return; */
    /* } */
    push_word_to_stack(REG_S, get_reg_value_16(REG_PC));
    push_byte_to_stack(REG_S, get_reg_value_8(REG_CC));
    set_reg_value_16(REG_PC, read_word_from_memory(0xFFF6));

    /* simulate FIRQ code execution, it will mess with the registers */
    set_reg_value_8(REG_A, 0xFE);
    set_reg_value_8(REG_B, 0xDC);
    set_reg_value_8(REG_DP, 0xBA);
    /* v, n, f, i are set by this */
    set_reg_value_8(REG_CC, 0xDA);
    set_reg_value_16(REG_X, 0xFEDC);
    set_reg_value_16(REG_Y, 0xBA98);
    set_reg_value_16(REG_U, 0x300);

    uint8 code_bytes[] = {
        OP_RTI
    };
    struct mem_loader_def test_memory[] = {
        { 0x5000, code_bytes, 1 },
    };
    load_memory(test_memory, 1);

    int cycles = run_cycles(opcode_table[OP_RTI].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    /* Same PC since fake IRQ occurred before we ran any instructions
       from the original PC of 0x0 */
    assert_int_equal(post_pc, pre_pc);
    assert_int_equal(cycles, opcode_table[OP_RTI].cycle_count);
    assert_int_equal(0xFE, get_reg_value_8(REG_A));
    assert_int_equal(0xDC, get_reg_value_8(REG_B));
    assert_int_equal(0xBA, get_reg_value_8(REG_DP));
    assert_int_equal(0x25, get_reg_value_8(REG_CC));
    assert_int_equal(0xFEDC, get_reg_value_16(REG_X));
    assert_int_equal(0xBA98, get_reg_value_16(REG_Y));
    assert_int_equal(0x300, get_reg_value_16(REG_U));
    assert_int_equal(0x100, get_reg_value_16(REG_S));
}

void rts_basic_test(void ** state) {
    (void) state; /* unused */

    set_reg_value_16(REG_PC, 0x100);
    set_reg_value_16(REG_S, 0x200);

    /* Simulate a call to a subroutine */
    push_word_to_stack(REG_S, get_reg_value_16(REG_PC));
    set_reg_value_16(REG_PC, 0x300);

    uint8 code_bytes[] = {
        OP_RTS
    };
    struct mem_loader_def test_memory[] = {
        { 0x300, code_bytes, 1 },
    };
    load_memory(test_memory, 1);

    int cycles = run_cycles(opcode_table[OP_RTS].cycle_count);
    uint16 post_pc = e_cpu_context.pc;

    assert_int_equal(post_pc, 0x100);
    assert_int_equal(cycles, opcode_table[OP_RTS].cycle_count);
    assert_int_equal(0x200, get_reg_value_16(REG_S));
}

void sex_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* this should extend to the whole d register */
    e_cpu_context.d.byte_acc.b = 0x1;
    /* this should be zeroed out */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = sex(OP_SEX, REG_NONE, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_SEX].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x1);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x0);
    assert_int_equal(e_cpu_context.d.d, 0x1);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_true(post_pc > pre_pc);
}

void sex_negative_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* this should extend to the whole d register */
    e_cpu_context.d.byte_acc.b = 0xFF;
    /* this should be F'd out */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = sex(OP_SEX, REG_NONE, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_SEX].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.d.d, 0xFFFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_true(post_pc > pre_pc);
}

void sex_zero_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* this should extend to the whole d register */
    e_cpu_context.d.byte_acc.b = 0;
    /* this should be zeroed out */
    e_cpu_context.d.byte_acc.a = 8;
    int cycles = sex(OP_SEX, REG_NONE, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_SEX].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x0);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x0);
    assert_int_equal(e_cpu_context.d.d, 0x0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_true(post_pc > pre_pc);
}

void swi_basic_test(void **state) {
    (void) state; /* unused */

    set_reg_value_8(REG_A, 0x2);
    set_reg_value_16(REG_PC, 0x100);

    uint8 code_bytes[] = {
        OP_INCA,
        OP_SWI
    };
    uint8 swi_bytes[] = {
        OP_CLRA,
        OP_RTI
    };
    write_word_to_memory(SWI_VECTOR, 0x200);
    struct mem_loader_def test_memory[] = {
        { 0x100, code_bytes, 2 },
        { 0x200, swi_bytes, 2 }
    };
    load_memory(test_memory, 2);

    /* Interrupt processor execution after the second INCA */
    int cycles = run_cycles(opcode_table[OP_INCA].cycle_count +
                            opcode_table[OP_SWI].cycle_count);

    assert_int_equal(get_reg_value_8(REG_A), 0x3);

    /* One INCA's at 2 cycles plus 19 cycles for SWI */
    assert_int_equal(cycles, 21);
    assert_int_equal(get_reg_value_16(REG_PC), 0x200);
}

void swi_with_rti_test(void **state) {
    (void) state; /* unused */

    set_reg_value_8(REG_A, 0x2);
    set_reg_value_16(REG_PC, 0x100);

    uint8 code_bytes[] = {
        OP_INCA,
        OP_SWI,
        OP_NOP
    };
    uint8 swi_bytes[] = {
        OP_CLRA,
        OP_RTI
    };
    write_word_to_memory(SWI_VECTOR, 0x200);
    struct mem_loader_def test_memory[] = {
        { 0x100, code_bytes, 3 },
        { 0x200, swi_bytes, 2 }
    };
    load_memory(test_memory, 2);

    /* Interrupt processor execution after the second INCA */
    int cycles = run_cycles(opcode_table[OP_INCA].cycle_count +
                            opcode_table[OP_SWI].cycle_count);

    assert_int_equal(get_reg_value_8(REG_A), 0x3);

    /* One INCA's at 2 cycles plus 19 cycles for SWI */
    assert_int_equal(cycles, 21);
    assert_int_equal(get_reg_value_16(REG_PC), 0x200);

    cycles += run_cycles(opcode_table[OP_CLRA].cycle_count);
    assert_int_equal(get_reg_value_8(REG_A), 0);

    /* Now return from the interrupt and the previous value of a should
       be restored */
    cycles += run_cycles(opcode_table[OP_RTI].cycle_count);
    assert_int_equal(get_reg_value_8(REG_A), 0x3);
    assert_int_equal(get_reg_value_16(REG_PC), 0x102);
}

void swi2_basic_test(void **state) {
    (void) state; /* unused */

    set_reg_value_8(REG_A, 0x2);
    set_reg_value_16(REG_PC, 0x100);

    uint8 code_bytes[] = {
        OP_INCA,
        OP_EXTENDED_X10,
        OP_SWI2
    };
    uint8 swi2_bytes[] = {
        OP_CLRA,
        OP_RTI
    };
    write_word_to_memory(SWI2_VECTOR, 0x200);
    struct mem_loader_def test_memory[] = {
        { 0x100, code_bytes, 3 },
        { 0x200, swi2_bytes, 2 }
    };
    load_memory(test_memory, 2);

    /* Interrupt processor execution after the second INCA */
    int cycles = run_cycles(opcode_table[OP_INCA].cycle_count +
                            opcode_ext_x10_table[OP_SWI2].cycle_count);

    assert_int_equal(get_reg_value_8(REG_A), 0x3);

    /* One INCA's at 2 cycles plus 20 cycles for SWI2 */
    assert_int_equal(cycles, 22);
    assert_int_equal(get_reg_value_16(REG_PC), 0x200);
}

void swi2_with_rti_test(void **state) {
    (void) state; /* unused */

    set_reg_value_8(REG_A, 0x2);
    set_reg_value_16(REG_PC, 0x100);

    uint8 code_bytes[] = {
        OP_INCA,
        OP_EXTENDED_X10,
        OP_SWI2,
        OP_NOP
    };
    uint8 swi2_bytes[] = {
        OP_CLRA,
        OP_RTI
    };
    write_word_to_memory(SWI2_VECTOR, 0x200);
    struct mem_loader_def test_memory[] = {
        { 0x100, code_bytes, 4 },
        { 0x200, swi2_bytes, 2 }
    };
    load_memory(test_memory, 2);

    /* Interrupt processor execution after the second INCA */
    int cycles = run_cycles(opcode_table[OP_INCA].cycle_count +
                            opcode_ext_x10_table[OP_SWI2].cycle_count);

    assert_int_equal(get_reg_value_8(REG_A), 0x3);

    /* One INCA's at 2 cycles plus 20 cycles for SWI2 */
    assert_int_equal(cycles, 22);
    assert_int_equal(get_reg_value_16(REG_PC), 0x200);

    cycles += run_cycles(opcode_table[OP_CLRA].cycle_count);
    assert_int_equal(get_reg_value_8(REG_A), 0);

    /* Now return from the interrupt and the previous value of a should
       be restored */
    cycles += run_cycles(opcode_table[OP_RTI].cycle_count);
    assert_int_equal(get_reg_value_8(REG_A), 0x3);
    assert_int_equal(get_reg_value_16(REG_PC), 0x103);
}

void swi3_basic_test(void **state) {
    (void) state; /* unused */

    set_reg_value_8(REG_A, 0x2);
    set_reg_value_16(REG_PC, 0x100);

    uint8 code_bytes[] = {
        OP_INCA,
        OP_EXTENDED_X11,
        OP_SWI3
    };
    uint8 swi3_bytes[] = {
        OP_CLRA,
        OP_RTI
    };
    write_word_to_memory(SWI3_VECTOR, 0x200);
    struct mem_loader_def test_memory[] = {
        { 0x100, code_bytes, 3 },
        { 0x200, swi3_bytes, 2 }
    };
    load_memory(test_memory, 2);

    /* Interrupt processor execution after the second INCA */
    int cycles = run_cycles(opcode_table[OP_INCA].cycle_count +
                            opcode_ext_x11_table[OP_SWI3].cycle_count);

    assert_int_equal(get_reg_value_8(REG_A), 0x3);

    /* One INCA's at 2 cycles plus 20 cycles for SWI3 */
    assert_int_equal(cycles, 22);
    assert_int_equal(get_reg_value_16(REG_PC), 0x200);
}

void swi3_with_rti_test(void **state) {
    (void) state; /* unused */

    set_reg_value_8(REG_A, 0x2);
    set_reg_value_16(REG_PC, 0x100);

    uint8 code_bytes[] = {
        OP_INCA,
        OP_EXTENDED_X11,
        OP_SWI3,
        OP_NOP
    };
    uint8 swi3_bytes[] = {
        OP_CLRA,
        OP_RTI
    };
    write_word_to_memory(SWI3_VECTOR, 0x200);
    struct mem_loader_def test_memory[] = {
        { 0x100, code_bytes, 4 },
        { 0x200, swi3_bytes, 2 }
    };
    load_memory(test_memory, 2);

    /* Interrupt processor execution after the second INCA */
    int cycles = run_cycles(opcode_table[OP_INCA].cycle_count +
                            opcode_ext_x11_table[OP_SWI3].cycle_count);

    assert_int_equal(get_reg_value_8(REG_A), 0x3);

    /* One INCA's at 2 cycles plus 20 cycles for SWI3 */
    assert_int_equal(cycles, 22);
    assert_int_equal(get_reg_value_16(REG_PC), 0x200);

    cycles += run_cycles(opcode_table[OP_CLRA].cycle_count);
    assert_int_equal(get_reg_value_8(REG_A), 0);

    /* Now return from the interrupt and the previous value of a should
       be restored */
    cycles += run_cycles(opcode_table[OP_RTI].cycle_count);
    assert_int_equal(get_reg_value_8(REG_A), 0x3);
    assert_int_equal(get_reg_value_16(REG_PC), 0x103);
}

void sync_basic_test(void **state) {
    (void) state; /* unused */

    set_reg_value_16(REG_PC, 0x100);
    set_reg_value_8(REG_A, 0x69);
    uint8 code_bytes[] = {
        OP_SYNC,
        OP_CLRA
    };
    struct mem_loader_def test_memory[] = {
        { 0x100, code_bytes, 2 },
    };
    load_memory(test_memory, 1);

    int cycles = run_cycles(0xFF);

    /* The processor is hanging on a sync instruction hence only processed that
       instruction's cycles */
    assert_int_equal(cycles, opcode_table[OP_SYNC].cycle_count);
    assert_int_equal(get_reg_value_16(REG_PC), 0x101);
    assert_int_equal(get_reg_value_8(REG_A), 0x69);
    assert_int_equal(e_cpu_context.halted_state, HS_SYNC);
}

void sync_basic_resume_test(void **state) {
    (void) state; /* unused */

    set_reg_value_16(REG_PC, 0x100);
    set_reg_value_8(REG_A, 0x69);
    e_cpu_context.cc.i = 1;
    uint8 code_bytes[] = {
        OP_SYNC,
        OP_CLRA
    };
    struct mem_loader_def test_memory[] = {
        { 0x100, code_bytes, 2 },
    };
    load_memory(test_memory, 1);

    int cycles = run_cycles(0xFF);
    assert_int_equal(cycles, opcode_table[OP_SYNC].cycle_count);

    /* The second execution should yield 0 cycles since the CPU is in
       sync state. */
    cycles += run_cycles(0xFF);
    assert_int_equal(cycles, opcode_table[OP_SYNC].cycle_count);
    assert_int_equal(get_reg_value_16(REG_PC), 0x101);
    assert_int_equal(get_reg_value_8(REG_A), 0x69);
    assert_int_equal(e_cpu_context.halted_state, HS_SYNC);

    /* Now send interrupt, suppressed, so that CPU execution is continued */
    e_cpu_context.irq = 1;
    cycles += run_cycles(opcode_table[OP_CLRA].cycle_count);

    assert_int_equal(cycles, opcode_table[OP_SYNC].cycle_count +
        opcode_table[OP_CLRA].cycle_count);
    assert_int_equal(get_reg_value_16(REG_PC), 0x102);
    assert_int_equal(get_reg_value_8(REG_A), 0);
    assert_int_equal(e_cpu_context.halted_state, HS_NONE);
}

void sync_basic_irq_test(void **state) {
    (void) state; /* unused */

    set_reg_value_16(REG_PC, 0x100);
    set_reg_value_8(REG_A, 0x69);
    e_cpu_context.cc.i = 0;
    uint8 code_bytes[] = {
        OP_SYNC,
        OP_CLRA
    };
    uint8 irq_bytes[] = {
        OP_INCA,
        OP_RTI
    };
    write_word_to_memory(IRQ_VECTOR, 0x200);
    struct mem_loader_def test_memory[] = {
        { 0x100, code_bytes, 2 },
        { 0x200, irq_bytes, 2 },
    };
    load_memory(test_memory, 2);

    int cycles = run_cycles(0xFF);
    assert_int_equal(cycles, opcode_table[OP_SYNC].cycle_count);

    /* The second execution should yield 0 cycles since the CPU is in
       sync state. */
    cycles += run_cycles(0xFF);
    assert_int_equal(cycles, opcode_table[OP_SYNC].cycle_count);
    assert_int_equal(get_reg_value_16(REG_PC), 0x101);
    assert_int_equal(get_reg_value_8(REG_A), 0x69);
    assert_int_equal(e_cpu_context.halted_state, HS_SYNC);

    /* Now send interrupt, non suppressed, so that CPU execution is continued
       at the IRQ */
    e_cpu_context.irq = 1;
    cycles += run_cycles(opcode_table[OP_CLRA].cycle_count);
    assert_int_equal(get_reg_value_16(REG_PC), 0x200);
    assert_int_equal(e_cpu_context.halted_state, HS_NONE);
    assert_int_equal(cycles, opcode_table[OP_SYNC].cycle_count +
                     12 /* cost of pushing all registers as part of IRQ */);
}

void tsta_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.a = 0x1;
    e_cpu_context.d.byte_acc.b = 8;
    e_cpu_context.cc.v = 1;
    int cycles = tst(OP_TSTA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_TSTA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x1);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void tstb_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.a = 0x1;
    e_cpu_context.d.byte_acc.b = 8;
    e_cpu_context.cc.v = 1;
    int cycles = tst(OP_TSTB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_TSTB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x1);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void tsta_negative_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.a = 0xFF;
    e_cpu_context.d.byte_acc.b = 8;
    e_cpu_context.cc.v = 1;
    int cycles = tst(OP_TSTA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_TSTA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void tstb_negative_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.a = 0x1;
    e_cpu_context.d.byte_acc.b = 0xFF;
    e_cpu_context.cc.v = 1;
    int cycles = tst(OP_TSTB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_TSTB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x1);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void tsta_zero_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.a = 0;
    e_cpu_context.d.byte_acc.b = 8;
    e_cpu_context.cc.v = 1;
    int cycles = tst(OP_TSTA, REG_A, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_TSTA].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void tstb_zero_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    e_cpu_context.d.byte_acc.a = 0x1;
    e_cpu_context.d.byte_acc.b = 0;
    e_cpu_context.cc.v = 1;
    int cycles = tst(OP_TSTB, REG_B, INHERENT);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_TSTB].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x1);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}
