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

/* Arithmetic Shift Right of 8-Bit Accumulator or Memory Byte */
int asr(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
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

    /* The Carry flag receives the value shifted out of bit 0. */
    e_cpu_context.cc.c = (reg_val & 0x1) > 0;
    reg_val >>= 1;
    /* The Negative flag is set equal to the value of bit 7. */
    e_cpu_context.cc.n = (reg_val & 0x80) > 0;
    /* The Zero flag is set if the new 8-bit value is zero; cleared
       otherwise. */
    e_cpu_context.cc.z = reg_val == 0;

    *p_reg = reg_val;
    return opcode_table[opcode].cycle_count;
}

/* Load Zero into Accumulator */
int clr(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) a_m; /* unused */

    e_cpu_context.pc++;

    switch (t_r) {
    case REG_A:
        e_cpu_context.d.byte_acc.a = 0;
        break;
    case REG_B:
        e_cpu_context.d.byte_acc.b = 0;
        break;
    default:
        assert(FALSE);
        return 0;
    }

    /* The Carry flag is cleared. */
    e_cpu_context.cc.c = 0;
    /* The Overflow flag is cleared. */
    e_cpu_context.cc.v = 0;
    /* The Zero flag is set. */
    e_cpu_context.cc.z = 1;
    /* The Negative flag is cleared. */
    e_cpu_context.cc.n = 0;

    return opcode_table[opcode].cycle_count;
}

/* Complement Accumulator */
int com(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
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
    /* one's complement */
    reg_val = ~reg_val;

    /* The Carry flag is always set. */
    e_cpu_context.cc.c = 1;
    /* The Overflow flag is cleared. */
    e_cpu_context.cc.v = 0;
    /* The Zero flag is set if the new value of the accumulator is zero;
       cleared otherwise. */
    e_cpu_context.cc.z = reg_val == 0;
    /* The Negative flag is set equal to the new value of the accumulators
       high-order bit. */
    e_cpu_context.cc.n = (reg_val & 0x80) > 0;

    *p_reg = reg_val;

    return opcode_table[opcode].cycle_count;
}

/* Decimal Addition Adjust */
int daa(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) t_r; /* unused */
    (void) a_m; /* unused */

    e_cpu_context.pc++;

    /* Previous BCD operation >= 100? */
    uint16 output = e_cpu_context.d.byte_acc.a;
    uint8 upper_nibble = (e_cpu_context.d.byte_acc.a & 0xF0) >> 4;
    uint8 lower_nibble = e_cpu_context.d.byte_acc.a & 0x0F;
    if (e_cpu_context.cc.c || upper_nibble > 9 ||
        (upper_nibble > 8 && lower_nibble > 9)) {
        output += 0x60;
    }
    if (e_cpu_context.cc.h || lower_nibble > 9) {
        output += 0x6;
    }

    e_cpu_context.d.byte_acc.a = (uint8) output & 0xFF;
    /* The Carry flag is set if the BCD addition produced a carry; cleared
       otherwise. */
    e_cpu_context.cc.c = (output & 0x100) > 0;
    /* The Negative flag is set equal to the new value of bit 7 in Accumulator
       A. */
    e_cpu_context.cc.n = (e_cpu_context.d.byte_acc.a & 0x80) > 0;
    /* The Zero flag is set if the new value of Accumulator A is zero; cleared
       otherwise. */
    e_cpu_context.cc.z = e_cpu_context.d.byte_acc.a == 0;

    return opcode_table[opcode].cycle_count;
}

/* Load Zero into Accumulator */
int dec(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
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
    /* The Overflow flag is set if the original value was 0x80 (8-bit) or 0x8000
       (16-bit); cleared otherwise. */
    e_cpu_context.cc.v = reg_val == 0x80;

    reg_val--;

    /* The Negative flag is set equal to the new value of the accumulators
       high-order bit. */
    e_cpu_context.cc.n = (reg_val & 0x80) > 0;
    /* The Zero flag is set if the new value of the accumulator is zero; cleared
       otherwise. */
    e_cpu_context.cc.z = (reg_val == 0);
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
