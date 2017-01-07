#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>

#include "core.h"

extern struct cpu_state e_cpu_context;
extern struct opcode_def opcode_table[];

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

void core_init_test(void **state) {
    (void) state; /* unused */

    assert_int_equal(e_cpu_context.x, 0);
    assert_int_equal(e_cpu_context.y, 0);
    assert_int_equal(e_cpu_context.u, U_POINTER);
    assert_int_equal(e_cpu_context.s, S_POINTER);
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

    for (int i = 0; i < MEMORY_SIZE; i++) {
        assert_int_equal(e_cpu_context.memory[i], 0);
    }
}

void test_e_flag(void **state) {
    (void) state; /* unused */

    e_cpu_context.cc.e = 1;
    unsigned int raw_cc = (unsigned int) *((unsigned int*) &e_cpu_context.cc);
    assert_int_equal(raw_cc, 0x80);
}

void test_f_flag(void **state) {
    (void) state; /* unused */

    e_cpu_context.cc.f = 1;
    unsigned int raw_cc = (unsigned int) *((unsigned int*) &e_cpu_context.cc);
    assert_int_equal(raw_cc, 0x40);
}

void test_h_flag(void **state) {
    (void) state; /* unused */

    e_cpu_context.cc.h = 1;
    unsigned int raw_cc = (unsigned int) *((unsigned int*) &e_cpu_context.cc);
    assert_int_equal(raw_cc, 0x20);
}

void test_i_flag(void **state) {
    (void) state; /* unused */

    e_cpu_context.cc.i = 1;
    unsigned int raw_cc = (unsigned int) *((unsigned int*) &e_cpu_context.cc);
    assert_int_equal(raw_cc, 0x10);
}

void test_n_flag(void **state) {
    (void) state; /* unused */

    e_cpu_context.cc.n = 1;
    unsigned int raw_cc = (unsigned int) *((unsigned int*) &e_cpu_context.cc);
    assert_int_equal(raw_cc, 0x8);
}

void test_z_flag(void **state) {
    (void) state; /* unused */

    e_cpu_context.cc.z = 1;
    unsigned int raw_cc = (unsigned int) *((unsigned int*) &e_cpu_context.cc);
    assert_int_equal(raw_cc, 0x4);
}

void test_v_flag(void **state) {
    (void) state; /* unused */

    e_cpu_context.cc.v = 1;
    unsigned int raw_cc = (unsigned int) *((unsigned int*) &e_cpu_context.cc);
    assert_int_equal(raw_cc, 0x2);
}

void test_c_flag(void **state) {
    (void) state; /* unused */

    e_cpu_context.cc.c = 1;
    unsigned int raw_cc = (unsigned int) *((unsigned int*) &e_cpu_context.cc);
    assert_int_equal(raw_cc, 0x1);
}

void test_all_flags(void **state) {
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
    assert_int_equal(raw_cc, 0xFF);
}

void test_load_memory(void **state) {
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

void test_load_memory_too_big(void **state) {
    (void) state; /* unused */

    uint8 test_bytes[] = {
        0x12, 0x34
    };

    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT + 0xA000, test_bytes, MEMORY_SIZE / 2 }
    };

    expect_assert_failure(load_memory(test_memory, 1));
}

void test_load_memory_too_far(void **state) {
    (void) state; /* unused */

    uint8 test_bytes[] = {
        0x12, 0x34
    };

    struct mem_loader_def test_memory[] = {
        { MEMORY_SIZE - 1, test_bytes, 2 }
    };

    expect_assert_failure(load_memory(test_memory, 1));
}

void read_byte_from_memory_test(void **state) {
    (void) state; /* unused */

    uint8 test_value = 0x7F;
    uint16 test_address = 0x5678;
    uint8 pre_value = read_byte_from_memory(test_address);
    write_byte_to_memory(test_address, test_value);
    uint8 post_value = read_byte_from_memory(test_address);

    assert_int_equal(pre_value, 0);
    assert_int_equal(post_value, test_value);
}

void read_word_from_memory_test(void **state) {
    (void) state; /* unused */

    uint16 test_value = 0x1234;
    uint16 test_address = 0x5678;
    uint16 pre_value = read_word_from_memory(test_address);
    write_word_to_memory(test_address, test_value);
    uint16 post_value = read_word_from_memory(test_address);

    assert_int_equal(pre_value, 0);
    assert_int_equal(post_value, test_value);
}

void read_byte_handler_immedidate_test(void **state) {
    (void) state; /* unused */

    uint8 test_value = 0x7F;
    /* IMMEDIATE mode reads data right from the pc register location
       and moves the pc forward */
    e_cpu_context.pc = 0x1234;
    uint8 extra_cycles = 1;
    uint8 pre_value = read_byte_handler(IMMEDIATE, &extra_cycles);
    e_cpu_context.pc--;
    write_byte_to_memory(e_cpu_context.pc, test_value);
    uint8 post_value = read_byte_handler(IMMEDIATE, &extra_cycles);

    assert_int_equal(pre_value, 0);
    assert_int_equal(post_value, test_value);
    assert_int_equal(e_cpu_context.pc, 0x1235);
    assert_int_equal(extra_cycles, 0);
}

void read_byte_handler_direct_test(void **state) {
    (void) state; /* unused */

    e_cpu_context.dp = S_POINTER >> 8;
    uint8 lower_byte_address = 0x40;

    uint8 test_value = 0x7F;
    uint8 code_bytes[] = {
        lower_byte_address
    };
    uint8 data_bytes[] = {
        test_value
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1 },
        { S_POINTER + lower_byte_address, data_bytes, 1 }
    };
    load_memory(test_memory, 2);

    /* DIRECT mode reads data right from the location pointed to
       by dp << 8 | IMMEDIATE value */
    e_cpu_context.pc = USER_SPACE_ROOT;
    uint8 extra_cycles = 1;
    uint8 read_value = read_byte_handler(DIRECT, &extra_cycles);

    assert_int_equal(read_value, test_value);
    assert_int_equal(e_cpu_context.pc, USER_SPACE_ROOT + 1);
    assert_int_equal(extra_cycles, 0);
}

void read_byte_handler_extended_test(void **state) {
    (void) state; /* unused */

    uint8 lower_byte_address = 0x40;
    uint8 test_value = 0x7F;
    uint8 code_bytes[] = {
        S_POINTER >> 8,
        lower_byte_address
    };
    uint8 data_bytes[] = {
        test_value
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
        { S_POINTER + lower_byte_address, data_bytes, 1 }
    };
    load_memory(test_memory, 2);

    /* EXTENDED mode reads data right from the location pointed to
       by the word at the PC */
    e_cpu_context.pc = USER_SPACE_ROOT;
    uint8 extra_cycles = 1;
    uint8 read_value = read_byte_handler(EXTENDED, &extra_cycles);

    assert_int_equal(read_value, test_value);
    assert_int_equal(e_cpu_context.pc, USER_SPACE_ROOT + 2);
    assert_int_equal(extra_cycles, 0);
}

void read_word_handler_direct_test(void **state) {
    (void) state; /* unused */

    e_cpu_context.dp = S_POINTER >> 8;
    uint8 lower_byte_address = 0x40;

    uint8 test_value = 0x7F;
    uint8 test_value_lower = 0x12;
    uint8 code_bytes[] = {
        lower_byte_address
    };
    uint8 data_bytes[] = {
        test_value,
        test_value_lower
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1 },
        { S_POINTER + lower_byte_address, data_bytes, 2 }
    };
    load_memory(test_memory, 2);

    /* DIRECT mode reads data right from the location pointed to
       by dp << 8 | IMMEDIATE value */
    e_cpu_context.pc = USER_SPACE_ROOT;
    uint8 extra_cycles = 1;
    uint16 read_value = read_word_handler(DIRECT, &extra_cycles);

    assert_int_equal(read_value, test_value << 8 | test_value_lower);
    assert_int_equal(e_cpu_context.pc, USER_SPACE_ROOT + 1);
    assert_int_equal(extra_cycles, 0);
}

void read_word_handler_immedidate_test(void **state) {
    (void) state; /* unused */

    uint8 extra_cycles = 1;
    uint16 test_value = 0x7FFF;
    /* IMMEDIATE mode reads data right from the pc register location
       and moves the pc forward */
    e_cpu_context.pc = 0x1234;
    uint16 pre_value = read_word_handler(IMMEDIATE, &extra_cycles);
    e_cpu_context.pc -= 2;
    write_word_to_memory(e_cpu_context.pc, test_value);
    uint16 post_value = read_word_handler(IMMEDIATE, &extra_cycles);

    assert_int_equal(pre_value, 0);
    assert_int_equal(post_value, test_value);
    assert_int_equal(e_cpu_context.pc, 0x1236);
    assert_int_equal(extra_cycles, 0);
}

void read_word_handler_extended_test(void **state) {
    (void) state; /* unused */

    uint8 extra_cycles = 1;
    uint8 lower_byte_address = 0x40;
    uint8 test_value = 0x7F;
    uint8 test_value_lower = 0x12;
    uint8 code_bytes[] = {
        S_POINTER >> 8,
        lower_byte_address
    };
    uint8 data_bytes[] = {
        test_value,
        test_value_lower
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
        { S_POINTER + lower_byte_address, data_bytes, 2 }
    };
    load_memory(test_memory, 2);

    /* EXTENDED mode reads data right from the location pointed to
       by the word at the PC */
    e_cpu_context.pc = USER_SPACE_ROOT;
    uint16 read_value = read_word_handler(EXTENDED, &extra_cycles);

    assert_int_equal(read_value, test_value << 8 | test_value_lower);
    assert_int_equal(e_cpu_context.pc, USER_SPACE_ROOT + 2);
    assert_int_equal(extra_cycles, 0);
}

void memory_clear_test(void **state) {
    (void) state; /* unused */

    uint8 test_value = 0x7F;
    e_cpu_context.pc = 0x1234;
    write_byte_to_memory(e_cpu_context.pc, test_value);
    uint8 extra_cycles = 1;
    uint8 pre_value = read_byte_handler(IMMEDIATE, &extra_cycles);

    /* this should clear out my memory */
    core_destroy();
    core_init();

    uint8 post_value = read_byte_handler(IMMEDIATE, &extra_cycles);

    assert_int_equal(pre_value, test_value);
    assert_int_equal(post_value, 0);
    assert_int_equal(extra_cycles, 0);
}

void get_memory_address_from_postbyte_immediate_test(void **state) {
    (void) state; /* unused */

    e_cpu_context.pc = 0x1234;
    uint8 extra_cycles = 1;
    uint16 read_value = get_memory_address_from_postbyte(IMMEDIATE, &extra_cycles);

    assert_int_equal(read_value, e_cpu_context.pc - 1);
    assert_int_equal(e_cpu_context.pc, 0x1235);
    assert_int_equal(extra_cycles, 0);
}

void get_memory_address_from_postbyte_direct_test(void **state) {
    (void) state; /* unused */

    uint8 extra_cycles = 1;
    e_cpu_context.dp = S_POINTER >> 8;
    uint8 lower_byte_address = 0x40;
    uint8 code_bytes[] = {
        lower_byte_address
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1 }
    };
    load_memory(test_memory, 1);

    /* DIRECT mode reads data right from the location pointed to
       by dp << 8 | IMMEDIATE value */
    uint16 read_value = get_memory_address_from_postbyte(DIRECT, &extra_cycles);

    assert_int_equal(read_value, S_POINTER | lower_byte_address);
    assert_int_equal(extra_cycles, 0);
}

void get_memory_address_from_postbyte_extended_test(void **state) {
    (void) state; /* unused */

    uint8 extra_cycles = 1;
    uint8 lower_byte_address = 0x40;
    uint8 code_bytes[] = {
        S_POINTER >> 8,
        lower_byte_address
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);

    /* word following instruction is full 16 bit address of
       operand */
    uint16 read_value = get_memory_address_from_postbyte(EXTENDED, &extra_cycles);

    assert_int_equal(read_value, S_POINTER | lower_byte_address);
    assert_int_equal(extra_cycles, 0);
}

void get_memory_address_from_postbyte_indexed_constant_basic_x_test(void **state) {
    (void) state; /* unused */

    uint8 extra_cycles = 1;
    uint8 code_bytes[] = {
        0x84 /* Constant offset of zero from register X */
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1 }
    };
    load_memory(test_memory, 1);
    set_reg_value_16(REG_X, 0xCAFE);

    /* word following instruction is full 16 bit address of
       operand */
    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xCAFE);
    assert_int_equal(extra_cycles, 0);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 1);
}

void get_memory_address_from_postbyte_indexed_constant_basic_y_test(void **state) {
    (void) state; /* unused */

    uint8 extra_cycles = 1;
    uint8 code_bytes[] = {
        0xA4 /* Constant offset of zero from register Y */
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1 }
    };
    load_memory(test_memory, 1);
    set_reg_value_16(REG_Y, 0xCAFE);

    /* word following instruction is full 16 bit address of
       operand */
    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xCAFE);
    assert_int_equal(extra_cycles, 0);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 1);
}

void get_memory_address_from_postbyte_indexed_constant_basic_u_test(void **state) {
    (void) state; /* unused */

    uint8 extra_cycles = 1;
    uint8 code_bytes[] = {
        0xC4 /* Constant offset of zero from register U */
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1 }
    };
    load_memory(test_memory, 1);
    set_reg_value_16(REG_U, 0xCAFE);

    /* word following instruction is full 16 bit address of
       operand */
    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xCAFE);
    assert_int_equal(extra_cycles, 0);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 1);
}

void get_memory_address_from_postbyte_indexed_constant_basic_s_test(void **state) {
    (void) state; /* unused */

    uint8 extra_cycles = 1;
    uint8 code_bytes[] = {
        0xE4 /* Constant offset of zero from register S */
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1 }
    };
    load_memory(test_memory, 1);
    set_reg_value_16(REG_S, 0xCAFE);

    /* word following instruction is full 16 bit address of
       operand */
    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xCAFE);
    assert_int_equal(extra_cycles, 0);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 1);
}

void get_memory_address_from_postbyte_indexed_constant_5bit_x_test(void **state) {
    (void) state; /* unused */

    uint8 extra_cycles = 1;
    uint8 code_bytes[] = {
        0x1F /* Constant 5-bit offset of -1 from register X */
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1 }
    };
    load_memory(test_memory, 1);
    set_reg_value_16(REG_X, 0xCAFF);

    /* word following instruction is full 16 bit address of
       operand */
    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xCAFE);
    assert_int_equal(extra_cycles, 1);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 1);
}

void get_memory_address_from_postbyte_indexed_constant_8bit_y_test(void **state) {
    (void) state; /* unused */

    uint8 extra_cycles = 1;
    uint8 code_bytes[] = {
        0xA8, /* Constant 8-bit offset of -1 from register Y */
        0xFF
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);
    set_reg_value_16(REG_Y, 0xCAFF);

    /* word following instruction is full 16 bit address of
       operand */
    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xCAFE);
    assert_int_equal(extra_cycles, 1);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 2);
}

/* Run a single NOP instruction which should yield 2 cycles */
void run_cycles_test(void **state) {
    (void) state; /* unused */

    uint8 code_bytes[] = {
        OP_NOP
    };

    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1}
    };

    load_memory(test_memory, 1);

    uint32 completed_cycles = run_cycles(opcode_table[OP_NOP].cycle_count);

    assert_int_equal(completed_cycles, opcode_table[OP_NOP].cycle_count);
    assert_int_equal(e_cpu_context.cycle_count, opcode_table[OP_NOP].cycle_count);
    assert_int_equal(e_cpu_context.pc, USER_SPACE_ROOT + 1);
}

/* Run two NOP instructions which should yield 4 cycles */
void run_cycles_multiple_test(void **state) {
    (void) state; /* unused */

    uint8 code_bytes[] = {
        OP_NOP, OP_NOP
    };

    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2}
    };

    load_memory(test_memory, 1);

    uint32 completed_cycles = run_cycles(opcode_table[OP_NOP].cycle_count * 2);

    assert_int_equal(completed_cycles, opcode_table[OP_NOP].cycle_count * 2);
    assert_int_equal(e_cpu_context.cycle_count,
                     opcode_table[OP_NOP].cycle_count * 2);
    assert_int_equal(e_cpu_context.pc, USER_SPACE_ROOT + 2);
}

/* 0x1 on the 6809 is an invalid exception and we'll use it for this NOTIMPL
   assert test */
void run_cycles_notimpl_test(void **state) {
    (void) state; /* unused */

    uint8 code_bytes[] = {
        0x1
    };

    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1}
    };

    load_memory(test_memory, 1);

    expect_assert_failure(run_cycles(1));
}

void decode_source_target_postbyte_test(void **state) {
    (void) state; /* unused */

    /* from X to B */
    uint8 postbyte = 0x19;
    enum target_register src, trg;
    decode_source_target_postbyte(postbyte, &src, &trg);

    assert_int_equal(src, REG_X);
    assert_int_equal(trg, REG_B);
}

void decode_source_target_postbyte_2_test(void **state) {
    (void) state; /* unused */

    /* from PC to DP */
    uint8 postbyte = 0x5B;
    enum target_register src, trg;
    decode_source_target_postbyte(postbyte, &src, &trg);

    assert_int_equal(src, REG_PC);
    assert_int_equal(trg, REG_DP);
}

void decode_source_target_postbyte_invalid_test(void **state) {
    (void) state; /* unused */

    /* from PC to invalid */
    uint8 postbyte = 0x5D;
    enum target_register src, trg;
    decode_source_target_postbyte(postbyte, &src, &trg);

    assert_int_equal(src, REG_PC);
    assert_int_equal(trg, REG_NONE);
}

void get_reg_value_8_test(void **state) {
    (void) state; /* unused */

    e_cpu_context.d.byte_acc.a = 0x78;
    uint8 val = get_reg_value_8(REG_A);

    assert_int_equal(val, 0x78);
}

void get_reg_value_8_invalid_test(void **state) {
    (void) state; /* unused */

    expect_assert_failure(get_reg_value_8(REG_U));
}

void set_reg_value_8_test(void **state) {
    (void) state; /* unused */

    set_reg_value_8(REG_CC, 0x78);
    uint8 val = get_reg_value_8(REG_CC);

    assert_int_equal(val, 0x78);
    assert_int_equal(e_cpu_context.cc.e, 0);
    assert_int_equal(e_cpu_context.cc.c, 0);
    assert_int_equal(e_cpu_context.cc.n, 1);
}

void get_reg_value_16_test(void **state) {
    (void) state; /* unused */

    e_cpu_context.d.d = 0x7812;
    uint16 val = get_reg_value_16(REG_D);

    assert_int_equal(val, 0x7812);
}

void get_reg_value_16_invalid_test(void **state) {
    (void) state; /* unused */

    expect_assert_failure(get_reg_value_16(REG_A));
}

void set_reg_value_16_test(void **state) {
    (void) state; /* unused */

    set_reg_value_16(REG_U, 0x7812);
    uint16 val = get_reg_value_16(REG_U);

    assert_int_equal(val, 0x7812);
    assert_int_equal(get_reg_value_16(REG_S), S_POINTER);
}
