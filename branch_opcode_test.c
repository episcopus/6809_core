#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "core.h"

extern struct cpu_state e_cpu_context;
extern struct opcode_def opcode_table[];
extern struct opcode_def opcode_ext_x10_table[];
extern struct opcode_def opcode_ext_x11_table[];

void bcc_basic_test(void **state) {
    (void) state; /* unused */
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BCC,
        offset, /* Signed 2's complement offset */
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        /* Branch address is calculated by taking the baseline user space, +2
           for the calling branch opcode and offset bytes, and then the offset,
           per the spec for branching operations. offset is cast to char to
           enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 2 + (char) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.c = 0;
    int cycles = run_cycles(opcode_table[OP_BCC].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_table[OP_BCC].cycle_count);
    assert_int_equal(post_pc, USER_SPACE_ROOT + 2 + (char) offset);
}

void bcc_non_branch_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BCC,
        offset, /* Signed 2's complement offset */
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        /* Branch address is calculated by taking the baseline user space, +2
           for the calling branch opcode and offset bytes, and then the offset,
           per the spec for branching operations. offset is cast to char to
           enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 2 + (char) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.c = 1;
    int cycles = run_cycles(opcode_table[OP_BCC].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_table[OP_BCC].cycle_count);
    assert_int_equal(post_pc, pre_pc + 2);
}

void bcc_negative_offset_test(void **state) {
    (void) state; /* unused */
    uint8 offset = 0xC0; /* -64 */

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BCC,
        offset, /* Signed 2's complement offset */
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        /* Branch address is calculated by taking the baseline user space, +2
           for the calling branch opcode and offset bytes, and then the offset,
           per the spec for branching operations. offset is cast to char to
           enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 2 + (char) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.c = 0;
    int cycles = run_cycles(opcode_table[OP_BCC].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_table[OP_BCC].cycle_count);
    assert_int_equal(post_pc, USER_SPACE_ROOT + 2 + (char) offset);
}

void bcc_negative_offset_non_branch_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;
    uint8 offset = 0xC0; /* -64 */

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BCC,
        offset, /* Signed 2's complement offset */
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 3 },
        /* Branch address is calculated by taking the baseline user space, +2
           for the calling branch opcode and offset bytes, and then the offset,
           per the spec for branching operations. offset is cast to char to
           enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 2 + (char) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.c = 1;
    int cycles = run_cycles(opcode_table[OP_BCC].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_table[OP_BCC].cycle_count);
    assert_int_equal(post_pc, pre_pc + 2);
}
