#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "core.h"

extern struct cpu_state e_cpu_context;
extern struct opcode_def opcode_table[];
extern struct opcode_def opcode_ext_x10_table[];
extern struct opcode_def opcode_ext_x11_table[];

void adca_indexed_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;
    uint8 lower_byte_offset = 0x40;

    uint8 code_bytes[] = {
        OP_ADCA_I,
        0x8C, /* Constant 8-bit offset from PC */
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        2
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { USER_SPACE_ROOT + 3 + lower_byte_offset, data_bytes, 1 }
    };
    load_memory(test_memory, 2);
    e_cpu_context.d.byte_acc.a = 1;

    int cycles = run_cycles(opcode_table[OP_ADCA_I].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x3);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADCA_I].cycle_count + 1);
    assert_true(post_pc == pre_pc + 3);
}

void adcb_indexed_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;
    uint8 lower_byte_offset = 0x40;

    uint8 code_bytes[] = {
        OP_ADCB_I,
        0x8C, /* Constant 8-bit offset from PC */
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        2
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { USER_SPACE_ROOT + 3 + lower_byte_offset, data_bytes, 1 }
    };
    load_memory(test_memory, 2);
    e_cpu_context.d.byte_acc.b = 1;

    int cycles = run_cycles(opcode_table[OP_ADCB_I].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x3);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADCB_I].cycle_count + 1);
    assert_int_equal(post_pc, pre_pc + 3);
}

void adda_indexed_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;
    uint8 lower_byte_offset = 0x40;

    uint8 code_bytes[] = {
        OP_ADDA_I,
        0x8C, /* Constant 8-bit offset from PC */
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        2
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { USER_SPACE_ROOT + 3 + lower_byte_offset, data_bytes, 1 }
    };
    load_memory(test_memory, 2);
    e_cpu_context.d.byte_acc.a = 1;

    int cycles = run_cycles(opcode_table[OP_ADDA_I].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x3);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADDA_I].cycle_count + 1);
    assert_true(post_pc == pre_pc + 3);
}

void addb_indexed_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;
    uint8 lower_byte_offset = 0x40;

    uint8 code_bytes[] = {
        OP_ADDB_I,
        0x8C, /* Constant 8-bit offset from PC */
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        2
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { USER_SPACE_ROOT + 3 + lower_byte_offset, data_bytes, 1 }
    };
    load_memory(test_memory, 2);
    e_cpu_context.d.byte_acc.b = 1;

    int cycles = run_cycles(opcode_table[OP_ADDB_I].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x3);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADDB_I].cycle_count + 1);
    assert_true(post_pc == pre_pc + 3);
}

void addd_indexed_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    uint8 lower_byte_offset = 0x40;

    uint8 code_bytes[] = {
        OP_ADDD_I,
        0x8C, /* Constant 8-bit offset from PC */
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        0x12,
        0x34
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { USER_SPACE_ROOT + 3 + lower_byte_offset, data_bytes, 2 }
    };
    load_memory(test_memory, 2);
    e_cpu_context.d.d = 1;

    int cycles = run_cycles(opcode_table[OP_ADDD_I].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.d, 0x1235);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.h, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ADDD_I].cycle_count + 1);
    assert_int_equal(post_pc, pre_pc + 3);
}

void anda_indexed_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;

    uint8 lower_byte_offset = 0x40;

    uint8 code_bytes[] = {
        OP_ANDA_I,
        0x8C, /* Constant 8-bit offset from PC */
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        0x5
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { USER_SPACE_ROOT + 3 + lower_byte_offset, data_bytes, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.d.byte_acc.a = 6;

    int cycles = run_cycles(opcode_table[OP_ANDA_I].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 4);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ANDA_I].cycle_count + 1);
    assert_int_equal(post_pc, pre_pc + 3);
}

void andb_indexed_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;

    uint8 lower_byte_offset = 0x40;

    uint8 code_bytes[] = {
        OP_ANDB_I,
        0x8C, /* Constant 8-bit offset from PC */
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        0x5
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { USER_SPACE_ROOT + 3 + lower_byte_offset, data_bytes, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.d.byte_acc.b = 6;

    int cycles = run_cycles(opcode_table[OP_ANDB_I].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 4);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_ANDB_I].cycle_count + 1);
    assert_int_equal(post_pc, pre_pc + 3);
}

void asl_indexed_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    uint8 lower_byte_offset = 0x40;

    uint8 code_bytes[] = {
        OP_ASL_I,
        0x8C,
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        0x2
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { USER_SPACE_ROOT + 3 + lower_byte_offset, data_bytes, 1 }
    };
    load_memory(test_memory, 2);

    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = run_cycles(opcode_table[OP_ASL_I].cycle_count);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_ASL_I].cycle_count + 1);
    assert_int_equal(read_byte_from_memory(USER_SPACE_ROOT + 3 + lower_byte_offset),
                     0x4);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(post_pc, pre_pc + 3);
}

void asr_indexed_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    uint8 lower_byte_offset = 0x40;

    uint8 code_bytes[] = {
        OP_ASR_I,
        0x8C,
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        0x2
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { USER_SPACE_ROOT + 3 + lower_byte_offset, data_bytes, 1 }
    };
    load_memory(test_memory, 2);

    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 8;
    int cycles = run_cycles(opcode_table[OP_ASR_I].cycle_count);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_ASR_I].cycle_count + 1);
    assert_int_equal(read_byte_from_memory(USER_SPACE_ROOT + 3 + lower_byte_offset),
                     0x1);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x8);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(post_pc, pre_pc + 3);
}

void bita_indexed_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;
    uint8 lower_byte_offset = 0x40;

    uint8 code_bytes[] = {
        OP_BITA_I,
        0x8C,
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        0x5
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { USER_SPACE_ROOT + 3 + lower_byte_offset, data_bytes, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.d.byte_acc.a = 6;

    int cycles = run_cycles(opcode_table[OP_BITA_I].cycle_count);
    int post_pc = e_cpu_context.pc;
    /* The accumulator should not get changed by this operation */
    assert_int_equal(e_cpu_context.d.byte_acc.a, 6);
    assert_int_equal(read_byte_from_memory(USER_SPACE_ROOT + 3 + lower_byte_offset),
                     0x5);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_BITA_I].cycle_count + 1);
    assert_int_equal(post_pc, pre_pc + 3);
}

void bitb_indexed_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;
    uint8 lower_byte_offset = 0x40;

    uint8 code_bytes[] = {
        OP_BITB_I,
        0x8C,
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        0x5
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { USER_SPACE_ROOT + 3 + lower_byte_offset, data_bytes, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.d.byte_acc.b = 6;

    int cycles = run_cycles(opcode_table[OP_BITB_I].cycle_count);
    int post_pc = e_cpu_context.pc;
    /* The accumulator should not get changed by this operation */
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(read_byte_from_memory(USER_SPACE_ROOT + 3 + lower_byte_offset),
                     0x5);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0x6);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_BITB_I].cycle_count + 1);
    assert_int_equal(post_pc, pre_pc + 3);
}

void clr_indexed_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    uint8 lower_byte_offset = 0x40;

    write_byte_to_memory(USER_SPACE_ROOT + 3 + lower_byte_offset, 0xBB);
    assert_int_equal(read_byte_from_memory(USER_SPACE_ROOT + 3 + lower_byte_offset),
                     0xBB);

    uint8 code_bytes[] = {
        OP_CLR_I,
        0x8C,
        lower_byte_offset
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 }
    };
    load_memory(test_memory, 1);

    e_cpu_context.d.byte_acc.a = 1;
    e_cpu_context.d.byte_acc.b = 8;

    int cycles = run_cycles(opcode_table[OP_CLR_I].cycle_count);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_CLR_I].cycle_count + 1);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 1);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 8);
    assert_int_equal(read_byte_from_memory(USER_SPACE_ROOT + 3 + lower_byte_offset), 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(post_pc, pre_pc + 3);
}

void cmpa_indexed_nocarry_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;

    uint8 lower_byte_offset = 0x40;
    uint8 code_bytes[] = {
        OP_CMPA_I,
        0x8C,
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        0x11
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { USER_SPACE_ROOT + 3 + lower_byte_offset, data_bytes, 1 }
    };
    load_memory(test_memory, 2);
    e_cpu_context.d.byte_acc.a = 0xFF;

    int cycles = run_cycles(opcode_table[OP_CMPA_I].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_CMPA_I].cycle_count + 1);
    assert_int_equal(post_pc, pre_pc + 3);
}

void cmpb_indexed_nocarry_test(void **state) {
    (void) state; /* unused */

    int pre_pc = e_cpu_context.pc;
    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;

    uint8 lower_byte_offset = 0x40;
    uint8 code_bytes[] = {
        OP_CMPB_I,
        0x8C,
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        0x11
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { USER_SPACE_ROOT + 3 + lower_byte_offset, data_bytes, 1 }
    };
    load_memory(test_memory, 2);
    e_cpu_context.d.byte_acc.b = 0xFF;

    int cycles = run_cycles(opcode_table[OP_CMPB_I].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_CMPB_I].cycle_count + 1);
    assert_int_equal(post_pc, pre_pc + 3);
}

void cmpd_indexed_nocarry_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    uint8 lower_byte_offset = 0x40;
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_CMPD_I,
        0x8C,
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        0x12,
        0x34
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 },
        { USER_SPACE_ROOT + 4 + lower_byte_offset, data_bytes, 2 }
    };
    load_memory(test_memory, 2);
    e_cpu_context.d.d = 0xFFFF;

    int cycles = run_cycles(opcode_ext_x10_table[OP_CMPD_I].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.d, 0xFFFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_CMPD_I].cycle_count + 1);
    assert_int_equal(post_pc, pre_pc + 4);
}

void cmps_indexed_nocarry_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    uint8 lower_byte_offset = 0x40;
    uint8 code_bytes[] = {
        OP_EXTENDED_X11,
        OP_CMPS_I,
        0x8C,
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        0x12,
        0x34
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 },
        { USER_SPACE_ROOT + 4 + lower_byte_offset, data_bytes, 2 }
    };
    load_memory(test_memory, 2);
    e_cpu_context.s = 0xFFFF;

    int cycles = run_cycles(opcode_ext_x11_table[OP_CMPS_I].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.s, 0xFFFF);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_ext_x11_table[OP_CMPS_I].cycle_count + 1);
    assert_int_equal(post_pc, pre_pc + 4);
}

void cmpu_indexed_nocarry_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    uint8 lower_byte_offset = 0x40;
    uint8 code_bytes[] = {
        OP_EXTENDED_X11,
        OP_CMPU_I,
        0x8C,
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        0x7A,
        0x7A
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 },
        { USER_SPACE_ROOT + 4 + lower_byte_offset, data_bytes, 2 }
    };

    load_memory(test_memory, 2);
    e_cpu_context.u = 0x2727;

    int cycles = run_cycles(opcode_ext_x11_table[OP_CMPU_I].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.u, 0x2727);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_ext_x11_table[OP_CMPU_I].cycle_count + 1);
    assert_int_equal(post_pc, pre_pc + 4);
}

void cmpx_indexed_overflow_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    uint8 lower_byte_offset = 0x40;
    uint8 code_bytes[] = {
        OP_CMPX_I,
        0x8C,
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        0xEE,
        0xEE
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { USER_SPACE_ROOT + 3 + lower_byte_offset, data_bytes, 2 }
    };

    load_memory(test_memory, 2);
    e_cpu_context.x = 0xFFFF;

    int cycles = run_cycles(opcode_table[OP_CMPX_I].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.x, 0xFFFF);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 1);
    assert_int_equal(cycles, opcode_table[OP_CMPX_I].cycle_count + 1);
    assert_int_equal(post_pc, pre_pc + 3);
}

void cmpy_indexed_zero_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    uint8 lower_byte_offset = 0x40;
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_CMPY_I,
        0x8C,
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        0x69,
        0x69
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 },
        { USER_SPACE_ROOT + 4 + lower_byte_offset, data_bytes, 2 }
    };

    load_memory(test_memory, 2);
    e_cpu_context.y = 0x6969;

    int cycles = run_cycles(opcode_ext_x10_table[OP_CMPY_I].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.y, 0x6969);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.z, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_CMPY_I].cycle_count + 1);
    assert_int_equal(post_pc, pre_pc + 4);
}

void com_indexed_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    uint8 lower_byte_offset = 0x40;
    uint8 code_bytes[] = {
        OP_COM_I,
        0x8C,
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        0x1
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { USER_SPACE_ROOT + 3 + lower_byte_offset, data_bytes, 1 }
    };
    load_memory(test_memory, 2);

    int cycles = run_cycles(opcode_table[OP_COM_I].cycle_count);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_COM_I].cycle_count + 1);
    assert_int_equal(read_byte_from_memory(USER_SPACE_ROOT + 3 + lower_byte_offset),
                     0xFE);
    assert_int_equal(e_cpu_context.cc.c, 1);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(e_cpu_context.cc.n, 1);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(post_pc, pre_pc + 3);
}

void dec_indexed_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    uint8 lower_byte_offset = 0x40;
    uint8 code_bytes[] = {
        OP_DEC_I,
        0x8C,
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        0x45
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { USER_SPACE_ROOT + 3 + lower_byte_offset, data_bytes, 1 }
    };
    load_memory(test_memory, 2);

    int cycles = run_cycles(opcode_table[OP_DEC_I].cycle_count);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_DEC_I].cycle_count + 1);
    assert_int_equal(read_byte_from_memory(USER_SPACE_ROOT + 3 + lower_byte_offset),
                     0x44);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(post_pc, pre_pc + 3);
}

void eora_indexed_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    uint8 lower_byte_offset = 0x40;
    uint8 code_bytes[] = {
        OP_EORA_I,
        0x8C,
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        0x5
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { USER_SPACE_ROOT + 3 + lower_byte_offset, data_bytes, 1 }
    };
    load_memory(test_memory, 2);

    /* b shouldn't be messed with */
    e_cpu_context.d.byte_acc.b = 0xFF;
    e_cpu_context.d.byte_acc.a = 4;

    int cycles = run_cycles(opcode_table[OP_EORA_I].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 1);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_EORA_I].cycle_count + 1);
    assert_int_equal(post_pc, pre_pc + 3);
}

void eorb_indexed_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    uint8 lower_byte_offset = 0x40;
    uint8 code_bytes[] = {
        OP_EORB_I,
        0x8C,
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        0x5
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { USER_SPACE_ROOT + 3 + lower_byte_offset, data_bytes, 1 }
    };
    load_memory(test_memory, 2);

    /* a shouldn't be messed with */
    e_cpu_context.d.byte_acc.a = 0xFF;
    e_cpu_context.d.byte_acc.b = 4;

    int cycles = run_cycles(opcode_table[OP_EORB_I].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 1);
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(cycles, opcode_table[OP_EORB_I].cycle_count + 1);
    assert_int_equal(post_pc, pre_pc + 3);
}

void inc_indexed_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    uint8 lower_byte_offset = 0x40;
    uint8 code_bytes[] = {
        OP_INC_I,
        0x8C,
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        0x45
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { USER_SPACE_ROOT + 3 + lower_byte_offset, data_bytes, 1 }
    };
    load_memory(test_memory, 2);

    int cycles = run_cycles(opcode_table[OP_INC_I].cycle_count);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_INC_I].cycle_count + 1);
    assert_int_equal(read_byte_from_memory(USER_SPACE_ROOT + 3 + lower_byte_offset),
                     0x46);
    assert_int_equal(e_cpu_context.cc.n, 0);
    assert_int_equal(e_cpu_context.cc.z, 0);
    assert_int_equal(e_cpu_context.cc.v, 0);
    assert_int_equal(post_pc, pre_pc + 3);
}

void jmp_indexed_test(void **state) {
    (void) state; /* unused */

    uint8 lower_byte_offset = 0x40;
    uint8 code_bytes[] = {
        OP_JMP_I,
        0x8C,
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        0x45,
        0x45
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { USER_SPACE_ROOT + 3 + lower_byte_offset, data_bytes, 2 }
    };
    load_memory(test_memory, 2);

    int cycles = run_cycles(opcode_table[OP_JMP_I].cycle_count);
    int post_pc = e_cpu_context.pc;

    assert_int_equal(cycles, opcode_table[OP_JMP_I].cycle_count + 1);
    assert_int_equal(post_pc, USER_SPACE_ROOT + 3 + lower_byte_offset);
}

void jsr_indexed_test(void **state) {
    (void) state; /* unused */

    uint16 pre_stack_pointer = e_cpu_context.s;
    uint8 lower_byte_offset = 0x40;
    uint8 code_bytes[] = {
        OP_JSR_I,
        0x8C,
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        0x45,
        0x45
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { USER_SPACE_ROOT + 3 + lower_byte_offset, data_bytes, 2 }
    };
    load_memory(test_memory, 2);

    int cycles = run_cycles(opcode_table[OP_JSR_I].cycle_count);
    int post_pc = e_cpu_context.pc;
    uint16 post_stack_pointer = e_cpu_context.s;

    assert_int_equal(cycles, opcode_table[OP_JSR_I].cycle_count + 1);
    assert_int_equal(post_pc, USER_SPACE_ROOT + 3 + lower_byte_offset);
    assert_int_equal(post_stack_pointer, pre_stack_pointer - 2);
    assert_int_equal(read_word_from_memory(post_stack_pointer),
                     USER_SPACE_ROOT + 3);
}

void lda_indexed_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;

    uint8 lower_byte_offset = 0x40;
    uint8 code_bytes[] = {
        OP_LDA_I,
        0x8C,
        lower_byte_offset
    };
    uint8 data_bytes[] = {
        0x1F
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { USER_SPACE_ROOT + 3 + lower_byte_offset, data_bytes, 1 }
    };
    load_memory(test_memory, 2);

    int cycles = run_cycles(opcode_table[OP_LDA_I].cycle_count);
    int post_pc = e_cpu_context.pc;
    assert_int_equal(e_cpu_context.d.byte_acc.a, 0x1F);
    assert_int_equal(e_cpu_context.d.byte_acc.b, 0);
    assert_int_equal(cycles, opcode_table[OP_LDA_I].cycle_count + 1);
    assert_int_equal(post_pc, pre_pc + 3);
}

/* void ldb_indexed_test(void **state) { */
/*     (void) state; /\* unused *\/ */
/*     int pre_pc = e_cpu_context.pc; */

/*     uint8 lower_byte_offset = 0x40; */
/*     uint8 code_bytes[] = { */
/*         OP_LDB_I, */
/*         S_POINTER >> 8, */
/*         lower_byte_offset */
/*     }; */
/*     uint8 data_bytes[] = { */
/*         0x1F */
/*     }; */
/*     struct mem_loader_def test_memory[] = { */
/*         { USER_SPACE_ROOT, code_bytes, 3 }, */
/*         { S_POINTER + lower_byte_offset, data_bytes, 1 } */
/*     }; */
/*     load_memory(test_memory, 2); */

/*     int cycles = run_cycles(opcode_table[OP_LDB_I].cycle_count); */
/*     int post_pc = e_cpu_context.pc; */
/*     assert_int_equal(e_cpu_context.d.byte_acc.a, 0); */
/*     assert_int_equal(e_cpu_context.d.byte_acc.b, 0x1F); */
/*     assert_int_equal(cycles, opcode_table[OP_LDB_I].cycle_count); */
/*     assert_int_equal(post_pc, pre_pc + 3); */
/* } */

/* void ldd_indexed_test(void **state) { */
/*     (void) state; /\* unused *\/ */
/*     int pre_pc = e_cpu_context.pc; */

/*     uint8 lower_byte_offset = 0x40; */
/*     uint8 code_bytes[] = { */
/*         OP_LDD_I, */
/*         S_POINTER >> 8, */
/*         lower_byte_offset */
/*     }; */
/*     uint8 data_bytes[] = { */
/*         0xF0, */
/*         0x0D */
/*     }; */
/*     struct mem_loader_def test_memory[] = { */
/*         { USER_SPACE_ROOT, code_bytes, 3 }, */
/*         { S_POINTER + lower_byte_offset, data_bytes, 2 } */
/*     }; */
/*     load_memory(test_memory, 2); */

/*     int cycles = run_cycles(opcode_table[OP_LDD_I].cycle_count); */
/*     int post_pc = e_cpu_context.pc; */
/*     assert_int_equal(e_cpu_context.d.d, 0xF00D); */
/*     assert_int_equal(cycles, opcode_table[OP_LDD_I].cycle_count); */
/*     assert_true(post_pc == pre_pc + 3); */
/* } */

/* void lds_indexed_test(void **state) { */
/*     (void) state; /\* unused *\/ */
/*     int pre_pc = e_cpu_context.pc; */

/*     uint8 lower_byte_offset = 0x40; */
/*     uint8 code_bytes[] = { */
/*         OP_IXTENDED_X10, */
/*         OP_LDS_I, */
/*         S_POINTER >> 8, */
/*         lower_byte_offset */
/*     }; */
/*     uint8 data_bytes[] = { */
/*         0xF0, */
/*         0x0D */
/*     }; */
/*     struct mem_loader_def test_memory[] = { */
/*         { USER_SPACE_ROOT, code_bytes, 4 }, */
/*         { S_POINTER + lower_byte_offset, data_bytes, 2 } */
/*     }; */
/*     load_memory(test_memory, 2); */

/*     int cycles = run_cycles(opcode_ext_x10_table[OP_LDS_I].cycle_count); */
/*     int post_pc = e_cpu_context.pc; */
/*     assert_int_equal(e_cpu_context.s, 0xF00D); */
/*     assert_int_equal(cycles, opcode_ext_x10_table[OP_LDS_I].cycle_count); */
/*     assert_true(post_pc == pre_pc + 4); */
/* } */

/* void ldu_indexed_test(void **state) { */
/*     (void) state; /\* unused *\/ */
/*     int pre_pc = e_cpu_context.pc; */

/*     uint8 lower_byte_offset = 0x40; */
/*     uint8 code_bytes[] = { */
/*         OP_LDU_I, */
/*         S_POINTER >> 8, */
/*         lower_byte_offset */
/*     }; */
/*     uint8 data_bytes[] = { */
/*         0xF0, */
/*         0x0D */
/*     }; */
/*     struct mem_loader_def test_memory[] = { */
/*         { USER_SPACE_ROOT, code_bytes, 3 }, */
/*         { S_POINTER + lower_byte_offset, data_bytes, 2 } */
/*     }; */
/*     load_memory(test_memory, 2); */

/*     int cycles = run_cycles(opcode_table[OP_LDU_I].cycle_count); */
/*     int post_pc = e_cpu_context.pc; */
/*     assert_int_equal(e_cpu_context.u, 0xF00D); */
/*     assert_int_equal(cycles, opcode_table[OP_LDU_I].cycle_count); */
/*     assert_true(post_pc == pre_pc + 3); */
/* } */

/* void ldx_indexed_test(void **state) { */
/*     (void) state; /\* unused *\/ */
/*     int pre_pc = e_cpu_context.pc; */

/*     uint8 lower_byte_offset = 0x40; */
/*     uint8 code_bytes[] = { */
/*         OP_LDX_I, */
/*         S_POINTER >> 8, */
/*         lower_byte_offset */
/*     }; */
/*     uint8 data_bytes[] = { */
/*         0xF0, */
/*         0x0D */
/*     }; */
/*     struct mem_loader_def test_memory[] = { */
/*         { USER_SPACE_ROOT, code_bytes, 3 }, */
/*         { S_POINTER + lower_byte_offset, data_bytes, 2 } */
/*     }; */
/*     load_memory(test_memory, 2); */

/*     int cycles = run_cycles(opcode_table[OP_LDX_I].cycle_count); */
/*     int post_pc = e_cpu_context.pc; */
/*     assert_int_equal(e_cpu_context.x, 0xF00D); */
/*     assert_int_equal(cycles, opcode_table[OP_LDX_I].cycle_count); */
/*     assert_true(post_pc == pre_pc + 3); */
/* } */

/* void ldy_indexed_test(void **state) { */
/*     (void) state; /\* unused *\/ */
/*     int pre_pc = e_cpu_context.pc; */

/*     uint8 lower_byte_offset = 0x40; */
/*     uint8 code_bytes[] = { */
/*         OP_IXTENDED_X10, */
/*         OP_LDY_I, */
/*         S_POINTER >> 8, */
/*         lower_byte_offset */
/*     }; */
/*     uint8 data_bytes[] = { */
/*         0xF0, */
/*         0x0D */
/*     }; */
/*     struct mem_loader_def test_memory[] = { */
/*         { USER_SPACE_ROOT, code_bytes, 4 }, */
/*         { S_POINTER + lower_byte_offset, data_bytes, 2 } */
/*     }; */
/*     load_memory(test_memory, 2); */

/*     int cycles = run_cycles(opcode_ext_x10_table[OP_LDY_I].cycle_count); */
/*     int post_pc = e_cpu_context.pc; */
/*     assert_int_equal(e_cpu_context.y, 0xF00D); */
/*     assert_int_equal(cycles, opcode_ext_x10_table[OP_LDY_I].cycle_count); */
/*     assert_true(post_pc == pre_pc + 4); */
/* } */

/* void neg_indexed_test(void **state) { */
/*     (void) state; /\* unused *\/ */
/*     int pre_pc = e_cpu_context.pc; */

/*     uint8 lower_byte_offset = 0x40; */
/*     uint8 code_bytes[] = { */
/*         OP_NEG_I, */
/*         S_POINTER >> 8, */
/*         lower_byte_offset */
/*     }; */
/*     uint8 data_bytes[] = { */
/*         0x1 */
/*     }; */
/*     struct mem_loader_def test_memory[] = { */
/*         { USER_SPACE_ROOT, code_bytes, 3 }, */
/*         { S_POINTER + lower_byte_offset, data_bytes, 1 } */
/*     }; */
/*     load_memory(test_memory, 2); */

/*     int cycles = run_cycles(opcode_table[OP_NEG_I].cycle_count); */
/*     int post_pc = e_cpu_context.pc; */

/*     assert_int_equal(cycles, opcode_table[OP_NEG_I].cycle_count); */
/*     assert_int_equal(read_byte_from_memory(S_POINTER + lower_byte_offset), */
/*                      0xFF); */
/*     assert_int_equal(e_cpu_context.cc.c, 1); */
/*     assert_int_equal(e_cpu_context.cc.v, 0); */
/*     assert_int_equal(e_cpu_context.cc.n, 1); */
/*     assert_int_equal(e_cpu_context.cc.z, 0); */
/*     assert_true(post_pc == pre_pc + 3); */
/* } */

/* void ora_indexed_test(void **state) { */
/*     (void) state; /\* unused *\/ */
/*     int pre_pc = e_cpu_context.pc; */

/*     uint8 lower_byte_offset = 0x40; */
/*     uint8 code_bytes[] = { */
/*         OP_ORA_I, */
/*         S_POINTER >> 8, */
/*         lower_byte_offset */
/*     }; */
/*     uint8 data_bytes[] = { */
/*         0x5 */
/*     }; */
/*     struct mem_loader_def test_memory[] = { */
/*         { USER_SPACE_ROOT, code_bytes, 3 }, */
/*         { S_POINTER + lower_byte_offset, data_bytes, 1 } */
/*     }; */
/*     load_memory(test_memory, 2); */

/*     e_cpu_context.d.byte_acc.a = 8; */
/*     /\* b shouldn't be messed with *\/ */
/*     e_cpu_context.d.byte_acc.b = 0xFF; */

/*     int cycles = run_cycles(opcode_table[OP_ORA_I].cycle_count); */
/*     int post_pc = e_cpu_context.pc; */
/*     assert_int_equal(e_cpu_context.d.byte_acc.a, 0xD); */
/*     assert_int_equal(e_cpu_context.d.byte_acc.b, 0xFF); */
/*     assert_int_equal(e_cpu_context.cc.n, 0); */
/*     assert_int_equal(e_cpu_context.cc.z, 0); */
/*     assert_int_equal(e_cpu_context.cc.v, 0); */
/*     assert_int_equal(cycles, opcode_table[OP_ORA_I].cycle_count); */
/*     assert_true(post_pc == pre_pc + 3); */
/* } */

/* void orb_indexed_test(void **state) { */
/*     (void) state; /\* unused *\/ */
/*     int pre_pc = e_cpu_context.pc; */

/*     uint8 lower_byte_offset = 0x40; */
/*     uint8 code_bytes[] = { */
/*         OP_ORB_I, */
/*         S_POINTER >> 8, */
/*         lower_byte_offset */
/*     }; */
/*     uint8 data_bytes[] = { */
/*         0x5 */
/*     }; */
/*     struct mem_loader_def test_memory[] = { */
/*         { USER_SPACE_ROOT, code_bytes, 3 }, */
/*         { S_POINTER + lower_byte_offset, data_bytes, 1 } */
/*     }; */
/*     load_memory(test_memory, 2); */

/*     /\* a shouldn't be messed with *\/ */
/*     e_cpu_context.d.byte_acc.a = 0xFF; */
/*     e_cpu_context.d.byte_acc.b = 8; */

/*     int cycles = run_cycles(opcode_table[OP_ORB_I].cycle_count); */
/*     int post_pc = e_cpu_context.pc; */
/*     assert_int_equal(e_cpu_context.d.byte_acc.b, 0xD); */
/*     assert_int_equal(e_cpu_context.d.byte_acc.a, 0xFF); */
/*     assert_int_equal(e_cpu_context.cc.n, 0); */
/*     assert_int_equal(e_cpu_context.cc.z, 0); */
/*     assert_int_equal(e_cpu_context.cc.v, 0); */
/*     assert_int_equal(cycles, opcode_table[OP_ORB_I].cycle_count); */
/*     assert_true(post_pc == pre_pc + 3); */
/* } */

/* void rol_indexed_test(void **state) { */
/*     (void) state; /\* unused *\/ */
/*     int pre_pc = e_cpu_context.pc; */

/*     uint8 lower_byte_offset = 0x40; */
/*     uint8 code_bytes[] = { */
/*         OP_ROL_I, */
/*         S_POINTER >> 8, */
/*         lower_byte_offset */
/*     }; */
/*     uint8 data_bytes[] = { */
/*         0x2 */
/*     }; */
/*     struct mem_loader_def test_memory[] = { */
/*         { USER_SPACE_ROOT, code_bytes, 3 }, */
/*         { S_POINTER + lower_byte_offset, data_bytes, 1 } */
/*     }; */
/*     load_memory(test_memory, 2); */

/*     int cycles = run_cycles(opcode_table[OP_ROL_I].cycle_count); */
/*     int post_pc = e_cpu_context.pc; */

/*     assert_int_equal(cycles, opcode_table[OP_ROL_I].cycle_count); */
/*     assert_int_equal(read_byte_from_memory(S_POINTER + lower_byte_offset), */
/*                      0x4); */
/*     assert_int_equal(e_cpu_context.cc.c, 0); */
/*     assert_int_equal(e_cpu_context.cc.n, 0); */
/*     assert_int_equal(e_cpu_context.cc.z, 0); */
/*     assert_int_equal(e_cpu_context.cc.v, 0); */
/*     assert_true(post_pc == pre_pc + 3); */
/* } */

/* void ror_indexed_test(void **state) { */
/*     (void) state; /\* unused *\/ */
/*     int pre_pc = e_cpu_context.pc; */

/*     uint8 lower_byte_offset = 0x40; */
/*     uint8 code_bytes[] = { */
/*         OP_ROR_I, */
/*         S_POINTER >> 8, */
/*         lower_byte_offset */
/*     }; */
/*     uint8 data_bytes[] = { */
/*         0x2 */
/*     }; */
/*     struct mem_loader_def test_memory[] = { */
/*         { USER_SPACE_ROOT, code_bytes, 3 }, */
/*         { S_POINTER + lower_byte_offset, data_bytes, 1 } */
/*     }; */
/*     load_memory(test_memory, 2); */

/*     int cycles = run_cycles(opcode_table[OP_ROR_I].cycle_count); */
/*     int post_pc = e_cpu_context.pc; */

/*     assert_int_equal(cycles, opcode_table[OP_ROR_I].cycle_count); */
/*     assert_int_equal(read_byte_from_memory(S_POINTER + lower_byte_offset), */
/*                      0x1); */
/*     assert_int_equal(e_cpu_context.cc.c, 0); */
/*     assert_int_equal(e_cpu_context.cc.n, 0); */
/*     assert_int_equal(e_cpu_context.cc.z, 0); */
/*     assert_int_equal(e_cpu_context.cc.v, 0); */
/*     assert_true(post_pc == pre_pc + 3); */
/* } */

/* void sbca_indexed_test(void **state) { */
/*     (void) state; /\* unused *\/ */
/*     int pre_pc = e_cpu_context.pc; */

/*     uint8 lower_byte_offset = 0x40; */
/*     uint8 code_bytes[] = { */
/*         OP_SBCA_I, */
/*         S_POINTER >> 8, */
/*         lower_byte_offset */
/*     }; */
/*     uint8 data_bytes[] = { */
/*         0x4 */
/*     }; */
/*     struct mem_loader_def test_memory[] = { */
/*         { USER_SPACE_ROOT, code_bytes, 3 }, */
/*         { S_POINTER + lower_byte_offset, data_bytes, 1 } */
/*     }; */
/*     load_memory(test_memory, 2); */

/*     /\* b shouldn't be messed with *\/ */
/*     set_reg_value_8(REG_B, 0xFF); */

/*     e_cpu_context.cc.c = 1; */
/*     set_reg_value_8(REG_A, 6); */

/*     int cycles = run_cycles(opcode_table[OP_SBCA_I].cycle_count); */
/*     int post_pc = e_cpu_context.pc; */
/*     assert_int_equal(get_reg_value_8(REG_A), 1); */
/*     assert_int_equal(get_reg_value_8(REG_B), 0xFF); */
/*     assert_int_equal(e_cpu_context.cc.n, 0); */
/*     assert_int_equal(e_cpu_context.cc.c, 0); */
/*     assert_int_equal(e_cpu_context.cc.z, 0); */
/*     assert_int_equal(e_cpu_context.cc.v, 0); */
/*     assert_int_equal(cycles, opcode_table[OP_SBCA_I].cycle_count); */
/*     assert_true(post_pc == pre_pc + 3); */
/* } */

/* void sbcb_indexed_test(void **state) { */
/*     (void) state; /\* unused *\/ */
/*     int pre_pc = e_cpu_context.pc; */

/*     uint8 lower_byte_offset = 0x40; */
/*     uint8 code_bytes[] = { */
/*         OP_SBCB_I, */
/*         S_POINTER >> 8, */
/*         lower_byte_offset */
/*     }; */
/*     uint8 data_bytes[] = { */
/*         0x4 */
/*     }; */
/*     struct mem_loader_def test_memory[] = { */
/*         { USER_SPACE_ROOT, code_bytes, 3 }, */
/*         { S_POINTER + lower_byte_offset, data_bytes, 1 } */
/*     }; */
/*     load_memory(test_memory, 2); */

/*     /\* a shouldn't be messed with *\/ */
/*     set_reg_value_8(REG_A, 0xFF); */
/*     e_cpu_context.cc.c = 1; */
/*     set_reg_value_8(REG_B, 6); */

/*     int cycles = run_cycles(opcode_table[OP_SBCB_I].cycle_count); */
/*     int post_pc = e_cpu_context.pc; */
/*     assert_int_equal(get_reg_value_8(REG_B), 1); */
/*     assert_int_equal(get_reg_value_8(REG_A), 0xFF); */
/*     assert_int_equal(e_cpu_context.cc.n, 0); */
/*     assert_int_equal(e_cpu_context.cc.c, 0); */
/*     assert_int_equal(e_cpu_context.cc.z, 0); */
/*     assert_int_equal(e_cpu_context.cc.v, 0); */
/*     assert_int_equal(cycles, opcode_table[OP_SBCB_I].cycle_count); */
/*     assert_true(post_pc == pre_pc + 3); */
/* } */

/* void sta_indexed_test(void **state) { */
/*     (void) state; /\* unused *\/ */
/*     int pre_pc = e_cpu_context.pc; */

/*     uint8 lower_byte_offset = 0x40; */
/*     assert_int_equal(read_byte_from_memory(S_POINTER + lower_byte_offset), */
/*                      0); */
/*     uint8 code_bytes[] = { */
/*         OP_STA_I, */
/*         S_POINTER >> 8, */
/*         lower_byte_offset */
/*     }; */
/*     struct mem_loader_def test_memory[] = { */
/*         { USER_SPACE_ROOT, code_bytes, 3 } */
/*     }; */
/*     load_memory(test_memory, 1); */
/*     set_reg_value_8(REG_A, 0x69); */

/*     int cycles = run_cycles(opcode_table[OP_STA_I].cycle_count); */
/*     int post_pc = e_cpu_context.pc; */
/*     assert_int_equal(e_cpu_context.d.byte_acc.a, 0x69); */
/*     assert_int_equal(read_byte_from_memory(S_POINTER + lower_byte_offset), */
/*                      0x69); */
/*     assert_int_equal(cycles, opcode_table[OP_STA_I].cycle_count); */
/*     assert_int_equal(post_pc, pre_pc + 3); */
/* } */

/* void stb_indexed_test(void **state) { */
/*     (void) state; /\* unused *\/ */
/*     int pre_pc = e_cpu_context.pc; */

/*     uint8 lower_byte_offset = 0x40; */
/*     assert_int_equal(read_byte_from_memory(S_POINTER + lower_byte_offset), */
/*                      0); */
/*     uint8 code_bytes[] = { */
/*         OP_STB_I, */
/*         S_POINTER >> 8, */
/*         lower_byte_offset */
/*     }; */
/*     struct mem_loader_def test_memory[] = { */
/*         { USER_SPACE_ROOT, code_bytes, 3 } */
/*     }; */
/*     load_memory(test_memory, 1); */
/*     set_reg_value_8(REG_B, 0x69); */

/*     int cycles = run_cycles(opcode_table[OP_STB_I].cycle_count); */
/*     int post_pc = e_cpu_context.pc; */
/*     assert_int_equal(e_cpu_context.d.byte_acc.b, 0x69); */
/*     assert_int_equal(read_byte_from_memory(S_POINTER + lower_byte_offset), */
/*                      0x69); */
/*     assert_int_equal(cycles, opcode_table[OP_STB_I].cycle_count); */
/*     assert_int_equal(post_pc, pre_pc + 3); */
/* } */

/* void std_indexed_test(void **state) { */
/*     (void) state; /\* unused *\/ */
/*     int pre_pc = e_cpu_context.pc; */

/*     uint8 lower_byte_offset = 0x40; */
/*     assert_int_equal(read_word_from_memory(S_POINTER + lower_byte_offset), */
/*                      0); */
/*     uint8 code_bytes[] = { */
/*         OP_STD_I, */
/*         S_POINTER >> 8, */
/*         lower_byte_offset */
/*     }; */
/*     struct mem_loader_def test_memory[] = { */
/*         { USER_SPACE_ROOT, code_bytes, 3 } */
/*     }; */
/*     load_memory(test_memory, 1); */
/*     set_reg_value_16(REG_D, 0x1969); */

/*     int cycles = run_cycles(opcode_table[OP_STD_I].cycle_count); */
/*     int post_pc = e_cpu_context.pc; */
/*     assert_int_equal(get_reg_value_16(REG_D), 0x1969); */
/*     assert_int_equal(read_word_from_memory(S_POINTER + lower_byte_offset), */
/*                      0x1969); */
/*     assert_int_equal(cycles, opcode_table[OP_STD_I].cycle_count); */
/*     assert_int_equal(post_pc, pre_pc + 3); */
/* } */

/* void sts_indexed_test(void **state) { */
/*     (void) state; /\* unused *\/ */
/*     int pre_pc = e_cpu_context.pc; */

/*     uint8 lower_byte_offset = 0x40; */
/*     assert_int_equal(read_word_from_memory(S_POINTER + lower_byte_offset), */
/*                      0); */
/*     uint8 code_bytes[] = { */
/*         OP_IXTENDED_X10, */
/*         OP_STS_I, */
/*         S_POINTER >> 8, */
/*         lower_byte_offset */
/*     }; */
/*     struct mem_loader_def test_memory[] = { */
/*         { USER_SPACE_ROOT, code_bytes, 4 } */
/*     }; */
/*     load_memory(test_memory, 1); */
/*     set_reg_value_16(REG_S, 0x1969); */

/*     int cycles = run_cycles(opcode_ext_x10_table[OP_STS_I].cycle_count); */
/*     int post_pc = e_cpu_context.pc; */
/*     assert_int_equal(get_reg_value_16(REG_S), 0x1969); */
/*     assert_int_equal(read_word_from_memory(S_POINTER + lower_byte_offset), */
/*                      0x1969); */
/*     assert_int_equal(cycles, opcode_ext_x10_table[OP_STS_I].cycle_count); */
/*     assert_int_equal(post_pc, pre_pc + 4); */
/* } */

/* void stu_indexed_test(void **state) { */
/*     (void) state; /\* unused *\/ */
/*     int pre_pc = e_cpu_context.pc; */

/*     uint8 lower_byte_offset = 0x40; */
/*     assert_int_equal(read_word_from_memory(S_POINTER + lower_byte_offset), */
/*                      0); */
/*     uint8 code_bytes[] = { */
/*         OP_STU_I, */
/*         S_POINTER >> 8, */
/*         lower_byte_offset */
/*     }; */
/*     struct mem_loader_def test_memory[] = { */
/*         { USER_SPACE_ROOT, code_bytes, 3 } */
/*     }; */
/*     load_memory(test_memory, 1); */
/*     set_reg_value_16(REG_U, 0x1969); */

/*     int cycles = run_cycles(opcode_table[OP_STU_I].cycle_count); */
/*     int post_pc = e_cpu_context.pc; */
/*     assert_int_equal(get_reg_value_16(REG_U), 0x1969); */
/*     assert_int_equal(read_word_from_memory(S_POINTER + lower_byte_offset), */
/*                      0x1969); */
/*     assert_int_equal(cycles, opcode_table[OP_STU_I].cycle_count); */
/*     assert_int_equal(post_pc, pre_pc + 3); */
/* } */

/* void stx_indexed_test(void **state) { */
/*     (void) state; /\* unused *\/ */
/*     int pre_pc = e_cpu_context.pc; */

/*     uint8 lower_byte_offset = 0x40; */
/*     assert_int_equal(read_word_from_memory(S_POINTER + lower_byte_offset), */
/*                      0); */
/*     uint8 code_bytes[] = { */
/*         OP_STX_I, */
/*         S_POINTER >> 8, */
/*         lower_byte_offset */
/*     }; */
/*     struct mem_loader_def test_memory[] = { */
/*         { USER_SPACE_ROOT, code_bytes, 3 } */
/*     }; */
/*     load_memory(test_memory, 1); */
/*     set_reg_value_16(REG_X, 0x1969); */

/*     int cycles = run_cycles(opcode_table[OP_STX_I].cycle_count); */
/*     int post_pc = e_cpu_context.pc; */
/*     assert_int_equal(get_reg_value_16(REG_X), 0x1969); */
/*     assert_int_equal(read_word_from_memory(S_POINTER + lower_byte_offset), */
/*                      0x1969); */
/*     assert_int_equal(cycles, opcode_table[OP_STX_I].cycle_count); */
/*     assert_int_equal(post_pc, pre_pc + 3); */
/* } */

/* void sty_indexed_test(void **state) { */
/*     (void) state; /\* unused *\/ */
/*     int pre_pc = e_cpu_context.pc; */

/*     uint8 lower_byte_offset = 0x40; */
/*     assert_int_equal(read_word_from_memory(S_POINTER + lower_byte_offset), */
/*                      0); */
/*     uint8 code_bytes[] = { */
/*         OP_IXTENDED_X10, */
/*         OP_STY_I, */
/*         S_POINTER >> 8, */
/*         lower_byte_offset */
/*     }; */
/*     struct mem_loader_def test_memory[] = { */
/*         { USER_SPACE_ROOT, code_bytes, 4 } */
/*     }; */
/*     load_memory(test_memory, 1); */
/*     set_reg_value_16(REG_Y, 0x1969); */

/*     int cycles = run_cycles(opcode_ext_x10_table[OP_STY_I].cycle_count); */
/*     int post_pc = e_cpu_context.pc; */
/*     assert_int_equal(get_reg_value_16(REG_Y), 0x1969); */
/*     assert_int_equal(read_word_from_memory(S_POINTER + lower_byte_offset), */
/*                      0x1969); */
/*     assert_int_equal(cycles, opcode_ext_x10_table[OP_STY_I].cycle_count); */
/*     assert_int_equal(post_pc, pre_pc + 4); */
/* } */

/* void suba_indexed_test(void **state) { */
/*     (void) state; /\* unused *\/ */
/*     int pre_pc = e_cpu_context.pc; */

/*     uint8 lower_byte_offset = 0x40; */
/*     uint8 code_bytes[] = { */
/*         OP_SUBA_I, */
/*         S_POINTER >> 8, */
/*         lower_byte_offset */
/*     }; */
/*     uint8 data_bytes[] = { */
/*         0x4 */
/*     }; */
/*     struct mem_loader_def test_memory[] = { */
/*         { USER_SPACE_ROOT, code_bytes, 3 }, */
/*         { S_POINTER + lower_byte_offset, data_bytes, 1 } */
/*     }; */
/*     load_memory(test_memory, 2); */

/*     /\* b shouldn't be messed with *\/ */
/*     set_reg_value_8(REG_B, 0xFF); */
/*     set_reg_value_8(REG_A, 6); */

/*     int cycles = run_cycles(opcode_table[OP_SUBA_I].cycle_count); */
/*     int post_pc = e_cpu_context.pc; */
/*     assert_int_equal(get_reg_value_8(REG_A), 2); */
/*     assert_int_equal(get_reg_value_8(REG_B), 0xFF); */
/*     assert_int_equal(e_cpu_context.cc.n, 0); */
/*     assert_int_equal(e_cpu_context.cc.c, 0); */
/*     assert_int_equal(e_cpu_context.cc.z, 0); */
/*     assert_int_equal(e_cpu_context.cc.v, 0); */
/*     assert_int_equal(cycles, opcode_table[OP_SUBA_I].cycle_count); */
/*     assert_true(post_pc == pre_pc + 3); */
/* } */

/* void subb_indexed_test(void **state) { */
/*     (void) state; /\* unused *\/ */
/*     int pre_pc = e_cpu_context.pc; */

/*     uint8 lower_byte_offset = 0x40; */
/*     uint8 code_bytes[] = { */
/*         OP_SUBB_I, */
/*         S_POINTER >> 8, */
/*         lower_byte_offset */
/*     }; */
/*     uint8 data_bytes[] = { */
/*         0x4 */
/*     }; */
/*     struct mem_loader_def test_memory[] = { */
/*         { USER_SPACE_ROOT, code_bytes, 3 }, */
/*         { S_POINTER + lower_byte_offset, data_bytes, 1 } */
/*     }; */
/*     load_memory(test_memory, 2); */

/*     /\* b shouldn't be messed with *\/ */
/*     set_reg_value_8(REG_A, 0xFF); */
/*     set_reg_value_8(REG_B, 6); */

/*     int cycles = run_cycles(opcode_table[OP_SUBB_I].cycle_count); */
/*     int post_pc = e_cpu_context.pc; */
/*     assert_int_equal(get_reg_value_8(REG_B), 2); */
/*     assert_int_equal(get_reg_value_8(REG_A), 0xFF); */
/*     assert_int_equal(e_cpu_context.cc.n, 0); */
/*     assert_int_equal(e_cpu_context.cc.c, 0); */
/*     assert_int_equal(e_cpu_context.cc.z, 0); */
/*     assert_int_equal(e_cpu_context.cc.v, 0); */
/*     assert_int_equal(cycles, opcode_table[OP_SUBB_I].cycle_count); */
/*     assert_true(post_pc == pre_pc + 3); */
/* } */

/* void subd_indexed_test(void **state) { */
/*      (void) state; /\* unused *\/ */
/*      int pre_pc = e_cpu_context.pc; */

/*      uint8 lower_byte_offset = 0x40; */
/*      uint8 code_bytes[] = { */
/*           OP_SUBD_I, */
/*           S_POINTER >> 8, */
/*           lower_byte_offset */
/*      }; */
/*      uint8 data_bytes[] = { */
/*           0x0, */
/*           0x4 */
/*      }; */
/*      struct mem_loader_def test_memory[] = { */
/*           { USER_SPACE_ROOT, code_bytes, 3 }, */
/*           { S_POINTER + lower_byte_offset, data_bytes, 2 } */
/*      }; */
/*      load_memory(test_memory, 2); */

/*      set_reg_value_16(REG_D, 6); */

/*      int cycles = run_cycles(opcode_table[OP_SUBD_I].cycle_count); */
/*      int post_pc = e_cpu_context.pc; */
/*      assert_int_equal(get_reg_value_16(REG_D), 2); */
/*      assert_int_equal(e_cpu_context.cc.n, 0); */
/*      assert_int_equal(e_cpu_context.cc.c, 0); */
/*      assert_int_equal(e_cpu_context.cc.z, 0); */
/*      assert_int_equal(e_cpu_context.cc.v, 0); */
/*      assert_int_equal(cycles, opcode_table[OP_SUBD_I].cycle_count); */
/*      assert_true(post_pc == pre_pc + 3); */
/* } */

/* void tst_indexed_test(void **state) { */
/*      (void) state; /\* unused *\/ */
/*      int pre_pc = e_cpu_context.pc; */

/*      uint8 lower_byte_offset = 0x40; */
/*      uint8 code_bytes[] = { */
/*           OP_TST_I, */
/*           S_POINTER >> 8, */
/*           lower_byte_offset */
/*      }; */
/*      uint8 data_bytes[] = { */
/*           0x0 */
/*      }; */
/*      struct mem_loader_def test_memory[] = { */
/*           { USER_SPACE_ROOT, code_bytes, 3 }, */
/*           { S_POINTER + lower_byte_offset, data_bytes, 1 } */
/*      }; */
/*      load_memory(test_memory, 2); */

/*      e_cpu_context.cc.v = 1; */
/*      int cycles = run_cycles(opcode_table[OP_TST_I].cycle_count); */
/*      int post_pc = e_cpu_context.pc; */

/*      assert_int_equal(cycles, opcode_table[OP_TST_I].cycle_count); */
/*      assert_int_equal(e_cpu_context.cc.n, 0); */
/*      assert_int_equal(e_cpu_context.cc.z, 1); */
/*      assert_int_equal(e_cpu_context.cc.v, 0); */
/*      assert_int_equal(post_pc, pre_pc + 3); */
/* } */
