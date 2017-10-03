#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "headers.h"

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

/*****************************************/

void lbcc_basic_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBCC,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.c = 0;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBCC].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, USER_SPACE_ROOT + 4 + (short int) offset);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBCC].cycle_count + 1);
}

void lbcc_non_branch_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;
    int pre_pc = e_cpu_context.pc;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBCC,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.c = 1;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBCC].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, pre_pc + 4);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBCC].cycle_count);
}

void lbcc_negative_offset_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0xF000; /* negative 0x1000 in 2's complement */

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBCC,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.c = 0;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBCC].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, USER_SPACE_ROOT + 4 + (short int) offset);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBCC].cycle_count + 1);
}

void lbcc_negative_offset_non_branch_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0xF000; /* negative 0x1000 in 2's complement */
    int pre_pc = e_cpu_context.pc;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBCC,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.c = 1;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBCC].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, pre_pc + 4);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBCC].cycle_count);
}

void bcs_basic_test(void **state) {
    (void) state; /* unused */
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BCS,
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
    int cycles = run_cycles(opcode_table[OP_BCS].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_table[OP_BCS].cycle_count);
    assert_int_equal(post_pc, USER_SPACE_ROOT + 2 + (char) offset);
}

void bcs_non_branch_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BCS,
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
    int cycles = run_cycles(opcode_table[OP_BCS].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_table[OP_BCS].cycle_count);
    assert_int_equal(post_pc, pre_pc + 2);
}

void lbcs_basic_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBCS,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.c = 1;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBCS].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, USER_SPACE_ROOT + 4 + (short int) offset);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBCS].cycle_count + 1);
}

void lbcs_non_branch_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;
    int pre_pc = e_cpu_context.pc;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBCS,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.c = 0;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBCS].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, pre_pc + 4);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBCS].cycle_count);
}

void beq_basic_test(void **state) {
    (void) state; /* unused */
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BEQ,
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

    e_cpu_context.cc.z = 1;
    int cycles = run_cycles(opcode_table[OP_BEQ].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_table[OP_BEQ].cycle_count);
    assert_int_equal(post_pc, USER_SPACE_ROOT + 2 + (char) offset);
}

void beq_non_branch_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BEQ,
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

    e_cpu_context.cc.z = 0;
    int cycles = run_cycles(opcode_table[OP_BEQ].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_table[OP_BEQ].cycle_count);
    assert_int_equal(post_pc, pre_pc + 2);
}

void lbeq_basic_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBEQ,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.z = 1;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBEQ].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, USER_SPACE_ROOT + 4 + (short int) offset);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBEQ].cycle_count + 1);
}

void lbeq_non_branch_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;
    int pre_pc = e_cpu_context.pc;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBEQ,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.z = 0;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBEQ].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, pre_pc + 4);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBEQ].cycle_count);
}

void bge_basic_test(void **state) {
    (void) state; /* unused */
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BGE,
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

    e_cpu_context.cc.n = e_cpu_context.cc.v = 1;
    int cycles = run_cycles(opcode_table[OP_BGE].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_table[OP_BGE].cycle_count);
    assert_int_equal(post_pc, USER_SPACE_ROOT + 2 + (char) offset);
}

void bge_non_branch_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BGE,
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

    e_cpu_context.cc.n = 1;
    e_cpu_context.cc.v = 0;
    int cycles = run_cycles(opcode_table[OP_BGE].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_table[OP_BGE].cycle_count);
    assert_int_equal(post_pc, pre_pc + 2);
}

void lbge_basic_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBGE,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.n = e_cpu_context.cc.v = 1;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBGE].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, USER_SPACE_ROOT + 4 + (short int) offset);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBGE].cycle_count + 1);
}

void lbge_non_branch_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;
    int pre_pc = e_cpu_context.pc;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBGE,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.n = 1;
    e_cpu_context.cc.v = 0;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBGE].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, pre_pc + 4);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBGE].cycle_count);
}

void bgt_basic_test(void **state) {
    (void) state; /* unused */
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BGT,
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

    e_cpu_context.cc.n = e_cpu_context.cc.v = 1;
    e_cpu_context.cc.z = 0;
    int cycles = run_cycles(opcode_table[OP_BGT].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_table[OP_BGT].cycle_count);
    assert_int_equal(post_pc, USER_SPACE_ROOT + 2 + (char) offset);
}

void bgt_non_branch_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BGT,
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

    e_cpu_context.cc.n = e_cpu_context.cc.v = 0;
    e_cpu_context.cc.z = 1;
    int cycles = run_cycles(opcode_table[OP_BGT].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_table[OP_BGT].cycle_count);
    assert_int_equal(post_pc, pre_pc + 2);
}

void lbgt_basic_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBGT,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.n = e_cpu_context.cc.v = 1;
    e_cpu_context.cc.z = 0;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBGT].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, USER_SPACE_ROOT + 4 + (short int) offset);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBGT].cycle_count + 1);
}

void lbgt_non_branch_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;
    int pre_pc = e_cpu_context.pc;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBGT,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.n = e_cpu_context.cc.v = 1;
    e_cpu_context.cc.z = 1;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBGT].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, pre_pc + 4);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBGT].cycle_count);
}

void bhi_basic_test(void **state) {
    (void) state; /* unused */
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BHI,
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
    e_cpu_context.cc.z = 0;
    int cycles = run_cycles(opcode_table[OP_BHI].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_table[OP_BHI].cycle_count);
    assert_int_equal(post_pc, USER_SPACE_ROOT + 2 + (char) offset);
}

void bhi_non_branch_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BHI,
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
    e_cpu_context.cc.z = 0;
    int cycles = run_cycles(opcode_table[OP_BHI].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_table[OP_BHI].cycle_count);
    assert_int_equal(post_pc, pre_pc + 2);
}

void lbhi_basic_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBHI,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.c = 0;
    e_cpu_context.cc.z = 0;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBHI].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, USER_SPACE_ROOT + 4 + (short int) offset);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBHI].cycle_count + 1);
}

void lbhi_non_branch_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;
    int pre_pc = e_cpu_context.pc;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBHI,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.z = 1;
    e_cpu_context.cc.c = 1;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBHI].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, pre_pc + 4);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBHI].cycle_count);
}

void ble_basic_test(void **state) {
    (void) state; /* unused */
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BLE,
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

    e_cpu_context.cc.n = 0;
    e_cpu_context.cc.v = 1;
    e_cpu_context.cc.z = 0;
    int cycles = run_cycles(opcode_table[OP_BLE].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_table[OP_BLE].cycle_count);
    assert_int_equal(post_pc, USER_SPACE_ROOT + 2 + (char) offset);
}

void ble_non_branch_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BLE,
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

    e_cpu_context.cc.n = 1;
    e_cpu_context.cc.v = 1;
    e_cpu_context.cc.z = 0;
    int cycles = run_cycles(opcode_table[OP_BLE].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_table[OP_BLE].cycle_count);
    assert_int_equal(post_pc, pre_pc + 2);
}

void lble_basic_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBLE,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.n = 0;
    e_cpu_context.cc.v = 1;
    e_cpu_context.cc.z = 0;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBLE].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, USER_SPACE_ROOT + 4 + (short int) offset);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBLE].cycle_count + 1);
}

void lble_non_branch_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;
    int pre_pc = e_cpu_context.pc;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBLE,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.n = 1;
    e_cpu_context.cc.v = 1;
    e_cpu_context.cc.z = 0;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBLE].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, pre_pc + 4);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBLE].cycle_count);
}

void bls_basic_test(void **state) {
    (void) state; /* unused */
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BLS,
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
    e_cpu_context.cc.z = 1;
    int cycles = run_cycles(opcode_table[OP_BLS].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_table[OP_BLS].cycle_count);
    assert_int_equal(post_pc, USER_SPACE_ROOT + 2 + (char) offset);
}

void bls_non_branch_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BLS,
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
    e_cpu_context.cc.z = 0;
    int cycles = run_cycles(opcode_table[OP_BLS].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_table[OP_BLS].cycle_count);
    assert_int_equal(post_pc, pre_pc + 2);
}

void lbls_basic_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBLS,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.c = 0;
    e_cpu_context.cc.z = 1;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBLS].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, USER_SPACE_ROOT + 4 + (short int) offset);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBLS].cycle_count + 1);
}

void lbls_non_branch_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;
    int pre_pc = e_cpu_context.pc;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBLS,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.c = 0;
    e_cpu_context.cc.z = 0;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBLS].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, pre_pc + 4);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBLS].cycle_count);
}

void blt_basic_test(void **state) {
    (void) state; /* unused */
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BLT,
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

    e_cpu_context.cc.n = 0;
    e_cpu_context.cc.v = 1;
    int cycles = run_cycles(opcode_table[OP_BLT].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_table[OP_BLT].cycle_count);
    assert_int_equal(post_pc, USER_SPACE_ROOT + 2 + (char) offset);
}

void blt_non_branch_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BLT,
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

    e_cpu_context.cc.n = 1;
    e_cpu_context.cc.v = 1;
    int cycles = run_cycles(opcode_table[OP_BLT].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_table[OP_BLT].cycle_count);
    assert_int_equal(post_pc, pre_pc + 2);
}

void lblt_basic_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBLT,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.n = 0;
    e_cpu_context.cc.v = 1;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBLT].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, USER_SPACE_ROOT + 4 + (short int) offset);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBLT].cycle_count + 1);
}

void lblt_non_branch_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;
    int pre_pc = e_cpu_context.pc;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBLT,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.n = 1;
    e_cpu_context.cc.v = 1;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBLT].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, pre_pc + 4);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBLT].cycle_count);
}

void bmi_basic_test(void **state) {
    (void) state; /* unused */
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BMI,
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

    e_cpu_context.cc.n = 1;
    int cycles = run_cycles(opcode_table[OP_BMI].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_table[OP_BMI].cycle_count);
    assert_int_equal(post_pc, USER_SPACE_ROOT + 2 + (char) offset);
}

void bmi_non_branch_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BMI,
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

    e_cpu_context.cc.n = 0;
    int cycles = run_cycles(opcode_table[OP_BMI].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_table[OP_BMI].cycle_count);
    assert_int_equal(post_pc, pre_pc + 2);
}

void lbmi_basic_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBMI,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.n = 1;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBMI].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, USER_SPACE_ROOT + 4 + (short int) offset);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBMI].cycle_count + 1);
}

void lbmi_non_branch_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;
    int pre_pc = e_cpu_context.pc;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBMI,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.n = 0;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBMI].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, pre_pc + 4);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBMI].cycle_count);
}

void bne_basic_test(void **state) {
    (void) state; /* unused */
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BNE,
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

    e_cpu_context.cc.z = 0;
    int cycles = run_cycles(opcode_table[OP_BNE].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_table[OP_BNE].cycle_count);
    assert_int_equal(post_pc, USER_SPACE_ROOT + 2 + (char) offset);
}

void bne_non_branch_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BNE,
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

    e_cpu_context.cc.z = 1;
    int cycles = run_cycles(opcode_table[OP_BNE].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_table[OP_BNE].cycle_count);
    assert_int_equal(post_pc, pre_pc + 2);
}

void lbne_basic_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBNE,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.z = 0;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBNE].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, USER_SPACE_ROOT + 4 + (short int) offset);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBNE].cycle_count + 1);
}

void lbne_non_branch_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;
    int pre_pc = e_cpu_context.pc;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBNE,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.z = 1;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBNE].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, pre_pc + 4);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBNE].cycle_count);
}

void bpl_basic_test(void **state) {
    (void) state; /* unused */
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BPL,
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

    e_cpu_context.cc.n = 0;
    int cycles = run_cycles(opcode_table[OP_BPL].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_table[OP_BPL].cycle_count);
    assert_int_equal(post_pc, USER_SPACE_ROOT + 2 + (char) offset);
}

void bpl_non_branch_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BPL,
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

    e_cpu_context.cc.n = 1;
    int cycles = run_cycles(opcode_table[OP_BPL].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_table[OP_BPL].cycle_count);
    assert_int_equal(post_pc, pre_pc + 2);
}

void lbpl_basic_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBPL,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.n = 0;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBPL].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, USER_SPACE_ROOT + 4 + (short int) offset);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBPL].cycle_count + 1);
}

void lbpl_non_branch_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;
    int pre_pc = e_cpu_context.pc;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBPL,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.n = 1;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBPL].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, pre_pc + 4);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBPL].cycle_count);
}

void bra_basic_test(void **state) {
    (void) state; /* unused */
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BRA,
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

    int cycles = run_cycles(opcode_table[OP_BRA].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_table[OP_BRA].cycle_count);
    assert_int_equal(post_pc, USER_SPACE_ROOT + 2 + (char) offset);
}

void lbra_basic_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_LBRA,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 3 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    int cycles = run_cycles(opcode_ext_x10_table[OP_LBRA].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, USER_SPACE_ROOT + 3 + (short int) offset);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBRA].cycle_count);
}

void brn_basic_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BRN,
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

    e_cpu_context.cc.n = 1;
    int cycles = run_cycles(opcode_table[OP_BRN].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_table[OP_BRN].cycle_count);
    assert_int_equal(post_pc, pre_pc + 2);
}

void lbrn_basic_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;
    int pre_pc = e_cpu_context.pc;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBRN,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.n = 1;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBRN].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, pre_pc + 4);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBRN].cycle_count);
}

void bsr_basic_test(void **state) {
    (void) state; /* unused */
    uint8 offset = 0x40;
    uint16 pre_sp = e_cpu_context.s;
    uint16 pre_pc = e_cpu_context.pc;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BSR,
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

    int cycles = run_cycles(opcode_table[OP_BSR].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    uint16 post_sp = e_cpu_context.s;

    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_table[OP_BSR].cycle_count);
    assert_int_equal(post_pc, USER_SPACE_ROOT + 2 + (char) offset);
    /* The sp should have moved to receive the pc */
    assert_int_equal(post_sp, pre_sp - 2);
    /* The saved pc should be its value as of having decoded the two byte BSR
       instruction */
    assert_int_equal(read_word_from_memory(post_sp), pre_pc + 2);
}

void lbsr_basic_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;
    uint16 pre_sp = e_cpu_context.s;
    uint16 pre_pc = e_cpu_context.pc;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_LBSR,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 4 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 3 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.c = 0;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBSR].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    uint16 post_sp = e_cpu_context.s;

    assert_int_equal(post_pc, USER_SPACE_ROOT + 3 + (short int) offset);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBSR].cycle_count);
    /* The sp should have moved to receive the pc */
    assert_int_equal(post_sp, pre_sp - 2);
    /* The saved pc should be its value as of having decoded the two byte BSR
       instruction */
    assert_int_equal(read_word_from_memory(post_sp), pre_pc + 3);
}

void bvc_basic_test(void **state) {
    (void) state; /* unused */
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BVC,
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

    e_cpu_context.cc.v = 0;
    int cycles = run_cycles(opcode_table[OP_BVC].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_table[OP_BVC].cycle_count);
    assert_int_equal(post_pc, USER_SPACE_ROOT + 2 + (char) offset);
}

void bvc_non_branch_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BVC,
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

    e_cpu_context.cc.v = 1;
    int cycles = run_cycles(opcode_table[OP_BVC].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_table[OP_BVC].cycle_count);
    assert_int_equal(post_pc, pre_pc + 2);
}

void lbvc_basic_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBVC,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.v = 0;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBVC].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, USER_SPACE_ROOT + 4 + (short int) offset);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBVC].cycle_count + 1);
}

void lbvc_non_branch_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;
    int pre_pc = e_cpu_context.pc;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBVC,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.v = 1;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBVC].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, pre_pc + 4);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBVC].cycle_count);
}

void bvs_basic_test(void **state) {
    (void) state; /* unused */
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BVS,
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

    e_cpu_context.cc.v = 1;
    int cycles = run_cycles(opcode_table[OP_BVS].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_table[OP_BVS].cycle_count);
    assert_int_equal(post_pc, USER_SPACE_ROOT + 2 + (char) offset);
}

void bvs_non_branch_test(void **state) {
    (void) state; /* unused */
    int pre_pc = e_cpu_context.pc;
    uint8 offset = 0x40;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_BVS,
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

    e_cpu_context.cc.v = 0;
    int cycles = run_cycles(opcode_table[OP_BVS].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_table[OP_BVS].cycle_count);
    assert_int_equal(post_pc, pre_pc + 2);
}

void lbvs_basic_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBVS,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.v = 1;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBVS].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, USER_SPACE_ROOT + 4 + (short int) offset);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_NOP);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBVS].cycle_count + 1);
}

void lbvs_non_branch_test(void **state) {
    (void) state; /* unused */
    uint16 offset = 0x4000;
    int pre_pc = e_cpu_context.pc;

    /* Instruction is expected to be NOP if branch, CLRA otherwise.
       Handle appropriately based on test. */
    uint8 code_bytes[] = {
        OP_EXTENDED_X10,
        OP_LBVS,
        (uint8) (offset >> 8) & 0xFF, /* Signed 2's complement offset */
        (uint8) offset & 0xFF,
        OP_CLRA
    };
    uint8 code_bytes_branch[] = {
        OP_NOP
    };
    struct mem_loader_def test_memory[] = {
        { USER_SPACE_ROOT, code_bytes, 5 },
        /* Branch address is calculated by taking the baseline user space, +4
           for the calling branch opcode and long offset bytes, and then the
           offset, per the spec for branching operations. offset is cast to char
           to enable two's complement / signed offsets. */
        { USER_SPACE_ROOT + 4 + (short int) offset, code_bytes_branch, 1 }
    };
    load_memory(test_memory, 2);

    e_cpu_context.cc.v = 0;
    int cycles = run_cycles(opcode_ext_x10_table[OP_LBVS].cycle_count);
    uint16 post_pc = e_cpu_context.pc;
    assert_int_equal(post_pc, pre_pc + 4);
    assert_int_equal((uint16) read_byte_from_memory(post_pc), (uint16) OP_CLRA);
    assert_int_equal(cycles, opcode_ext_x10_table[OP_LBVS].cycle_count);
}
