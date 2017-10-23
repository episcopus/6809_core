#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>

#include "headers.h"
#include "tests/test.h"

extern struct cpu_state e_cpu_context;
extern struct opcode_def opcode_table[];
extern const char* root_test_path;

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
    assert_int_equal(e_cpu_context.hsync_cycles, HSYNC_CYCLES_TOTAL);
    assert_int_equal(e_cpu_context.vsync_cycles, VSYNC_CYCLES_TOTAL);
    assert_int_equal(e_cpu_context.irq, 0);
    assert_int_equal(e_cpu_context.firq, 0);
    assert_int_equal(e_cpu_context.nmi, 0);
    assert_int_equal(e_cpu_context.halted_state, HS_NONE);
    assert_int_equal(e_cpu_context.swi_hook, 0);
    assert_int_equal(e_cpu_context.swi_hook_set, 0);

    for (int i = 0; i < MEMORY_SIZE; i++) {
        /* Skip the default interrupt vector locations initiatlized in
           core_init() */
        switch (i) {
        case RESET_VECTOR - 0x4000:
        case RESET_VECTOR + 1 - 0x4000:
        case NMI_VECTOR - 0x4000:
        case NMI_VECTOR + 1 - 0x4000:
        case SWI_VECTOR - 0x4000:
        case SWI_VECTOR + 1 - 0x4000:
        case IRQ_VECTOR - 0x4000:
        case IRQ_VECTOR + 1 - 0x4000:
        case FIRQ_VECTOR - 0x4000:
        case FIRQ_VECTOR + 1 - 0x4000:
        case SWI2_VECTOR - 0x4000:
        case SWI2_VECTOR + 1 - 0x4000:
        case SWI3_VECTOR - 0x4000:
        case SWI3_VECTOR + 1 - 0x4000:
            continue;
        }

        assert_int_equal(e_cpu_context.memory[i], 0);
    }

    assert_int_equal(e_cpu_context.sam_state.ty_control_bit, 0);
    assert_int_equal(e_cpu_context.sam_state.m1_control_bit, 1);
    assert_int_equal(e_cpu_context.sam_state.m0_control_bit, 0);
    assert_int_equal(e_cpu_context.sam_state.r1_control_bit, 0);
    assert_int_equal(e_cpu_context.sam_state.r0_control_bit, 0);
    assert_int_equal(e_cpu_context.sam_state.p1_control_bit, 0);
    assert_int_equal(e_cpu_context.sam_state.f6_control_bit, 0);
    assert_int_equal(e_cpu_context.sam_state.f5_control_bit, 0);
    assert_int_equal(e_cpu_context.sam_state.f4_control_bit, 0);
    assert_int_equal(e_cpu_context.sam_state.f3_control_bit, 0);
    assert_int_equal(e_cpu_context.sam_state.f2_control_bit, 0);
    assert_int_equal(e_cpu_context.sam_state.f1_control_bit, 1);
    assert_int_equal(e_cpu_context.sam_state.f0_control_bit, 0);
    assert_int_equal(e_cpu_context.sam_state.v2_control_bit, 0);
    assert_int_equal(e_cpu_context.sam_state.v1_control_bit, 0);
    assert_int_equal(e_cpu_context.sam_state.v0_control_bit, 0);

    assert_int_equal(e_cpu_context.pia_state.ddr_1_a, 0);
    assert_int_equal(e_cpu_context.pia_state.dddr_1_a, 0);
    assert_int_equal(e_cpu_context.pia_state.ddr_1_b, 0);
    assert_int_equal(e_cpu_context.pia_state.dddr_1_b, 0);
    assert_int_equal(e_cpu_context.pia_state.ddr_2_a, 0);
    assert_int_equal(e_cpu_context.pia_state.dddr_2_a, 0);
    assert_int_equal(e_cpu_context.pia_state.ddr_2_b, 0);
    assert_int_equal(e_cpu_context.pia_state.dddr_2_b, 0);
    for (int i = 0; i < PIA_KEYBOARD_SIZE; i++) {
        assert_int_equal(e_cpu_context.pia_state.host_keys[i], 0);
    }
}

void basic_rom_test(void **state) {
    (void) state; /* unused */

    load_roms();

    /* RAM / ROM mode and normal page order */
    e_cpu_context.sam_state.ty_control_bit = 0;
    e_cpu_context.sam_state.p1_control_bit = 0;

    assert_int_equal(coco_read_byte_from_memory(0xA000), 0xA1);
    assert_int_equal(coco_read_byte_from_memory(0xA001), 0xCB);
    assert_int_equal(coco_read_byte_from_memory(0xBFFE), 0xA0);
    assert_int_equal(coco_read_byte_from_memory(0xBFFF), 0x27);
}

void extended_rom_test(void **state) {
    (void) state; /* unused */

    load_roms();

    /* RAM / ROM mode and normal page order */
    e_cpu_context.sam_state.ty_control_bit = 0;
    e_cpu_context.sam_state.p1_control_bit = 0;

    assert_int_equal(coco_read_byte_from_memory(0x8000), 0x45);
    assert_int_equal(coco_read_byte_from_memory(0x8001), 0x58);
    assert_int_equal(coco_read_byte_from_memory(0x9FFE), 0x44);
    assert_int_equal(coco_read_byte_from_memory(0x9FFF), 0x39);
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

void run_hsync_interval_test(void **state) {
    /* This code is normally set to iterate 255 times but will now break after
       the appropriate hsync cycle amount */
    set_reg_value_8(REG_B, 0xFF);
    uint8 code_bytes[] = {
        OP_INCA,
        OP_INCA,
        OP_INCA,
        OP_DECB,
        OP_BNE,
        0xFA
    };
    struct mem_loader_def test_memory[] = {
        { 0x100, code_bytes, 6 },
    };
    load_memory(test_memory, 1);

    uint32 this_cycles = run_hsync_interval();
    assert_in_range(this_cycles, 50, 64);

    assert_int_equal(e_cpu_context.irq, 0);
}

void run_hsync_interval_irq_test(void **state) {
    set_reg_value_8(REG_B, 0xFF);
    uint8 code_bytes[] = {
        OP_INCA,
        OP_INCA,
        OP_INCA,
        OP_DECB,
        OP_BNE,
        0xFA
    };
    struct mem_loader_def test_memory[] = {
        { 0x100, code_bytes, 6 },
    };
    load_memory(test_memory, 1);

    /* Enable the HSYNC interrupt which should be firing after the instructions
       have run */
    uint8 pia1_cra = pia_read_byte_from_memory(0xFF01);
    pia1_cra |= 0x1;
    pia_write_byte_to_memory(0xFF01, pia1_cra);
    e_cpu_context.cc.i = 0;

    uint32 this_cycles = run_hsync_interval();
    assert_in_range(this_cycles, 50, 64);

    assert_int_equal(e_cpu_context.irq, 1);
}

void run_vsync_interval_irq_test(void **state) {
    set_reg_value_16(REG_D, 0xFFFF);
    set_reg_value_16(REG_PC, 0x1000);

    /* 255 * 255 doubly nested loop, we are expecting to have
       run cycles break out after VSYNC cycles */
    uint8 code_bytes[] = {
        OP_NOP,
        OP_DECA,
        OP_BNE,
        0xFC,
        OP_LDA,
        0xFF,
        OP_DECB,
        OP_BNE,
        0xF7
    };
    struct mem_loader_def test_memory[] = {
        { 0x100, code_bytes, 13 },
    };
    load_memory(test_memory, 1);

    /* Enable the VSYNC interrupt which should be firing after the instructions
       have run */
    uint8 pia1_crb = pia_read_byte_from_memory(0xFF03);
    pia1_crb |= 0x1;
    pia_write_byte_to_memory(0xFF03, pia1_crb);
    e_cpu_context.cc.i = 0;

    uint32 this_cycles = run_cycles(VSYNC_CYCLES_TOTAL);
    assert_in_range(this_cycles, VSYNC_CYCLES_TOTAL - 10,
                    VSYNC_CYCLES_TOTAL + 10);

    /* Execution will break out of the main loop before the interrupt triggering
       core has had a chance to run. But the flag will have been set by the
       vsync housekeeping routine. */
    assert_int_equal(e_cpu_context.irq, 1);
}

void process_interrupts_nmi_test(void **state) {
    (void) state; /* unused */

    set_reg_value_8(REG_A, 0x2);
    set_reg_value_16(REG_PC, 0x100);

    uint8 code_bytes[] = {
        OP_INCA,
        OP_INCA,
        OP_INCA
    };
    uint8 nmi_bytes[] = {
        OP_CLRA,
        OP_RTI
    };

    struct mem_loader_def test_memory[] = {
        { 0x100, code_bytes, 3 },
        { DEFAULT_NMI_VECTOR, nmi_bytes, 2 }
    };
    load_memory(test_memory, 2);

    /* Interrupt processor execution after the second INCA */
    int cycles = run_cycles(opcode_table[OP_INCA].cycle_count);
    cycles += run_cycles(opcode_table[OP_INCA].cycle_count);

    assert_int_equal(get_reg_value_8(REG_A), 0x4);
    e_cpu_context.nmi = 1;
    cycles += run_cycles(opcode_table[OP_INCA].cycle_count);

    /* Two INCA's at 2 cycles each plus 12 cycles for all registers being
       pushed */
    assert_int_equal(cycles, 16);
    assert_int_equal(get_reg_value_16(REG_PC), DEFAULT_NMI_VECTOR);
}

void process_interrupts_nmi_with_rti_test(void **state) {
    (void) state; /* unused */

    set_reg_value_8(REG_A, 0x2);
    set_reg_value_16(REG_PC, 0x100);

    uint8 code_bytes[] = {
        OP_INCA,
        OP_INCA,
        OP_INCA
    };
    uint8 nmi_bytes[] = {
        OP_CLRA,
        OP_RTI
    };

    struct mem_loader_def test_memory[] = {
        { 0x100, code_bytes, 3 },
        { DEFAULT_NMI_VECTOR, nmi_bytes, 2 }
    };
    load_memory(test_memory, 2);

    /* Interrupt processor execution after the second INCA */
    int cycles = run_cycles(opcode_table[OP_INCA].cycle_count);
    cycles += run_cycles(opcode_table[OP_INCA].cycle_count);

    assert_int_equal(get_reg_value_8(REG_A), 0x4);
    e_cpu_context.nmi = 1;
    cycles += run_cycles(opcode_table[OP_INCA].cycle_count);

    /* Two INCA's at 2 cycles each plus 12 cycles for all registers being
       pushed */
    assert_int_equal(cycles, 16);
    assert_int_equal(get_reg_value_16(REG_PC), DEFAULT_NMI_VECTOR);
    assert_int_equal(e_cpu_context.cc.f, 1);
    assert_int_equal(e_cpu_context.cc.i, 1);

    /* Interrupt routine clears A but we expect its value to be restored
       by the RTI instruction */
    cycles += run_cycles(opcode_table[OP_CLRA].cycle_count);
    assert_int_equal(get_reg_value_8(REG_A), 0);

    cycles += run_cycles(opcode_table[OP_RTI].cycle_count);
    assert_int_equal(get_reg_value_16(REG_PC), 0x100 + 2);
    assert_int_equal(get_reg_value_8(REG_A), 0x4);

    cycles += run_cycles(opcode_table[OP_INCA].cycle_count);
    /* Previous value (16), plus one CLRA (2), one RTI w/ e enabled (15)
       and finally one INCA (2) = 35 */
    assert_int_equal(cycles, 35);
    assert_int_equal(get_reg_value_8(REG_A), 0x5);
    assert_int_equal(e_cpu_context.cc.f, 0);
    assert_int_equal(e_cpu_context.cc.i, 0);
}

void process_interrupts_firq_test(void **state) {
    (void) state; /* unused */

    set_reg_value_8(REG_A, 0x2);
    set_reg_value_16(REG_PC, 0x100);

    uint8 code_bytes[] = {
        OP_INCA,
        OP_INCA,
        OP_INCA
    };
    uint8 firq_bytes[] = {
        OP_CLRA,
        OP_RTI
    };

    struct mem_loader_def test_memory[] = {
        { 0x100, code_bytes, 3 },
        { DEFAULT_FIRQ_VECTOR, firq_bytes, 2 }
    };
    load_memory(test_memory, 2);

    /* Interrupt processor execution after the second INCA */
    int cycles = run_cycles(opcode_table[OP_INCA].cycle_count);
    cycles += run_cycles(opcode_table[OP_INCA].cycle_count);

    assert_int_equal(get_reg_value_8(REG_A), 0x4);
    e_cpu_context.firq = 1;
    cycles += run_cycles(opcode_table[OP_INCA].cycle_count);

    /* Two INCA's at 2 cycles each plus 3 cycles for PC and CC registers being
       pushed */
    assert_int_equal(cycles, 7);
    assert_int_equal(get_reg_value_16(REG_PC), DEFAULT_FIRQ_VECTOR);
    assert_int_equal(e_cpu_context.cc.f, 1);
    assert_int_equal(e_cpu_context.cc.i, 1);
}

void process_interrupts_firq_with_rti_test(void **state) {
    (void) state; /* unused */

    set_reg_value_8(REG_A, 0x2);
    set_reg_value_16(REG_PC, 0x100);

    uint8 code_bytes[] = {
        OP_INCA,
        OP_INCA,
        OP_INCA
    };
    uint8 firq_bytes[] = {
        OP_CLRA,
        OP_RTI
    };

    struct mem_loader_def test_memory[] = {
        { 0x100, code_bytes, 3 },
        { DEFAULT_FIRQ_VECTOR, firq_bytes, 2 }
    };
    load_memory(test_memory, 2);

    /* Interrupt processor execution after the second INCA */
    int cycles = run_cycles(opcode_table[OP_INCA].cycle_count);
    cycles += run_cycles(opcode_table[OP_INCA].cycle_count);

    assert_int_equal(get_reg_value_8(REG_A), 0x4);
    assert_int_equal(e_cpu_context.cc.f, 0);
    assert_int_equal(e_cpu_context.cc.i, 0);
    e_cpu_context.firq = 1;
    cycles += run_cycles(opcode_table[OP_INCA].cycle_count);

    /* Two INCA's at 2 cycles each plus 3 cycles for PC and CC registers being
       pushed */
    assert_int_equal(cycles, 7);
    assert_int_equal(get_reg_value_16(REG_PC), DEFAULT_FIRQ_VECTOR);
    /* FIRQ and IRQ are masked during FIRQ execution */
    assert_int_equal(e_cpu_context.cc.f, 1);
    assert_int_equal(e_cpu_context.cc.i, 1);
    /* Turn off interrupt so it doesn't get triggered again for the test */
    e_cpu_context.firq = 0;

    /* Interrupt routine clears A but we expect its value to not be restored
       by the RTI instruction */
    cycles += run_cycles(opcode_table[OP_CLRA].cycle_count);
    assert_int_equal(e_cpu_context.cc.e, 0);
    assert_int_equal(cycles, 9);
    assert_int_equal(get_reg_value_8(REG_A), 0);

    cycles += run_cycles(opcode_table[OP_RTI].cycle_count);
    assert_int_equal(cycles, 15);
    assert_int_equal(get_reg_value_16(REG_PC), 0x100 + 2);
    assert_int_equal(get_reg_value_8(REG_A), 0);

    cycles += run_cycles(opcode_table[OP_INCA].cycle_count);
    /* Previous value (7), plus one CLRA (2), one RTI w/o e enabled (6)
       and finally one INCA (2) = 17 */
    assert_int_equal(cycles, 17);
    assert_int_equal(get_reg_value_8(REG_A), 0x1);
    assert_int_equal(e_cpu_context.cc.f, 0);
    assert_int_equal(e_cpu_context.cc.i, 0);
}

void process_interrupts_irq_test(void **state) {
    (void) state; /* unused */

    set_reg_value_8(REG_A, 0x2);
    set_reg_value_16(REG_PC, 0x100);

    uint8 code_bytes[] = {
        OP_INCA,
        OP_INCA,
        OP_INCA
    };
    uint8 irq_bytes[] = {
        OP_CLRA,
        OP_RTI
    };

    struct mem_loader_def test_memory[] = {
        { 0x100, code_bytes, 3 },
        { DEFAULT_IRQ_VECTOR, irq_bytes, 2 }
    };
    load_memory(test_memory, 2);

    /* Interrupt processor execution after the second INCA */
    int cycles = run_cycles(opcode_table[OP_INCA].cycle_count);
    cycles += run_cycles(opcode_table[OP_INCA].cycle_count);

    assert_int_equal(get_reg_value_8(REG_A), 0x4);
    e_cpu_context.irq = 1;
    cycles += run_cycles(opcode_table[OP_INCA].cycle_count);

    /* Two INCA's at 2 cycles each plus 12 cycles for all registers being
       pushed */
    assert_int_equal(cycles, 16);
    assert_int_equal(get_reg_value_16(REG_PC), DEFAULT_IRQ_VECTOR);
    assert_int_equal(e_cpu_context.cc.f, 0);
    assert_int_equal(e_cpu_context.cc.i, 1);
}

void process_interrupts_irq_with_rti_test(void **state) {
    (void) state; /* unused */

    set_reg_value_8(REG_A, 0x2);
    set_reg_value_16(REG_PC, 0x100);

    uint8 code_bytes[] = {
        OP_INCA,
        OP_INCA,
        OP_INCA
    };
    uint8 irq_bytes[] = {
        OP_CLRA,
        OP_RTI
    };

    struct mem_loader_def test_memory[] = {
        { 0x100, code_bytes, 3 },
        { DEFAULT_IRQ_VECTOR, irq_bytes, 2 }
    };
    load_memory(test_memory, 2);

    /* Interrupt processor execution after the second INCA */
    int cycles = run_cycles(opcode_table[OP_INCA].cycle_count);
    cycles += run_cycles(opcode_table[OP_INCA].cycle_count);

    assert_int_equal(get_reg_value_8(REG_A), 0x4);
    assert_int_equal(e_cpu_context.cc.f, 0);
    assert_int_equal(e_cpu_context.cc.i, 0);
    e_cpu_context.irq = 1;
    cycles += run_cycles(opcode_table[OP_INCA].cycle_count);

    /* Two INCA's at 2 cycles each plus 12 cycles for all registers being
       pushed */
    assert_int_equal(cycles, 16);
    assert_int_equal(get_reg_value_16(REG_PC), DEFAULT_IRQ_VECTOR);
    /* IRQ is masked during IRQ execution, not FIRQ */
    assert_int_equal(e_cpu_context.cc.f, 0);
    assert_int_equal(e_cpu_context.cc.i, 1);
    /* Turn off interrupt so it doesn't get triggered again for the test */
    e_cpu_context.irq = 0;

    /* Interrupt routine clears A but we expect its value to be restored
       by the RTI instruction */
    cycles += run_cycles(opcode_table[OP_CLRA].cycle_count);
    assert_int_equal(e_cpu_context.cc.e, 1);
    assert_int_equal(cycles, 18);
    assert_int_equal(get_reg_value_8(REG_A), 0);

    cycles += run_cycles(opcode_table[OP_RTI].cycle_count);
    /* Previous value (16), plus one CLRA (2), one RTI w/ e enabled (15) */
    assert_int_equal(cycles, 33);
    assert_int_equal(get_reg_value_16(REG_PC), 0x100 + 2);
    assert_int_equal(get_reg_value_8(REG_A), 4);

    cycles += run_cycles(opcode_table[OP_INCA].cycle_count);
    /* Previous value (16), plus one CLRA (2), one RTI w/ e enabled (15)
       and finally one INCA (2) = 35 */
    assert_int_equal(cycles, 35);
    assert_int_equal(get_reg_value_8(REG_A), 0x5);
    assert_int_equal(e_cpu_context.cc.f, 0);
    assert_int_equal(e_cpu_context.cc.i, 0);
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

void disassemble_instruction_notimpl_test(void **state) {
    (void) state; /* unused */

    uint8 code_bytes[] = {
        0x1
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1 },
    };
    load_memory(test_memory, 1);
    e_cpu_context.pc = USER_SPACE_ROOT;

    char decoded[100];
    uint8 num_bytes = disassemble_instruction(e_cpu_context.pc, decoded);

    assert_int_equal(num_bytes, 1);
    assert_string_equal(decoded, "$01");
}

void disassemble_instruction_ext_notimpl_test(void **state) {
    (void) state; /* unused */

    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        0x1
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
    };
    load_memory(test_memory, 1);
    e_cpu_context.pc = USER_SPACE_ROOT;

    char decoded[100];
    uint8 num_bytes = disassemble_instruction(e_cpu_context.pc, decoded);

    assert_int_equal(num_bytes, 2);
    assert_string_equal(decoded, "$10 $01");
}

void disassemble_instruction_inherent_test(void **state) {
    (void) state; /* unused */

    uint8 code_bytes[] = {
        OP_MUL
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 1 },
    };
    load_memory(test_memory, 1);
    e_cpu_context.pc = USER_SPACE_ROOT;

    char decoded[100];
    uint8 num_bytes = disassemble_instruction(e_cpu_context.pc, decoded);

    assert_int_equal(num_bytes, 1);
    assert_string_equal(decoded, "MUL");
}

void disassemble_instruction_immediate_test(void **state) {
    (void) state; /* unused */

    uint8 code_bytes[] = {
        OP_CMPA,
        0x69
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
    };
    load_memory(test_memory, 1);
    e_cpu_context.pc = USER_SPACE_ROOT;

    char decoded[100];
    uint8 num_bytes = disassemble_instruction(e_cpu_context.pc, decoded);

    assert_int_equal(num_bytes, 2);
    assert_string_equal(decoded, "CMPA #$69");
}

void disassemble_instruction_immediate_16_bit_test(void **state) {
    (void) state; /* unused */

    uint8 code_bytes[] = {
        OP_CMPX,
        0x12,
        0x34
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
    };
    load_memory(test_memory, 1);
    e_cpu_context.pc = USER_SPACE_ROOT;

    char decoded[100];
    uint8 num_bytes = disassemble_instruction(e_cpu_context.pc, decoded);

    assert_int_equal(num_bytes, 3);
    assert_string_equal(decoded, "CMPX #$1234");
}

void disassemble_instruction_immediate_ext_16_bit_test(void **state) {
    (void) state; /* unused */

    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LDS,
        0x12,
        0x34
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 },
    };
    load_memory(test_memory, 1);
    e_cpu_context.pc = USER_SPACE_ROOT;

    char decoded[100];
    uint8 num_bytes = disassemble_instruction(e_cpu_context.pc, decoded);

    assert_int_equal(num_bytes, 4);
    assert_string_equal(decoded, "LDS #$1234");
}

void disassemble_instruction_extended_test(void **state) {
    (void) state; /* unused */

    uint8 code_bytes[] = {
        OP_DEC_E,
        0x12,
        0x34
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
    };
    load_memory(test_memory, 1);
    e_cpu_context.pc = USER_SPACE_ROOT;

    char decoded[100];
    uint8 num_bytes = disassemble_instruction(e_cpu_context.pc, decoded);

    assert_int_equal(num_bytes, 3);
    assert_string_equal(decoded, "DEC >$1234");
}

void disassemble_instruction_direct_test(void **state) {
    (void) state; /* unused */

    uint8 code_bytes[] = {
        OP_DEC_D,
        0x34
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
    };
    load_memory(test_memory, 1);
    e_cpu_context.pc = USER_SPACE_ROOT;
    e_cpu_context.dp = 0x12;

    char decoded[100];
    uint8 num_bytes = disassemble_instruction(e_cpu_context.pc, decoded);

    assert_int_equal(num_bytes, 2);
    assert_string_equal(decoded, "DEC >$1234");
}

void disassemble_instruction_indexed_const_off_test(void **state) {
    (void) state; /* unused */

    uint8 code_bytes[] = {
        OP_STA_I,
        0x64 /* const offset of +4 from S */
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
    };
    load_memory(test_memory, 1);
    e_cpu_context.pc = USER_SPACE_ROOT;

    char decoded[100];
    uint8 num_bytes = disassemble_instruction(e_cpu_context.pc, decoded);

    assert_int_equal(num_bytes, 2);
    assert_string_equal(decoded, "STA 4,S");
}

void disassemble_instruction_indexed_ext_indirect_test(void **state) {
    (void) state; /* unused */

    uint8 code_bytes[] = {
        OP_ORA_I,
        0x9F, /* extended indirect */
        0x12,
        0x34
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 },
    };
    load_memory(test_memory, 1);
    e_cpu_context.pc = USER_SPACE_ROOT;

    char decoded[100];
    uint8 num_bytes = disassemble_instruction(e_cpu_context.pc, decoded);

    assert_int_equal(num_bytes, 4);
    assert_string_equal(decoded, "ORA [$1234]");
}

void disassemble_instruction_indexed_const_off_no_off_test(void **state) {
    (void) state; /* unused */

    uint8 code_bytes[] = {
        OP_STA_I,
        0xE4 /* const offset of 0 from S */
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
    };
    load_memory(test_memory, 1);
    e_cpu_context.pc = USER_SPACE_ROOT;

    char decoded[100];
    uint8 num_bytes = disassemble_instruction(e_cpu_context.pc, decoded);

    assert_int_equal(num_bytes, 2);
    assert_string_equal(decoded, "STA ,S");
}

void disassemble_instruction_indexed_const_off_16_bit_test(void **state) {
    (void) state; /* unused */

    uint8 code_bytes[] = {
        OP_ADDA_I,
        0xA9, /* const offset of 0x1234 from Y */
        0x12,
        0x34
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 },
    };
    load_memory(test_memory, 1);
    e_cpu_context.pc = USER_SPACE_ROOT;

    char decoded[100];
    uint8 num_bytes = disassemble_instruction(e_cpu_context.pc, decoded);

    assert_int_equal(num_bytes, 4);
    assert_string_equal(decoded, "ADDA 4660,Y");
}

void disassemble_instruction_indexed_const_off_8_bit_indirect_test(void **state) {
    (void) state; /* unused */

    uint8 code_bytes[] = {
        OP_JSR_I,
        0xD8, /* const offset of -20 from U, indirected */
        0xEC
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
    };
    load_memory(test_memory, 1);
    e_cpu_context.pc = USER_SPACE_ROOT;

    char decoded[100];
    uint8 num_bytes = disassemble_instruction(e_cpu_context.pc, decoded);

    assert_int_equal(num_bytes, 3);
    assert_string_equal(decoded, "JSR [-20,U]");
}

void disassemble_instruction_indexed_acc_test(void **state) {
    uint8 code_bytes[] = {
        OP_SBCA_I,
        0x85 /* B acc offset from X */
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
    };
    load_memory(test_memory, 1);
    e_cpu_context.pc = USER_SPACE_ROOT;

    char decoded[100] = { 0 };
    uint8 num_bytes = disassemble_instruction(e_cpu_context.pc, decoded);

    assert_int_equal(num_bytes, 2);
    assert_string_equal(decoded, "SBCA B,X");
}

void disassemble_instruction_indexed_acc_indirect_test(void **state) {
    uint8 code_bytes[] = {
        OP_ANDA_I,
        0xF6 /* A acc offset from S, indirected */
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 2 },
    };
    load_memory(test_memory, 1);
    e_cpu_context.pc = USER_SPACE_ROOT;

    char decoded[100] = { 0 };
    uint8 num_bytes = disassemble_instruction(e_cpu_context.pc, decoded);

    assert_int_equal(num_bytes, 2);
    assert_string_equal(decoded, "ANDA [A,S]");
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

void push_byte_to_stack_basic_test(void **state) {
    (void) state; /* unused */

    e_cpu_context.s = 0x100;
    push_byte_to_stack(REG_S, 0x69);

    assert_int_equal(get_reg_value_16(REG_S), 0xFF);
    assert_int_equal(read_byte_from_memory(0xFF), 0x69);
}

void pull_byte_from_stack_basic_test(void **state) {
    (void) state; /* unused */

    e_cpu_context.s = 0xFF;
    write_byte_to_memory(0xFF, 0x69);
    uint8 read_byte = pull_byte_from_stack(REG_S);

    assert_int_equal(read_byte, 0x69);
    assert_int_equal(get_reg_value_16(REG_S), 0x100);
}

void push_byte_to_stack_invalid_test(void **state) {
    e_cpu_context.s = 0x100;

    expect_assert_failure(push_byte_to_stack(REG_X, 0x69));
}

void push_pull_byte_stack_combo_test(void **state) {
    e_cpu_context.s = 0x100;

    push_byte_to_stack(REG_S, 0x12);
    push_byte_to_stack(REG_S, 0x34);
    push_byte_to_stack(REG_S, 0x56);

    assert_int_equal(pull_byte_from_stack(REG_S), 0x56);
    assert_int_equal(pull_byte_from_stack(REG_S), 0x34);
    assert_int_equal(pull_byte_from_stack(REG_S), 0x12);
}

void push_byte_to_stack_u_test(void **state) {
    (void) state; /* unused */

    e_cpu_context.s = 0x100;
    e_cpu_context.u = 0x200;
    push_byte_to_stack(REG_U, 0x69);

    assert_int_equal(get_reg_value_16(REG_U), 0x1FF);
    assert_int_equal(read_byte_from_memory(0x1FF), 0x69);
    assert_int_equal(get_reg_value_16(REG_S), 0x100);
    assert_int_equal(read_byte_from_memory(0x100), 0);
}

void push_word_to_stack_basic_test(void **state) {
    (void) state; /* unused */

    e_cpu_context.s = 0x100;
    push_word_to_stack(REG_S, 0x69AB);

    assert_int_equal(get_reg_value_16(REG_S), 0xFE);
    assert_int_equal(read_word_from_memory(0xFE), 0x69AB);
}

void pull_word_from_stack_basic_test(void **state) {
    (void) state; /* unused */

    e_cpu_context.s = 0xFE;
    write_word_to_memory(0xFE, 0x69AB);
    uint16 read_word = pull_word_from_stack(REG_S);

    assert_int_equal(read_word, 0x69AB);
    assert_int_equal(get_reg_value_16(REG_S), 0x100);
}

void push_word_to_stack_invalid_test(void **state) {
    e_cpu_context.s = 0x100;

    expect_assert_failure(push_word_to_stack(REG_X, 0x69AB));
}

void push_pull_word_stack_combo_test(void **state) {
    e_cpu_context.s = 0x100;

    push_word_to_stack(REG_S, 0x12);
    push_word_to_stack(REG_S, 0x34);
    push_word_to_stack(REG_S, 0x56);

    assert_int_equal(pull_word_from_stack(REG_S), 0x56);
    assert_int_equal(pull_word_from_stack(REG_S), 0x34);
    assert_int_equal(pull_word_from_stack(REG_S), 0x12);
}

void push_word_to_stack_u_test(void **state) {
    (void) state; /* unused */

    e_cpu_context.s = 0x100;
    e_cpu_context.u = 0x200;
    push_word_to_stack(REG_U, 0x69AB);

    assert_int_equal(get_reg_value_16(REG_U), 0x1FE);
    assert_int_equal(read_word_from_memory(0x1FE), 0x69AB);
    assert_int_equal(get_reg_value_16(REG_S), 0x100);
    assert_int_equal(read_word_from_memory(0x100), 0);
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
