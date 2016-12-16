#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "core.h"

extern struct cpu_state e_cpu_context;
extern struct opcode_def opcode_table[];
extern struct opcode_def opcode_ext_x10_table[];
extern struct opcode_def opcode_ext_x11_table[];

void adca_direct_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;
    uint8 lower_byte_address = 0x40;
    e_cpu_context.dp = S_POINTER >> 8;

    uint8 code_bytes[] = {
        OP_ADCA_D,
        lower_byte_address
    };
    uint8 data_bytes[] = {
        2
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
        { S_POINTER + lower_byte_address, data_bytes, 1 }
    };
    load_memory(test_memory, 2);
    e_cpu_context.d.byte_acc.a = 1;

    int cycles = run_cycles(opcode_table[OP_ADCA_D].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x3);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADCA_D].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void adcb_direct_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;
    uint8 lower_byte_address = 0x40;
    e_cpu_context.dp = S_POINTER >> 8;

    uint8 code_bytes[] = {
        OP_ADCB_D,
        lower_byte_address
    };
    uint8 data_bytes[] = {
        2
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
        { S_POINTER + lower_byte_address, data_bytes, 1 }
    };
    load_memory(test_memory, 2);
    e_cpu_context.d.byte_acc.b = 1;

    int cycles = run_cycles(opcode_table[OP_ADCB_D].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x3);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADCB_D].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void adda_direct_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;
    uint8 lower_byte_address = 0x40;
    e_cpu_context.dp = S_POINTER >> 8;

    uint8 code_bytes[] = {
        OP_ADDA_D,
        lower_byte_address
    };
    uint8 data_bytes[] = {
        2
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
        { S_POINTER + lower_byte_address, data_bytes, 1 }
    };
    load_memory(test_memory, 2);
    e_cpu_context.d.byte_acc.a = 1;

    int cycles = run_cycles(opcode_table[OP_ADDA_D].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x3);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADDA_D].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void addb_direct_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;
    uint8 lower_byte_address = 0x40;
    e_cpu_context.dp = S_POINTER >> 8;

    uint8 code_bytes[] = {
        OP_ADDB_D,
        lower_byte_address
    };
    uint8 data_bytes[] = {
        2
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
        { S_POINTER + lower_byte_address, data_bytes, 1 }
    };
    load_memory(test_memory, 2);
    e_cpu_context.d.byte_acc.b = 1;

    int cycles = run_cycles(opcode_table[OP_ADDB_D].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x3);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADDB_D].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void addd_direct_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    uint8 lower_byte_address = 0x40;
    e_cpu_context.dp = S_POINTER >> 8;

    uint8 code_bytes[] = {
        OP_ADDD_D,
        lower_byte_address
    };
    uint8 data_bytes[] = {
        0x12,
        0x34
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
        { S_POINTER + lower_byte_address, data_bytes, 2 }
    };
    load_memory(test_memory, 2);
    e_cpu_context.d.d = 1;

    int cycles = run_cycles(opcode_table[OP_ADDD_D].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.d, 0x1235);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADDD_D].cycle_count);
    assert_int_equal(post_pc, pre_pc + 2);
}

void anda_direct_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;

    uint8 lower_byte_address = 0x40;
    e_cpu_context.dp = S_POINTER >> 8;

    uint8 code_bytes[] = {
        OP_ANDA_D,
        lower_byte_address
    };
    uint8 data_bytes[] = {
        0x5
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
        { S_POINTER + lower_byte_address, data_bytes, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.d.byte_acc.a = 6;

    int cycles = run_cycles(opcode_table[OP_ANDA_D].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 4);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ANDA_D].cycle_count);
    assert_int_equal(post_pc, pre_pc + 2);
}

void andb_direct_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;

    uint8 lower_byte_address = 0x40;
    e_cpu_context.dp = S_POINTER >> 8;

    uint8 code_bytes[] = {
        OP_ANDB_D,
        lower_byte_address
    };
    uint8 data_bytes[] = {
        0x5
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
        { S_POINTER + lower_byte_address, data_bytes, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.d.byte_acc.b = 6;

    int cycles = run_cycles(opcode_table[OP_ANDB_D].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 4);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ANDB_D].cycle_count);
    assert_int_equal(post_pc, pre_pc + 2);
}

void asl_direct_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    uint8 lower_byte_address = 0x40;
    e_cpu_context.dp = S_POINTER >> 8;

    uint8 code_bytes[] = {
        OP_ASL_D,
        lower_byte_address
    };
    uint8 data_bytes[] = {
        0x2
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
        { S_POINTER + lower_byte_address, data_bytes, 1 }
    };
    load_memory(test_memory, 2);

    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = run_cycles(opcode_table[OP_ASL_D].cycle_count);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_ASL_D].cycle_count);
    assert_int_equal(read_byte_from_memory(S_POINTER + lower_byte_address),
                     0x4);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void asr_direct_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    uint8 lower_byte_address = 0x40;
    e_cpu_context.dp = S_POINTER >> 8;

    uint8 code_bytes[] = {
        OP_ASR_D,
        lower_byte_address
    };
    uint8 data_bytes[] = {
        0x2
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
        { S_POINTER + lower_byte_address, data_bytes, 1 }
    };
    load_memory(test_memory, 2);

    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = run_cycles(opcode_table[OP_ASR_D].cycle_count);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_ASR_D].cycle_count);
    assert_int_equal(read_byte_from_memory(S_POINTER + lower_byte_address),
                     0x1);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_true(post_pc > pre_pc);
}

void bita_direct_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;
    uint8 lower_byte_address = 0x40;
    e_cpu_context.dp = S_POINTER >> 8;

    uint8 code_bytes[] = {
        OP_BITA_D,
        lower_byte_address
    };
    uint8 data_bytes[] = {
        0x5
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
        { S_POINTER + lower_byte_address, data_bytes, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.d.byte_acc.a = 6;

    int cycles = run_cycles(opcode_table[OP_BITA_D].cycle_count);
    int post_pc = e_cpu_context.pc;
    /* The accumulator should not get changed by this operation */
    assert_int_equal(e_cpu_context.d.byte_acc.a, 6);
    assert_int_equal(read_byte_from_memory(S_POINTER + lower_byte_address),
                     0x5);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_BITA_D].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void bitb_direct_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;
    uint8 lower_byte_address = 0x40;
    e_cpu_context.dp = S_POINTER >> 8;

    uint8 code_bytes[] = {
        OP_BITB_D,
        lower_byte_address
    };
    uint8 data_bytes[] = {
        0x5
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
        { S_POINTER + lower_byte_address, data_bytes, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.d.byte_acc.b = 6;

    int cycles = run_cycles(opcode_table[OP_BITB_D].cycle_count);
    int post_pc = e_cpu_context.pc;
    /* The accumulator should not get changed by this operation */
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(read_byte_from_memory(S_POINTER + lower_byte_address),
                     0x5);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x6);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_BITB_D].cycle_count);
    assert_int_equal(post_pc, pre_pc + 2);
}

void clr_direct_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    uint8 lower_byte_address = 0x40;
    e_cpu_context.dp = S_POINTER >> 8;

    write_byte_to_memory(S_POINTER + lower_byte_address, 0xBB);
    assert_int_equal(read_byte_from_memory(S_POINTER + lower_byte_address),
                     0xBB);

    uint8 code_bytes[] = {
        OP_CLR,
        lower_byte_address
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);

    e_cpu_context.d.byte_acc.a = 1;
    e_cpu_context.d.byte_acc.b = 8;

    int cycles = run_cycles(opcode_table[OP_CLR].cycle_count);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_CLR].cycle_count);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 1);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 8);
    assert_int_equal(read_byte_from_memory(S_POINTER + lower_byte_address), 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(cycles, opcode_table[OP_CLR].cycle_count);
    assert_int_equal(post_pc, pre_pc + 2);
}

void cmpa_direct_nocarry_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;

    uint8 lower_byte_address = 0x40;
    e_cpu_context.dp = S_POINTER >> 8;
    uint8 code_bytes[] = {
        OP_CMPA_D,
        lower_byte_address
    };
    uint8 data_bytes[] = {
        0x11
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
        { S_POINTER + lower_byte_address, data_bytes, 1 }
    };
    load_memory(test_memory, 2);
    e_cpu_context.d.byte_acc.a = 0xFF;

    int cycles = run_cycles(opcode_table[OP_CMPA_D].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_CMPA_D].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void cmpb_direct_nocarry_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;

    uint8 lower_byte_address = 0x40;
    e_cpu_context.dp = S_POINTER >> 8;
    uint8 code_bytes[] = {
        OP_CMPB_D,
        lower_byte_address
    };
    uint8 data_bytes[] = {
        0x11
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
        { S_POINTER + lower_byte_address, data_bytes, 1 }
    };
    load_memory(test_memory, 2);
    e_cpu_context.d.byte_acc.b = 0xFF;

    int cycles = run_cycles(opcode_table[OP_CMPB_D].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_CMPB_D].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}

void cmpd_direct_nocarry_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    uint8 lower_byte_address = 0x40;
    e_cpu_context.dp = S_POINTER >> 8;
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_CMPD_D,
        lower_byte_address
    };
    uint8 data_bytes[] = {
        0x12,
        0x34
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 },
        { S_POINTER + lower_byte_address, data_bytes, 2 }
    };
    load_memory(test_memory, 2);
    e_cpu_context.d.d = 0xFFFF;

    int cycles = run_cycles(opcode_ext_x10_table[OP_CMPD_D].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.d, 0xFFFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_CMPD_D].cycle_count);
    assert_true(post_pc == pre_pc + 3);
}

void cmps_direct_nocarry_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    uint8 lower_byte_address = 0x40;
    e_cpu_context.dp = S_POINTER >> 8;
    uint8 code_bytes[] = {
        OP_EXTENDED_X11,
        OP_CMPS_D,
        lower_byte_address
    };
    uint8 data_bytes[] = {
        0x12,
        0x34
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 },
        { S_POINTER + lower_byte_address, data_bytes, 2 }
    };
    load_memory(test_memory, 2);
    e_cpu_context.s = 0xFFFF;

    int cycles = run_cycles(opcode_ext_x11_table[OP_CMPS_D].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.s, 0xFFFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_ext_x11_table[OP_CMPS_D].cycle_count);
    assert_true(post_pc == pre_pc + 3);
}

void cmpu_direct_nocarry_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    uint8 lower_byte_address = 0x40;
    e_cpu_context.dp = S_POINTER >> 8;
    uint8 code_bytes[] = {
        OP_EXTENDED_X11,
        OP_CMPU_D,
        lower_byte_address
    };
    uint8 data_bytes[] = {
        0x7A,
        0x7A
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { S_POINTER + lower_byte_address, data_bytes, 2 }
    };

    load_memory(test_memory, 2);
    e_cpu_context.u = 0x2727;

    int cycles = run_cycles(opcode_ext_x11_table[OP_CMPU_D].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.u, 0x2727);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_ext_x11_table[OP_CMPU_D].cycle_count);
    assert_true(post_pc == pre_pc + 3);
}

void cmpx_direct_overflow_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    uint8 lower_byte_address = 0x40;
    e_cpu_context.dp = S_POINTER >> 8;
    uint8 code_bytes[] = {
        OP_CMPX_D,
        lower_byte_address
    };
    uint8 data_bytes[] = {
        0xEE,
        0xEE
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
        { S_POINTER + lower_byte_address, data_bytes, 2 }
    };

    load_memory(test_memory, 2);
    e_cpu_context.x = 0xFFFF;

    int cycles = run_cycles(opcode_table[OP_CMPX_D].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.x, 0xFFFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_table[OP_CMPX_D].cycle_count);
    assert_true(post_pc == pre_pc + 2);
}
