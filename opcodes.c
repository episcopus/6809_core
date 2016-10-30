#include <assert.h>
#include "functions.h"

extern struct opcode_def opcode_table[];
extern struct cpu_state e_cpu_context;

/* Add Accumulator B to Index Register X */
int abx(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) t_r; /* unused */
    (void) a_m; /* unused */

    e_cpu_context.pc++;
    e_cpu_context.x += e_cpu_context.d.byte_acc.b;
    return opcode_table[opcode].cycle_count;
}

/* Arithmetic Shift Left of 8-Bit Accumulator or Memory Byte */
int asl(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) a_m; /* unused */

    e_cpu_context.pc++;
    uint8* p_reg = 0;
    switch (t_r) {
    case REG_A:
        p_reg = &e_cpu_context.d.byte_acc.a;
        break;
    case REG_B:
        p_reg = &e_cpu_context.d.byte_acc.b;
        break;
    default:
        assert(FALSE);
        return 0;
    }

    uint8 reg_val = *p_reg;

    /*The Carry flag receives the value shifted out of bit 7. */
    e_cpu_context.cc.c = (reg_val & 0x80) > 0;
    /* The Overflow flag is set to the Exclusive-OR of the original
       values of bits 6 and 7. */
    e_cpu_context.cc.v = ((reg_val & 0x80) ^
        (reg_val & 0x40) << 1) > 0;
    reg_val <<= 1;
    /* The Negative flag is set equal to the new value of bit 7; previously
       bit 6. */
    e_cpu_context.cc.n = (reg_val & 0x80) > 0;
    /* The Zero flag is set if the new 8-bit value is zero; cleared
       otherwise. */
    e_cpu_context.cc.z = reg_val == 0;

    *p_reg = reg_val;
    return opcode_table[opcode].cycle_count;
}

/* No Operation */
int nop(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) t_r; /* unused */
    (void) a_m; /* unused */

    e_cpu_context.pc++;
    return opcode_table[opcode].cycle_count;
}

/* Not Implemented - used for NYI or invalid */
int notimpl(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) t_r; /* unused */
    (void) a_m; /* unused */

    /* Not yet implemented opcode */
    assert(FALSE);

    return opcode_table[opcode].cycle_count;
}
