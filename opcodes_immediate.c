#include <assert.h>
#include <stdio.h>
#include "functions.h"

extern struct opcode_def opcode_table[];
extern struct cpu_state e_cpu_context;

/* Add Memory Byte plus Carry with Accumulator A or B */
int adc(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
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
    uint8 memory_val = read_byte_handler(a_m);
    uint16 total_val = reg_val + memory_val +
        (e_cpu_context.cc.c ? 1 : 0);
    uint8 output_val = total_val & 0xFF;

    /* The Carry flag is set if a carry out of bit 7 occurred; cleared
       otherwise. */
    e_cpu_context.cc.c = (total_val & 0x100) > 0 ? 1 : 0;
    /* The Half-Carry flag is set if a carry into bit 4 occurred; cleared
       otherwise. */
    e_cpu_context.cc.h = reg_val <= 0x0F && memory_val <= 0x0F &&
        (output_val & 0x10) > 0 ? 1 : 0;
    /* The Negative flag is set equal to the new value of bit 7 of
       the accumulator. */
    e_cpu_context.cc.n = (output_val & 0x80) > 1;
    /* The Zero flag is set if the new accumulator value is zero;
       cleared otherwise. */
    e_cpu_context.cc.z = output_val == 0;
    /* The Overflow flag is set if an overflow occurred; cleared otherwise. */
    /* If the sum of two positive numbers yields a negative result, the sum
       has overflowed. */
    uint8 pos_overflow = (reg_val & 0x80) == 0 && (memory_val & 0x80) == 0 &&
        e_cpu_context.cc.n;
    /* If the sum of two negative numbers yields a positive result, the sum
       has overflowed. */
    uint8 neg_overflow = (reg_val & 0x80) > 0 && (memory_val & 0x80) > 0 &&
        (output_val & 0x80) == 0;
    e_cpu_context.cc.v = pos_overflow || neg_overflow;

    *p_reg = output_val;
    return opcode_table[opcode].cycle_count;
}

/* Add Memory Byte to 8-Bit Accumulator */
int add(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
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
    uint8 memory_val = read_byte_handler(a_m);
    uint16 total_val = reg_val + memory_val;
    uint8 output_val = total_val & 0xFF;

    /* The Carry flag is set if a carry out of bit 7 occurred; cleared
       otherwise. */
    e_cpu_context.cc.c = (total_val & 0x100) > 0 ? 1 : 0;
    /* The Half-Carry flag is set if a carry into bit 4 occurred; cleared
       otherwise. */
    e_cpu_context.cc.h = reg_val <= 0x0F && memory_val <= 0x0F &&
        (output_val & 0x10) > 0 ? 1 : 0;
    /* The Negative flag is set equal to the new value of bit 7 of
       the accumulator. */
    e_cpu_context.cc.n = (output_val & 0x80) > 1;
    /* The Zero flag is set if the new accumulator value is zero;
       cleared otherwise. */
    e_cpu_context.cc.z = output_val == 0;
    /* The Overflow flag is set if an overflow occurred; cleared otherwise. */
    /* If the sum of two positive numbers yields a negative result, the sum
       has overflowed. */
    uint8 pos_overflow = (reg_val & 0x80) == 0 && (memory_val & 0x80) == 0 &&
        e_cpu_context.cc.n;
    /* If the sum of two negative numbers yields a positive result, the sum
       has overflowed. */
    uint8 neg_overflow = (reg_val & 0x80) > 0 && (memory_val & 0x80) > 0 &&
        (output_val & 0x80) == 0;
    e_cpu_context.cc.v = pos_overflow || neg_overflow;

    *p_reg = output_val;
    return opcode_table[opcode].cycle_count;
}

/* Add Memory Word to 16-Bit Accumulator */
int addd(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    e_cpu_context.pc++;

    uint16* p_reg = 0;
    switch (t_r) {
    case REG_D:
        p_reg = &e_cpu_context.d.d;
        break;
    default:
        assert(FALSE);
        return 0;
    }

    uint16 reg_val = *p_reg;
    uint16 memory_val = read_word_handler(a_m);
    uint32 total_val = reg_val + memory_val;
    uint16 output_val = total_val & 0xFFFF;

    /* The Carry flag is set if a carry out of bit 15 occurred; cleared
       otherwise. */
    e_cpu_context.cc.c = (total_val & 0x10000) > 0 ? 1 : 0;
    /* The Negative flag is set equal to the new value of bit 15 of
       the accumulator. */
    e_cpu_context.cc.n = (output_val & 0x8000) > 1;
    /* The Zero flag is set if the new accumulator value is zero;
       cleared otherwise. */
    e_cpu_context.cc.z = output_val == 0;
    /* The Overflow flag is set if an overflow occurred; cleared otherwise. */
    /* If the sum of two positive numbers yields a negative result, the sum
       has overflowed. */
    uint8 pos_overflow = (reg_val & 0x8000) == 0 &&
        (memory_val & 0x8000) == 0 &&
        e_cpu_context.cc.n;
    /* If the sum of two negative numbers yields a positive result, the sum
       has overflowed. */
    uint8 neg_overflow = (reg_val & 0x8000) > 0 &&
        (memory_val & 0x8000) > 0 &&
        (output_val & 0x80) == 0;
    e_cpu_context.cc.v = pos_overflow || neg_overflow;

    *p_reg = output_val;
    return opcode_table[opcode].cycle_count;
}

/* Logically AND Memory Byte with Accumulator A or B */
int and(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
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
    uint8 memory_val = read_byte_handler(a_m);

    reg_val &= memory_val;

    /* The Negative flag is set equal to the new value of bit 7 of
       the accumulator. */
    e_cpu_context.cc.n = (reg_val & 0x80) > 1;
    /* The Zero flag is set if the new accumulator value is zero;
       cleared otherwise. */
    e_cpu_context.cc.z = reg_val == 0;
    /* The Overflow flag is cleared by this instruction. */
    e_cpu_context.cc.v = 0;

    *p_reg = reg_val;
    return opcode_table[opcode].cycle_count;
}
