#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"

extern struct cpu_state e_cpu_context;
extern struct opcode_def opcode_table[];
const char* root_test_path = "/build/tests/program/";

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

    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xCAFE);
    assert_int_equal(extra_cycles, 1);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 1);
}

void get_memory_address_from_postbyte_indexed_constant_5bit_positive_x_test(void **state) {
    (void) state; /* unused */

    uint8 extra_cycles = 1;
    uint8 code_bytes[] = {
        0x41 /* Constant 5-bit offset of 1 from register U */
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1 }
    };
    load_memory(test_memory, 1);
    set_reg_value_16(REG_U, 0xCAFD);

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

    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xCAFE);
    assert_int_equal(extra_cycles, 1);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 2);
}

void get_memory_address_from_postbyte_indexed_constant_16bit_s_test(void **state) {
    (void) state; /* unused */

    uint8 extra_cycles = 1;
    uint8 code_bytes[] = {
        0xE9, /* Constant 16-bit offset of -1 from register S */
        0xFF,
        0xFF
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 }
    };
    load_memory(test_memory, 1);
    set_reg_value_16(REG_S, 0xCAFF);

    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xCAFE);
    assert_int_equal(extra_cycles, 4);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 3);
}

void get_memory_address_from_postbyte_indexed_constant_16bit_s_indirect_test(void **state) {
    (void) state; /* unused */

    uint8 extra_cycles = 1;
    uint8 code_bytes[] = {
        0xF9, /* Constant 16-bit offset of -1 from register S, indirect mode */
        0xFF,
        0xFF
    };
    uint8 indirect_bytes[] = {
        0xF0,
        0x0D
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { 0xCAFE, indirect_bytes, 2 }
    };
    load_memory(test_memory, 2);
    set_reg_value_16(REG_S, 0xCAFF);

    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xF00D);
    assert_int_equal(extra_cycles, 7);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 3);
}

void get_memory_address_from_postbyte_indexed_constant_8bit_s_indirect_test(void **state) {
    (void) state; /* unused */

    uint8 extra_cycles = 1;
    uint8 code_bytes[] = {
        0xF8, /* Constant 8-bit offset of -1 from register S, indirect mode */
        0xFF
    };
    uint8 indirect_bytes[] = {
        0xF0,
        0x0D
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
        { 0xCAFE, indirect_bytes, 2 }
    };
    load_memory(test_memory, 2);
    set_reg_value_16(REG_S, 0xCAFF);

    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xF00D);
    assert_int_equal(extra_cycles, 4);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 2);
}

void get_memory_address_from_postbyte_indexed_constant_indirect_u_test(void **state) {
    (void) state; /* unused */

    uint8 extra_cycles = 1;
    uint8 code_bytes[] = {
        0xD4 /* Constant offset of zero from register U, indirect mode */
    };
    uint8 indirect_bytes[] = {
        0xF0,
        0x0D
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1 },
        { 0xCAFE, indirect_bytes, 2 }
    };
    load_memory(test_memory, 2);
    set_reg_value_16(REG_U, 0xCAFE);

    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xF00D);
    assert_int_equal(extra_cycles, 3);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 1);
}

void get_memory_address_from_postbyte_indexed_accumulator_a_test(void **state) {
    (void) state; /* unused */

    uint8 extra_cycles = 0;
    uint8 code_bytes[] = {
        0x86 /* A accumulator offset from register X */
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1 }
    };
    load_memory(test_memory, 1);
    set_reg_value_16(REG_X, 0xCAFF);
    set_reg_value_8(REG_A, 0xFF);


    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xCAFE);
    assert_int_equal(extra_cycles, 1);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 1);
}

void get_memory_address_from_postbyte_indexed_accumulator_b_test(void **state) {
    uint8 extra_cycles = 0;
    uint8 code_bytes[] = {
        0xA5 /* B accumulator offset from register Y */
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1 }
    };
    load_memory(test_memory, 1);
    set_reg_value_16(REG_Y, 0xCAFF);
    set_reg_value_8(REG_B, 0xFF);

    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xCAFE);
    assert_int_equal(extra_cycles, 1);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 1);
}

void get_memory_address_from_postbyte_indexed_accumulator_d_test(void **state) {
    uint8 extra_cycles = 1;
    uint8 code_bytes[] = {
        0xCB /* D accumulator offset from register U */
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1 }
    };
    load_memory(test_memory, 1);
    set_reg_value_16(REG_U, 0xCAFF);
    set_reg_value_16(REG_D, 0xFFFF);

    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xCAFE);
    assert_int_equal(extra_cycles, 4);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 1);
}

void get_memory_address_from_postbyte_indexed_accumulator_a_indirect_test(void **state) {
    (void) state; /* unused */

    uint8 extra_cycles = 1;
    uint8 code_bytes[] = {
        0x96 /* A accumulator offset from register X, indirect */
    };
    uint8 indirect_bytes[] = {
        0xF0,
        0x0D
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1 },
        { 0xCAFE, indirect_bytes, 2 },
    };
    load_memory(test_memory, 2);
    set_reg_value_16(REG_X, 0xCAFF);
    set_reg_value_8(REG_A, 0xFF);

    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xF00D);
    assert_int_equal(extra_cycles, 4);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 1);
}

void get_memory_address_from_postbyte_indexed_accumulator_b_indirect_test(void **state) {
    (void) state; /* unused */

    uint8 extra_cycles = 1;
    uint8 code_bytes[] = {
        0xB5 /* B accumulator offset from register Y, indirect */
    };
    uint8 indirect_bytes[] = {
        0xF0,
        0x0D
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1 },
        { 0xCAFE, indirect_bytes, 2 },
    };
    load_memory(test_memory, 2);
    set_reg_value_16(REG_Y, 0xCAFF);
    set_reg_value_8(REG_B, 0xFF);

    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xF00D);
    assert_int_equal(extra_cycles, 4);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 1);
}

void get_memory_address_from_postbyte_indexed_accumulator_d_indirect_test(void **state) {
    (void) state; /* unused */

    uint8 extra_cycles = 1;
    uint8 code_bytes[] = {
        0xDB /* D accumulator offset from register U, indirect */
    };
    uint8 indirect_bytes[] = {
        0xF0,
        0x0D
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1 },
        { 0xCAFE, indirect_bytes, 2 },
    };
    load_memory(test_memory, 2);
    set_reg_value_16(REG_U, 0xCAFF);
    set_reg_value_16(REG_D, 0xFFFF);

    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xF00D);
    assert_int_equal(extra_cycles, 7);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 1);
}

void get_memory_address_from_postbyte_indexed_inc_1_test(void **state) {
    uint8 extra_cycles = 0;
    uint8 code_bytes[] = {
        0x80 /* Post-Increment by 1 based on X */
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1 }
    };
    load_memory(test_memory, 1);
    set_reg_value_16(REG_X, 0xCAFE);

    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xCAFE);
    assert_int_equal(get_reg_value_16(REG_X), 0xCAFF);
    assert_int_equal(extra_cycles, 2);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 1);
}

void get_memory_address_from_postbyte_indexed_inc_2_test(void **state) {
    uint8 extra_cycles = 0;
    uint8 code_bytes[] = {
        0xA1 /* Post-Increment by 2 based on Y */
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1 }
    };
    load_memory(test_memory, 1);
    set_reg_value_16(REG_Y, 0xCAFE);

    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xCAFE);
    assert_int_equal(get_reg_value_16(REG_Y), 0xCB00);
    assert_int_equal(extra_cycles, 3);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 1);
}

void get_memory_address_from_postbyte_indexed_dec_1_test(void **state) {
    uint8 extra_cycles = 0;
    uint8 code_bytes[] = {
        0xC2 /* Pre-decrement by 1 based on U */
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1 }
    };
    load_memory(test_memory, 1);
    set_reg_value_16(REG_U, 0xCAFE);

    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xCAFD);
    assert_int_equal(get_reg_value_16(REG_U), 0xCAFD);
    assert_int_equal(extra_cycles, 2);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 1);
}

void get_memory_address_from_postbyte_indexed_dec_2_test(void **state) {
    uint8 extra_cycles = 0;
    uint8 code_bytes[] = {
        0xE3 /* Pre-decrement by 2 based on S */
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1 }
    };
    load_memory(test_memory, 1);
    set_reg_value_16(REG_S, 0xCAFE);

    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xCAFC);
    assert_int_equal(get_reg_value_16(REG_S), 0xCAFC);
    assert_int_equal(extra_cycles, 3);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 1);
}

void get_memory_address_from_postbyte_indexed_inc_2_indirect_test(void **state) {
    uint8 extra_cycles = 0;
    uint8 code_bytes[] = {
        0x91 /* Post-increment by 2 based on X */
    };
    uint8 indirect_bytes[] = {
        0xF0,
        0x0D
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1 },
        { 0xCAFE, indirect_bytes, 2 },
    };
    load_memory(test_memory, 2);
    set_reg_value_16(REG_X, 0xCAFE);

    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xF00D);
    assert_int_equal(get_reg_value_16(REG_X), 0xCB00);
    assert_int_equal(extra_cycles, 6);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 1);
}

void get_memory_address_from_postbyte_indexed_dec_2_indirect_test(void **state) {
    uint8 extra_cycles = 0;
    uint8 code_bytes[] = {
        0xF3 /* Pre-decrement by 2 based on S */
    };
    uint8 indirect_bytes[] = {
        0xF0,
        0x0D
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1 },
        { 0xCAFE, indirect_bytes, 2 },
    };
    load_memory(test_memory, 2);
    set_reg_value_16(REG_S, 0xCB00);

    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xF00D);
    assert_int_equal(get_reg_value_16(REG_S), 0xCAFE);
    assert_int_equal(extra_cycles, 6);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 1);
}

void get_memory_address_from_postbyte_indexed_pc_offset_8_test(void **state) {
    uint8 extra_cycles = 0;
    uint8 code_bytes[] = {
        0x8C, /* Constant offset from PC, 8 bit */
        0x10
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 }
    };
    load_memory(test_memory, 1);

    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, USER_SPACE_ROOT + 2 + 0x10);
    assert_int_equal(extra_cycles, 1);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 2);
}

void get_memory_address_from_postbyte_indexed_pc_offset_16_test(void **state) {
    uint8 extra_cycles = 0;
    uint8 code_bytes[] = {
        0x8D, /* Constant offset from PC, 16 bit */
        0xF0,
        0x00
    };

    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 }
    };
    load_memory(test_memory, 1);

    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, USER_SPACE_ROOT + 3 - 0x1000);
    assert_int_equal(extra_cycles, 5);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 3);
}

void get_memory_address_from_postbyte_indexed_pc_offset_8_indirect_test(void **state) {
    uint8 extra_cycles = 0;
    uint8 code_bytes[] = {
        0x9C, /* Constant offset from PC, 8 bit */
        0x10
    };
    uint8 indirect_bytes[] = {
        0xF0,
        0x0D
    };

    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
        { USER_SPACE_ROOT + 2 + 0x10, indirect_bytes, 2 }
    };
    load_memory(test_memory, 2);

    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xF00D);
    assert_int_equal(extra_cycles, 4);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 2);
}

void get_memory_address_from_postbyte_indexed_pc_offset_16_indirect_test(void **state) {
    uint8 extra_cycles = 0;
    uint8 code_bytes[] = {
        0x9D, /* Constant offset from PC, 16 bit */
        0xF0,
        0x00
    };
    uint8 indirect_bytes[] = {
        0xF0,
        0x0D
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        { USER_SPACE_ROOT + 3 - 0x1000, indirect_bytes, 2 }
    };
    load_memory(test_memory, 2);

    uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

    assert_int_equal(read_value, 0xF00D);
    assert_int_equal(extra_cycles, 8);
    assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 3);
}

void get_memory_address_from_postbyte_indexed_extended_indirect_test(void **state) {
  uint8 extra_cycles = 0;
  uint8 code_bytes[] = {
    0x9F, /* Constant offset from PC, 16 bit */
    0xF0,
    0x0D
  };
  uint8 indirect_bytes[] = {
    0xCA,
    0xFE
  };
  struct mem_loader_def test_memory[] = {
    { USER_SPACE_ROOT, code_bytes, 3 },
    { 0xF00D, indirect_bytes, 2 }
  };
  load_memory(test_memory, 2);

  uint16 read_value = get_memory_address_from_postbyte(INDEXED, &extra_cycles);

  assert_int_equal(read_value, 0xCAFE);
  assert_int_equal(extra_cycles, 5);
  assert_int_equal(get_reg_value_16(REG_PC), USER_SPACE_ROOT + 3);
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

char* get_test_program_path(const char* prog_name) {
    if (!prog_name) {
        return NULL;
    }

    const char* cur_path = getenv("PWD");
    int path_length = strlen(cur_path) +
        strlen(root_test_path) +
        strlen(prog_name);
    char* final_test_path = malloc(path_length);
    memset(final_test_path, '\0', path_length);

    strncat(final_test_path, cur_path, strlen(cur_path));
    strncat(final_test_path, root_test_path, strlen(root_test_path));
    strncat(final_test_path, prog_name, strlen(prog_name));

    return final_test_path;
}

void perform_memory_checks(struct test_check* checks, size_t len) {
    if (!checks) {
        return;
    }

    for (int i=0; i<len; i++) {
        assert_int_equal(checks[i].value,
                         read_byte_from_memory(checks[i].address));
    }
}

void init_from_decb_memory_test(void **state) {
    (void) state; /* unused */

    /* Trivial program that copies one byte of memory from one place
       to the next in DECB binary format:

                            (8-bit_data_transf):00001         ;; 8-BIT DATA TRANSFER
                            (8-bit_data_transf):00002         ;; Purpose: Move the contents of memory location $5000 to memory location $5001.
                            (8-bit_data_transf):00003
                            (8-bit_data_transf):00004                 org     $5000
      5000 69               (8-bit_data_transf):00005                 fcb     $69
                            (8-bit_data_transf):00006
                            (8-bit_data_transf):00007                 org     $2000
      2000 B65000           (8-bit_data_transf):00008                 lda     $5000
      2003 B75001           (8-bit_data_transf):00009                 sta     $5001
                            (8-bit_data_transf):00010                 end     $2000
    */

    /* first preamble and payload, one byte (0x69) loaded at 0x5000 */
    uint8 payload[] = { 0x00, 0x00, 0x01, 0x50, 0x00, \
                        0x69, \
                        /* second preamble and payload, with the two instruction program */
                        0x00, 0x00, 0x06, 0x20, 0x00, \
                        0xB6, 0x50, 0x00, 0xB7, 0x50, 0x01, \
                        /* Postamble with 0x2000 initial PC to be set */
                        0xFF, 0x00, 0x00, 0x20, 0x00 };
    uint16 payload_size = sizeof(payload);

    uint16 num_preambles = init_from_decb_memory(payload, payload_size);

    assert_int_equal(num_preambles, 2);
    assert_int_equal(read_byte_from_memory(0x5000), 0x69);
    assert_int_equal(e_cpu_context.pc, 0x2000);
    assert_int_equal((uint16) read_byte_from_memory(e_cpu_context.pc),
                     (uint16) OP_LDA_E);
    assert_int_equal((uint16) read_byte_from_memory(e_cpu_context.pc + 3),
                     (uint16) OP_STA_E);
}

void init_from_decb_memory_run_cycles_test(void **state) {
    (void) state; /* unused */

    /* This is the first mini emulation end-to-end test use case!

       Trivial program that copies one byte of memory from one place
       to the next in DECB binary format, see init_from_decb_memory_test() */
    uint8 payload[] = { 0x00, 0x00, 0x01, 0x50, 0x00, \
                        0x69, \
                        0x00, 0x00, 0x06, 0x20, 0x00, \
                        0xB6, 0x50, 0x00, 0xB7, 0x50, 0x01, \
                        0xFF, 0x00, 0x00, 0x20, 0x00 };
    uint16 payload_size = sizeof(payload);

    init_from_decb_memory(payload, payload_size);
    uint16 num_cycles = run_cycles(opcode_table[OP_LDA_E].cycle_count +
                                   opcode_table[OP_STA_E].cycle_count);

    assert_int_equal(read_byte_from_memory(0x5001), 0x69);
    assert_int_equal(e_cpu_context.pc, 0x2006);
    assert_int_equal(num_cycles, opcode_table[OP_LDA_E].cycle_count +
                     opcode_table[OP_STA_E].cycle_count);
}

void init_from_decb_memory_invalid_preamble_test(void **state) {
    (void) state; /* unused */

    /* 0x0 is expected at the start of each preamble  */
    uint8 payload[] = { 0x01, 0x00, 0x01, 0x50, 0x00, \
                        0x69, \
                        0x00, 0x00, 0x06, 0x20, 0x00, \
                        0xB6, 0x50, 0x00, 0xB7, 0x50, 0x01, \
                        0xFF, 0x00, 0x00, 0x20, 0x00 };
    uint16 payload_size = sizeof(payload);

    expect_assert_failure(init_from_decb_memory(payload, payload_size));
}

void init_from_decb_memory_invalid_preamble_2_test(void **state) {
    (void) state; /* unused */

    /* 0x0 is expected at the start of each preamble  */
    uint8 payload[] = { 0x00, 0x00, 0x01, 0x50, 0x00, \
                        0x69, \
                        0x01, 0x00, 0x06, 0x20, 0x00, \
                        0xB6, 0x50, 0x00, 0xB7, 0x50, 0x01, \
                        0xFF, 0x00, 0x00, 0x20, 0x00 };
    uint16 payload_size = sizeof(payload);

    expect_assert_failure(init_from_decb_memory(payload, payload_size));
}

void init_from_decb_memory_invalid_postamble_test(void **state) {
    (void) state; /* unused */

    /* 0xFE is expected at the start of each postamble  */
    uint8 payload[] = { 0x00, 0x00, 0x01, 0x50, 0x00, \
                        0x69, \
                        0x00, 0x00, 0x06, 0x20, 0x00, \
                        0xB6, 0x50, 0x00, 0xB7, 0x50, 0x01, \
                        0xFE, 0x00, 0x00, 0x20, 0x00 };
    uint16 payload_size = sizeof(payload);

    expect_assert_failure(init_from_decb_memory(payload, payload_size));
}

void init_from_decb_memory_invalid_preamble_length_test(void **state) {
    (void) state; /* unused */

    /* Preambles are 5 bytes long followed by the payload and the postamble and
       stuff */
    uint8 payload[] = { 0x00, 0x00, 0x01, 0x50 };
    uint16 payload_size = sizeof(payload);

    expect_assert_failure(init_from_decb_memory(payload, payload_size));
}

void init_from_decb_memory_invalid_payload_size_test(void **state) {
    (void) state; /* unused */

    /* First preamble payload size is erroneous  */
    uint8 payload[] = { 0x00, 0x81, 0x69, 0x50, 0x00, \
                        0x69, \
                        0x00, 0x00, 0x06, 0x20, 0x00, \
                        0xB6, 0x50, 0x00, 0xB7, 0x50, 0x01, \
                        0xFE, 0x00, 0x00, 0x20, 0x00 };
    uint16 payload_size = sizeof(payload);

    expect_assert_failure(init_from_decb_memory(payload, payload_size));
}

void init_from_decb_memory_invalid_postamble_too_short_test(void **state) {
    (void) state; /* unused */

    /* Postamble is too short  */
    uint8 payload[] = { 0x00, 0x00, 0x01, 0x50, 0x00, \
                        0x69, \
                        0x00, 0x00, 0x06, 0x20, 0x00, \
                        0xB6, 0x50, 0x00, 0xB7, 0x50, 0x01, \
                        0xFF, 0x00, 0x00, 0x20 };
    uint16 payload_size = sizeof(payload);

    expect_assert_failure(init_from_decb_memory(payload, payload_size));
}

void init_from_decb_memory_invalid_postamble_too_long_test(void **state) {
    (void) state; /* unused */

    /* Postamble is too short  */
    uint8 payload[] = { 0x00, 0x00, 0x01, 0x50, 0x00, \
                        0x69, \
                        0x00, 0x00, 0x06, 0x20, 0x00, \
                        0xB6, 0x50, 0x00, 0xB7, 0x50, 0x01, \
                        0xFF, 0x00, 0x00, 0x20, 0x00, 0x00 };
    uint16 payload_size = sizeof(payload);

    expect_assert_failure(init_from_decb_memory(payload, payload_size));
}

void init_from_decb_memory_invalid_postamble_byte_wrong_test(void **state) {
    (void) state; /* unused */

    /* Postamble is too short  */
    uint8 payload[] = { 0x00, 0x00, 0x01, 0x50, 0x00, \
                        0x69, \
                        0x00, 0x00, 0x06, 0x20, 0x00, \
                        0xB6, 0x50, 0x00, 0xB7, 0x50, 0x01, \
                        0xFF, 0x00, 0x01, 0x20, 0x00 };
    uint16 payload_size = sizeof(payload);

    expect_assert_failure(init_from_decb_memory(payload, payload_size));
}

void init_from_decb_file_basic_test(void **state) {
    (void) state; /* unused */

    struct test_check checks[] = {
        { 0x5000, 0x69 }
    };

    char* program_path = get_test_program_path("8-bit_data_transfer.bin");
    int num_preambles = init_from_decb_file(program_path);
    free(program_path);

    assert_int_equal(num_preambles, 2);
    perform_memory_checks(checks, sizeof(checks) / sizeof(checks[0]));
}

void init_from_decb_file_basic_run_cycles_test(void **state) {
    (void) state; /* unused */

    struct test_check checks[] = {
        { 0x5001, 0x69 }
    };

    char* program_path = get_test_program_path("8-bit_data_transfer.bin");
    init_from_decb_file(program_path);
    free(program_path);

    uint16 num_cycles = run_cycles(opcode_table[OP_LDA_E].cycle_count +
                                   opcode_table[OP_STA_E].cycle_count);
    perform_memory_checks(checks, sizeof(checks) / sizeof(checks[0]));
    assert_int_equal(e_cpu_context.pc, 0x2006);
    assert_int_equal(num_cycles, opcode_table[OP_LDA_E].cycle_count +
                     opcode_table[OP_STA_E].cycle_count);
}

void init_from_decb_file_error_test(void **state) {
    (void) state; /* unused */

    int num_preambles = 0;
    expect_assert_failure(num_preambles = init_from_decb_file(
                              "/this/file/doesnt/exist"));

    assert_int_equal(num_preambles, 0);
}

void program_8_bit_addition_program_test(void **state) {
    (void) state; /* unused */

    struct test_check checks[] = {
        { 0x5000, 0x38 },
        { 0x5001, 0x2B },
        { 0x5002, 0x63 }
    };

    char* program_path = get_test_program_path("8-bit_addition.bin");
    init_from_decb_file(program_path);
    free(program_path);

    uint16 num_cycles = run_cycles(opcode_table[OP_LDA_E].cycle_count +
                                   opcode_table[OP_ADDA_E].cycle_count +
                                   opcode_table[OP_STA_E].cycle_count);
    perform_memory_checks(checks, sizeof(checks) / sizeof(checks[0]));
    assert_int_equal(e_cpu_context.pc, 0x2009);
    assert_int_equal(num_cycles, opcode_table[OP_LDA_E].cycle_count +
                     opcode_table[OP_ADDA_E].cycle_count +
                     opcode_table[OP_STA_E].cycle_count);

}
