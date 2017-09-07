#include <assert.h>
#include <stdio.h>

#include "opcodes.h"

extern struct opcode_def opcode_table[];
extern struct opcode_def opcode_ext_x10_table[];
extern struct opcode_def opcode_ext_x11_table[];
extern struct cpu_state e_cpu_context;

/* This struct captures the inverse order stacking order for psh operations */
const struct stack_op_postbyte_entry stack_op_pb_entry_table[] = {
    { REG_CC, 0 },
    { REG_A, 1 },
    { REG_B, 2 },
    { REG_DP, 3 },
    { REG_X, 4 },
    { REG_Y, 5 },
    { REG_S, 6 },
    { REG_PC, 7 }
};

/* Add Accumulator B to Index Register X */
int abx(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) t_r; /* unused */
    (void) a_m; /* unused */

    e_cpu_context.pc++;
    e_cpu_context.x += e_cpu_context.d.byte_acc.b;
    return opcode_table[opcode].cycle_count;
}

/* Add Memory Byte plus Carry with Accumulator A or B */
int adc(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    e_cpu_context.pc++;

    uint8 extra_cycles = 0;
    uint8 reg_val = get_reg_value_8(t_r);
    uint8 memory_val = read_byte_handler(a_m, &extra_cycles);
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

    set_reg_value_8(t_r, output_val);
    return opcode_table[opcode].cycle_count + extra_cycles;
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
    uint8 extra_cycles = 0;
    uint8 memory_val = read_byte_handler(a_m, &extra_cycles);
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
    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Add Memory Word to 16-Bit Accumulator */
int addd(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    e_cpu_context.pc++;

    uint8 extra_cycles = 0;
    uint16 reg_val = get_reg_value_16(t_r);
    uint16 memory_val = read_word_handler(a_m, &extra_cycles);
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

    set_reg_value_16(t_r, output_val);
    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Logically AND Memory Byte with Accumulator A or B */
int and(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    e_cpu_context.pc++;

    uint8 reg_val = get_reg_value_8(t_r);
    uint8 extra_cycles = 0;
    uint8 memory_val = read_byte_handler(a_m, &extra_cycles);

    reg_val &= memory_val;

    /* The Negative flag is set equal to the new value of bit 7 of
       the accumulator. */
    e_cpu_context.cc.n = (reg_val & 0x80) > 1;
    /* The Zero flag is set if the new accumulator value is zero;
       cleared otherwise. */
    e_cpu_context.cc.z = reg_val == 0;
    /* The Overflow flag is cleared by this instruction. */
    e_cpu_context.cc.v = 0;

    set_reg_value_8(t_r, reg_val);
    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Logically AND Immediate Value with the CC Register */
int andcc(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) t_r; /* unused */

    e_cpu_context.pc++;
    uint8 reg_val = (uint8) *((uint8*) &e_cpu_context.cc);
    uint8 extra_cycles = 0;
    uint8 memory_val = read_byte_handler(a_m, &extra_cycles);

    reg_val &= memory_val;
    *((uint8*) &e_cpu_context.cc) = reg_val;

    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Arithmetic Shift Left of 8-Bit Accumulator or Memory Byte */
int asl(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    e_cpu_context.pc++;

    uint16 out_addr = 0;
    uint8 reg_val = 0;
    uint8 extra_cycles = 0;
    if (a_m == INHERENT) {
        reg_val = get_reg_value_8(t_r);
    }
    else {
        out_addr = get_memory_address_from_postbyte(a_m, &extra_cycles);
        reg_val = read_byte_from_memory(out_addr);
    }

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

    if (a_m == INHERENT) {
        set_reg_value_8(t_r, reg_val);
    }
    else {
        write_byte_to_memory(out_addr, reg_val);
    }

    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Arithmetic Shift Right of 8-Bit Accumulator or Memory Byte */
int asr(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) a_m; /* unused */
    e_cpu_context.pc++;

    uint16 out_addr = 0;
    uint8 reg_val = 0;
    uint8 extra_cycles = 0;
    if (a_m == INHERENT) {
        reg_val = get_reg_value_8(t_r);
    }
    else {
        out_addr = get_memory_address_from_postbyte(a_m, &extra_cycles);
        reg_val = read_byte_from_memory(out_addr);
    }

    /* The Carry flag receives the value shifted out of bit 0. */
    e_cpu_context.cc.c = (reg_val & 0x1) > 0;
    /* Stash the sign bit away for the operation */
    uint8 sign = reg_val & 0x80;
    reg_val &= 0x7F;

    reg_val >>= 1;

    /* Restore any sign */
    reg_val |= sign;

    /* The Negative flag is set equal to the value of bit 7. */
    e_cpu_context.cc.n = (reg_val & 0x80) > 0;
    /* The Zero flag is set if the new 8-bit value is zero; cleared
       otherwise. */
    e_cpu_context.cc.z = reg_val == 0;

    if (a_m == INHERENT) {
        set_reg_value_8(t_r, reg_val);
    }
    else {
        write_byte_to_memory(out_addr, reg_val);
    }

    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Bit Test Accumulator A or B with Memory Byte Value */
int bit(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    e_cpu_context.pc++;

    uint8 reg_val = get_reg_value_8(t_r);
    uint8 extra_cycles = 0;
    uint8 memory_val = read_byte_handler(a_m, &extra_cycles);

    reg_val &= memory_val;

    /* The Negative flag is set equal to the new value of bit 7 of
       the accumulator. */
    e_cpu_context.cc.n = (reg_val & 0x80) > 1;
    /* The Zero flag is set if the new accumulator value is zero;
       cleared otherwise. */
    e_cpu_context.cc.z = reg_val == 0;
    /* The Overflow flag is cleared by this instruction. */
    e_cpu_context.cc.v = 0;

    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Handler for all branching opcodes */
int branch(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    e_cpu_context.pc++;
    uint8 extra_cycles = 0;
    short int offset = 0;

    switch (opcode) {
    case OP_BCC:
        offset = (char) read_byte_from_memory(e_cpu_context.pc++);
        if (e_cpu_context.cc.c) {
            /* Reverse logic here to make it always read the postbyte (offset)
               and advance the PC, nullifying the branch in the negative
               logic case. */
            offset = 0;
        }
        break;
    }

    e_cpu_context.pc += offset;
    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Handler for all long branching opcodes */
int branch16(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    e_cpu_context.pc++;
    uint8 extra_cycles = 0;
    short int offset = 0;
    /* Need to reference the correct opcode table for the cycle lookup at the
       end of the function. Almost all branch operations are on the 0x10 table. */
    struct opcode_def* this_opcode_table = opcode_ext_x10_table;

    switch (opcode) {
    case OP_LBCC:
        offset = (short int) read_word_from_memory(e_cpu_context.pc);
        e_cpu_context.pc += 2;
        if (e_cpu_context.cc.c) {
            offset = 0;
        }
        else {
            /* The 6809 requires an extra cycle if the branch is taken. */
            extra_cycles++;
        }
        break;
    }

    e_cpu_context.pc += offset;
    return this_opcode_table[opcode].cycle_count + extra_cycles;
}

/* Load Zero into Accumulator */
int clr(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) a_m; /* unused */
    e_cpu_context.pc++;

    uint8 extra_cycles = 0;
    uint16 out_addr = 0;
    if (a_m == INHERENT) {
        set_reg_value_8(t_r, 0);
    }
    else {
        out_addr = get_memory_address_from_postbyte(a_m, &extra_cycles);
        write_byte_to_memory(out_addr, 0);
    }

    /* The Carry flag is cleared. */
    e_cpu_context.cc.c = 0;
    /* The Overflow flag is cleared. */
    e_cpu_context.cc.v = 0;
    /* The Zero flag is set. */
    e_cpu_context.cc.z = 1;
    /* The Negative flag is cleared. */
    e_cpu_context.cc.n = 0;

    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Complement Accumulator */
int com(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) a_m; /* unused */
    e_cpu_context.pc++;

    uint8 extra_cycles = 0;
    uint16 out_addr = 0;
    uint8 reg_val = 0;
    if (a_m == INHERENT) {
        reg_val = get_reg_value_8(t_r);
    }
    else {
        out_addr = get_memory_address_from_postbyte(a_m, &extra_cycles);
        reg_val = read_byte_from_memory(out_addr);
    }

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

    if (a_m == INHERENT) {
        set_reg_value_8(t_r, reg_val);
    }
    else {
        write_byte_to_memory(out_addr, reg_val);
    }

    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Subtract from value in 8-Bit Accumulator */
int cmp(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
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
    uint8 extra_cycles = 0;
    uint8 memory_val = read_byte_handler(a_m, &extra_cycles);

    uint8 output_val = reg_val - memory_val;

    /* The Negative flag is set equal to the value of bit 7 of the result. */
    e_cpu_context.cc.n = (output_val & 0x80) > 1;
    /* The Zero flag is set if the resulting value is zero; cleared
       otherwise. */
    e_cpu_context.cc.z = output_val == 0;
    /* The Carry flag is set if a borrow into bit 7 was needed; cleared
       otherwise. */
    e_cpu_context.cc.c = reg_val < memory_val;
    /* The Overflow flag is set if an overflow occurred; cleared otherwise. */
    /* If the sum of two positive numbers yields a negative result, the sum
       has overflowed. */
    uint8 pos_overflow = (reg_val & 0x80) == 0 &&
        (memory_val & 0x80) == 0 &&
        e_cpu_context.cc.n;
    /* If the sum of two negative numbers yields a positive result, the sum
       has overflowed. */
    uint8 neg_overflow = (reg_val & 0x80) > 0 &&
        (memory_val & 0x80) > 0 &&
        (output_val & 0x80) == 0;
    e_cpu_context.cc.v = pos_overflow || neg_overflow;

    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Compare Memory Word from 16-Bit Register */
int cmp16(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    e_cpu_context.pc++;

    uint16* p_reg = 0;
    /* Need to reference the correct opcode table for the cycle lookup at the
       end of the function. */
    struct opcode_def* this_opcode_table = NULL;
    switch (t_r) {
    case REG_D:
        p_reg = &e_cpu_context.d.d;
        this_opcode_table = opcode_ext_x10_table;
        break;
    case REG_S:
        p_reg = &e_cpu_context.s;
        this_opcode_table = opcode_ext_x11_table;
        break;
    case REG_U:
        p_reg = &e_cpu_context.u;
        this_opcode_table = opcode_ext_x11_table;
        break;
    case REG_X:
        p_reg = &e_cpu_context.x;
        this_opcode_table = opcode_table;
        break;
    case REG_Y:
        p_reg = &e_cpu_context.y;
        this_opcode_table = opcode_ext_x10_table;
        break;
    default:
        assert(FALSE);
        return 0;
    }

    uint16 reg_val = *p_reg;
    uint8 extra_cycles = 0;
    uint16 memory_val = read_word_handler(a_m, &extra_cycles);

    uint16 output_val = reg_val - memory_val;

    /* The Negative flag is set equal to the value of bit 7 of the result. */
    e_cpu_context.cc.n = (output_val & 0x8000) > 1;
    /* The Zero flag is set if the resulting value is zero; cleared
       otherwise. */
    e_cpu_context.cc.z = output_val == 0;
    /* The Carry flag is set if a borrow into bit 7 was needed; cleared
       otherwise. */
    e_cpu_context.cc.c = reg_val < memory_val;
    /* The Overflow flag is set if an overflow occurred; cleared otherwise. */
    /* If the sum of two positive numbers yields a negative result, the sum
       has overflowed. */
    uint16 pos_overflow = (reg_val & 0x8000) == 0 &&
        (memory_val & 0x8000) == 0 &&
        e_cpu_context.cc.n;
    /* If the sum of two negative numbers yields a positive result, the sum
       has overflowed. */
    uint16 neg_overflow = (reg_val & 0x8000) > 0 &&
        (memory_val & 0x8000) > 0 &&
        (output_val & 0x8000) == 0;
    e_cpu_context.cc.v = pos_overflow || neg_overflow;

    return this_opcode_table[opcode].cycle_count + extra_cycles;
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

/* Decrement Accumulator */
int dec(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) a_m; /* unused */
    e_cpu_context.pc++;

    uint16 out_addr = 0;
    uint8 reg_val = 0;
    uint8 extra_cycles = 0;
    if (a_m == INHERENT) {
        reg_val = get_reg_value_8(t_r);
    }
    else {
        out_addr = get_memory_address_from_postbyte(a_m, &extra_cycles);
        reg_val = read_byte_from_memory(out_addr);
    }

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

    if (a_m == INHERENT) {
        set_reg_value_8(t_r, reg_val);
    }
    else {
        write_byte_to_memory(out_addr, reg_val);
    }

    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Exclusively-OR Memory Byte with Accumulator A or B */
int eor(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
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
    uint8 extra_cycles = 0;
    uint8 memory_val = read_byte_handler(a_m, &extra_cycles);

    reg_val ^= memory_val;

    /* The Negative flag is set equal to the new value of bit 7 of
       the accumulator. */
    e_cpu_context.cc.n = (reg_val & 0x80) > 1;
    /* The Zero flag is set if the new accumulator value is zero;
       cleared otherwise. */
    e_cpu_context.cc.z = reg_val == 0;
    /* The Overflow flag is cleared by this instruction. */
    e_cpu_context.cc.v = 0;

    *p_reg = reg_val;
    return opcode_table[opcode].cycle_count + extra_cycles;
}


/* Exchange Registers */
int exg(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) t_r; /* unused */
    (void) a_m; /* unused */
    e_cpu_context.pc++;

    uint8 extra_cycles = 0;
    uint8 post_byte = read_byte_handler(IMMEDIATE, &extra_cycles);
    enum target_register src;
    enum target_register trg;
    decode_source_target_postbyte(post_byte, &src, &trg);

    enum reg_size src_size = get_reg_size(src);
    enum reg_size trg_size = get_reg_size(trg);

    /* Tolerate one invalid register, not both */
    if (src_size == REG_SIZE_INVALID && trg_size != REG_SIZE_INVALID) {
        src_size = trg_size;
    }
    else if (trg_size == REG_SIZE_INVALID && src_size != REG_SIZE_INVALID) {
        trg_size = src_size;
    }
    else if (trg_size == REG_SIZE_INVALID && src_size == REG_SIZE_INVALID) {
        /* Do nothing when we get trolled */
        return opcode_table[opcode].cycle_count + extra_cycles;
    }

    /* If an invalid register encoding is specified for either register, a
       constant value of FF or FFFF is used for the exchange.  */
    if (src_size == trg_size && src_size == REG_SIZE_8) {
        uint8 src_value = src == REG_NONE ? 0xFF : get_reg_value_8(src);
        uint8 trg_value = trg == REG_NONE ? 0xFF : get_reg_value_8(trg);
        if (src != REG_NONE) {
            set_reg_value_8(src, trg_value);
        }
        if (trg != REG_NONE) {
            set_reg_value_8(trg, src_value);
        }
    }
    else if (src_size == trg_size && src_size == REG_SIZE_16) {
        uint16 src_value = src == REG_NONE ? 0xFFFF : get_reg_value_16(src);
        uint16 trg_value = trg == REG_NONE ? 0xFFFF : get_reg_value_16(trg);
        if (src != REG_NONE) {
            set_reg_value_16(src, trg_value);
        }
        if (trg != REG_NONE) {
            set_reg_value_16(trg, src_value);
        }
    }
    else if (src_size == REG_SIZE_16) {
        uint16 src_value = get_reg_value_16(src);
        uint8 trg_value = get_reg_value_8(trg);
        uint16 new_src_value = 0xFF00 | trg_value;
        uint8 new_trg_value = (uint8) (src_value & 0xFF);
        set_reg_value_16(src, new_src_value);
        set_reg_value_8(trg, new_trg_value);
    }
    else { /* src_size == REG_SIZE_8 && trg_size == REG_SIZE_16 */
        /* The 8-bit register is always exchanged with the lower half of the
           16-bit register, and the the upper half of the 16-bit register is
           then set to the value shown in the table below. */
        uint8 new_src_value = 0;
        uint16 new_trg_value = 0;
        switch (src) {
        case REG_A:
            if (trg == REG_D) {
                /* The one exception is for EXG A,D which produces exactly the
                   same result as EXG A,B */
                uint8 temp = get_reg_value_8(src);
                set_reg_value_8(src, get_reg_value_8(REG_B));
                set_reg_value_8(REG_B, temp);
                break;
            }
        case REG_B:
            new_trg_value = 0xFF00 | get_reg_value_8(src);
            new_src_value = (uint8) (get_reg_value_16(trg) & 0xFF);
            break;
        case REG_CC:
        case REG_DP:
            new_trg_value = (get_reg_value_8(src) << 8) | get_reg_value_8(src);
            new_src_value = (uint8) (get_reg_value_16(trg) & 0xFF);
            break;
        default:
            assert(FALSE);
            break;
        }

        if (!(src == REG_A && trg == REG_D)) {
            /* This special case was handled above */
            set_reg_value_8(src, new_src_value);
            set_reg_value_16(trg, new_trg_value);
        }
    }

    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Increment Accumulator */
int inc(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) a_m; /* unused */
    e_cpu_context.pc++;

    uint16 out_addr = 0;
    uint8 reg_val = 0;
    uint8 extra_cycles = 0;
    if (a_m == INHERENT) {
        reg_val = get_reg_value_8(t_r);
    }
    else {
        out_addr = get_memory_address_from_postbyte(a_m, &extra_cycles);
        reg_val = read_byte_from_memory(out_addr);
    }

    /* The Overflow flag is set if the original value was $7F (8-bit) or $7FFF
       (16-bit); cleared otherwise. */
    e_cpu_context.cc.v = reg_val == 0x7F;

    reg_val++;

    /* The Negative flag is set equal to the new value of the accumulators
       high-order bit. */
    e_cpu_context.cc.n = (reg_val & 0x80) > 0;
    /* The Zero flag is set if the new value of the accumulator is zero; cleared
       otherwise. */
    e_cpu_context.cc.z = (reg_val == 0);

    if (a_m == INHERENT) {
        set_reg_value_8(t_r, reg_val);
    }
    else {
        write_byte_to_memory(out_addr, reg_val);
    }
    return opcode_table[opcode].cycle_count + extra_cycles;
}


/* Unconditional Jump */
int jmp(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    e_cpu_context.pc++;

    uint8 extra_cycles = 0;
    uint16 out_addr = get_memory_address_from_postbyte(a_m, &extra_cycles);
    e_cpu_context.pc = out_addr;

    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Unconditional Jump to Subroutine */
int jsr(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    e_cpu_context.pc++;

    uint8 extra_cycles = 0;
    uint16 out_addr = get_memory_address_from_postbyte(a_m, &extra_cycles);

    /* Save current PC onto hardware stack */
    e_cpu_context.s -= 2;
    write_word_to_memory(e_cpu_context.s, e_cpu_context.pc);

    e_cpu_context.pc = out_addr;

    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Load Data into 8-Bit Accumulator */
int ld(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    e_cpu_context.pc++;

    uint8 extra_cycles = 0;
    uint8 memory_val = read_byte_handler(a_m, &extra_cycles);
    set_reg_value_8(t_r, memory_val);

    /* The Negative flag is set equal to the new value of bit 7 of
       the accumulator. */
    e_cpu_context.cc.n = (memory_val & 0x80) > 1;
    /* The Zero flag is set if the new accumulator value is zero;
       cleared otherwise. */
    e_cpu_context.cc.z = memory_val == 0;
    /* The Overflow flag is cleared by this instruction. */
    e_cpu_context.cc.v = 0;

    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Load Data into 16-Bit Register */
int ld16(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    e_cpu_context.pc++;

    /* Need to reference the correct opcode table for the cycle lookup at the
       end of the function. */
    struct opcode_def* this_opcode_table = NULL;
    switch (t_r) {
    case REG_S:
        this_opcode_table = opcode_ext_x10_table;
        break;
    case REG_Y:
        this_opcode_table = opcode_ext_x10_table;
        break;
    default:
        this_opcode_table = opcode_table;
        break;
    }

    uint8 extra_cycles = 0;
    uint16 memory_val = read_word_handler(a_m, &extra_cycles);
    set_reg_value_16(t_r, memory_val);

    /* The Negative flag is set equal to the new value of bit 15 of
       the accumulator. */
    e_cpu_context.cc.n = (memory_val & 0x8000) > 1;
    /* The Zero flag is set if the new accumulator value is zero;
       cleared otherwise. */
    e_cpu_context.cc.z = memory_val == 0;
    /* The Overflow flag is cleared by this instruction. */
    e_cpu_context.cc.v = 0;

    return this_opcode_table[opcode].cycle_count + extra_cycles;
}

/* Load Effective Address */
int lea(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    e_cpu_context.pc++;

    uint8 extra_cycles = 0;
    uint16 out_addr = get_memory_address_from_postbyte(a_m, &extra_cycles);
    set_reg_value_16(t_r, out_addr);
    if (t_r == REG_X || t_r == REG_Y) {
        e_cpu_context.cc.z = !out_addr;
    }

    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Logical Shift Right of 8-Bit Accumulator or Memory Byte */
int lsr(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) a_m; /* unused */

    e_cpu_context.pc++;
    uint8 reg_val = 0;
    uint8 extra_cycles = 0;
    uint16 out_addr = 0;

    if (a_m == INHERENT) {
        reg_val = get_reg_value_8(t_r);
    }
    else {
        out_addr = get_memory_address_from_postbyte(a_m, &extra_cycles);
        reg_val = read_byte_from_memory(out_addr);
    }

    /* The Carry flag receives the value shifted out of bit 0. */
    e_cpu_context.cc.c = (reg_val & 0x1) > 0;
    reg_val >>= 1;
    /* The Negative flag is set equal to the value of bit 7. */
    e_cpu_context.cc.n = (reg_val & 0x80) > 0;
    /* The Zero flag is set if the new 8-bit value is zero; cleared
       otherwise. */
    e_cpu_context.cc.z = reg_val == 0;

    if (a_m == INHERENT) {
        set_reg_value_8(t_r, reg_val);
    }
    else {
        write_byte_to_memory(out_addr, reg_val);
    }

    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Unsigned Multiply of Accumulator A and Accumulator B */
int mul(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) t_r; /* unused */
    (void) a_m; /* unused */

    e_cpu_context.pc++;

    e_cpu_context.d.d = e_cpu_context.d.byte_acc.a * e_cpu_context.d.byte_acc.b;

    /*The Carry flag is set equal to the new value of bit 7 in Accumulator B. */
    e_cpu_context.cc.c = (e_cpu_context.d.byte_acc.b & 0x80) > 0;
    /*The Zero flag is set if the 16-bit result is zero; cleared otherwise. */
    e_cpu_context.cc.z = e_cpu_context.d.d == 0;

    return opcode_table[opcode].cycle_count;
}

/* Negation (Twos-Complement) of Accumulator */
int neg(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) a_m; /* unused */
    e_cpu_context.pc++;

    uint16 out_addr = 0;
    uint8 reg_val = 0;
    uint8 extra_cycles = 0;
    if (a_m == INHERENT) {
        reg_val = get_reg_value_8(t_r);
    }
    else {
        out_addr = get_memory_address_from_postbyte(a_m, &extra_cycles);
        reg_val = read_byte_from_memory(out_addr);
    }

    /* The Carry flag represents a Borrow for this operation and is therefore
       always set unless the accumulator’s original value was zero. */
    e_cpu_context.cc.c = reg_val != 0;

    if (reg_val == 0x80) {
        /* If the original value of the accumulator is 0x80 (0x8000 for NEGD)
           then the Overflow flag (V) is set and the accumulator’s value is not
           modified. */
        e_cpu_context.cc.v = 1;
    }
    else {
        /* two's complement */
        reg_val = 0 - reg_val;
        e_cpu_context.cc.v = 0;
    }

    /* The Zero flag is set if the new value of the accumulator is zero;
       cleared otherwise. */
    e_cpu_context.cc.z = reg_val == 0;
    /* The Negative flag is set equal to the new value of the accumulators
       high-order bit. */
    e_cpu_context.cc.n = (reg_val & 0x80) > 0;

    if (a_m == INHERENT) {
        set_reg_value_8(t_r, reg_val);
    }
    else {
        write_byte_to_memory(out_addr, reg_val);
    }

    return opcode_table[opcode].cycle_count + extra_cycles;
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

/* Logically OR Accumulator with a Byte from Memory */
int or(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    e_cpu_context.pc++;

    uint8 extra_cycles = 0;
    uint8 reg_val = get_reg_value_8(t_r);
    uint8 memory_val = read_byte_handler(a_m, &extra_cycles);

    reg_val |= memory_val;

    /* The Negative flag is set equal to the new value of bit 7 of
       the accumulator. */
    e_cpu_context.cc.n = (reg_val & 0x80) > 1;
    /* The Zero flag is set if the new accumulator value is zero;
       cleared otherwise. */
    e_cpu_context.cc.z = reg_val == 0;
    /* The Overflow flag is cleared by this instruction. */
    e_cpu_context.cc.v = 0;

    set_reg_value_8(t_r, reg_val);
    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Logically OR the CC Register with an Immediate Value */
int orcc(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    e_cpu_context.pc++;

    uint8 extra_cycles = 0;
    uint8 reg_val = get_reg_value_8(t_r);
    uint8 memory_val = read_byte_handler(a_m, &extra_cycles);

    reg_val |= memory_val;

    set_reg_value_8(t_r, reg_val);
    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Push Registers onto a Stack */
int psh(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    e_cpu_context.pc++;

    uint8 extra_cycles = 0;
    /* postbyte indicating which registers to push */
    uint8 postbyte = read_byte_handler(a_m, &extra_cycles);
    /* baseline figure for clock cycles, each register pushed is one
       additional cycle */
    uint8 cycles = opcode_table[opcode].cycle_count;
    /* baseline stack pointer, decremented for each push, works for
       both u and s pointer */
    uint16 stack_pointer = get_reg_value_16(t_r);

    /* number of possible registers to push - it's really 8 but
       the REG_S value is special and needs to toggle since can't push own
       stack pointer */
    int sizeof_table = sizeof(stack_op_pb_entry_table) /
        sizeof(struct stack_op_postbyte_entry);
    /* push all necessary registers as requested in the postbyte according
       to the 6809 spec. */
    for (int i = sizeof_table - 1; i >= 0; i--) {
        if (postbyte & (1 << i)) {
            enum target_register this_t_r = stack_op_pb_entry_table[i].reg;

            if (get_reg_size(this_t_r) == REG_SIZE_16) {
                /* REG_S in this table is a special value that flips to REG_U in
                   the case where the function is called with REG_S target
                   register. This is because the psh instruction cannot push
                   its own stack pointer */
                this_t_r == REG_S && t_r == REG_S ? this_t_r = REG_U : REG_S;
                uint16 val = get_reg_value_16(this_t_r);
                uint8 lower_byte = val;
                uint8 upper_byte = val >> 8;

                write_byte_to_memory(--stack_pointer, lower_byte);
                write_byte_to_memory(--stack_pointer, upper_byte);
                cycles += 2;
            }
            else if (get_reg_size(this_t_r) == REG_SIZE_8) {
                uint8 val = get_reg_value_8(this_t_r);

                write_byte_to_memory(--stack_pointer, val);
                cycles++;
            }
        }
    }

    set_reg_value_16(t_r, stack_pointer);
    return cycles + extra_cycles;
}

/* Pull Registers from Stack */
int pul(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    e_cpu_context.pc++;

    uint8 extra_cycles = 0;
    /* postbyte indicating which registers to push */
    uint8 postbyte = read_byte_handler(a_m, &extra_cycles);
    /* baseline figure for clock cycles, each register pushed is one
       additional cycle */
    uint8 cycles = opcode_table[opcode].cycle_count;
    /* baseline stack pointer, incremented for each pull, works for
       both u and s pointer */
    uint16 stack_pointer = get_reg_value_16(t_r);

    /* number of possible registers to pull - it's really 8 but
       the REG_S value is special and needs to toggle since can't pull own
       stack pointer */
    int sizeof_table = sizeof(stack_op_pb_entry_table) /
        sizeof(struct stack_op_postbyte_entry);
    /* pull all necessary registers as requested in the postbyte according
       to the 6809 spec. */
    for (int i = 0; i < sizeof_table; i++) {
        if (postbyte & (1 << i)) {
            enum target_register this_t_r = stack_op_pb_entry_table[i].reg;

            if (get_reg_size(this_t_r) == REG_SIZE_16) {
                /* REG_S in this table is a special value that flips to REG_U in
                   the case where the function is called with REG_S target
                   register. This is because the pul instruction cannot pull
                   its own stack pointer */
                this_t_r == REG_S && t_r == REG_S ? this_t_r = REG_U : REG_S;

                uint16 this_val = read_word_from_memory(stack_pointer);
                stack_pointer += 2;
                set_reg_value_16(this_t_r, this_val);

                cycles += 2;
            }
            else if (get_reg_size(this_t_r) == REG_SIZE_8) {
                uint8 this_val = read_byte_from_memory(stack_pointer++);
                set_reg_value_8(this_t_r, this_val);

                cycles++;
            }
        }
    }

    set_reg_value_16(t_r, stack_pointer);
    return cycles + extra_cycles;
}

/* Rotate 8-Bit Accumulator or Memory Byte Left through Carry */
int rol(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) a_m; /* unused */
    e_cpu_context.pc++;

    uint16 out_addr = 0;
    uint8 reg_val = 0;
    uint8 extra_cycles = 0;
    if (a_m == INHERENT) {
        reg_val = get_reg_value_8(t_r);
    }
    else {
        out_addr = get_memory_address_from_postbyte(a_m, &extra_cycles);
        reg_val = read_byte_from_memory(out_addr);
    }

    uint8 old_carry = e_cpu_context.cc.c;
    /*The Carry flag receives the value shifted out of bit 7. */
    e_cpu_context.cc.c = (reg_val & 0x80) > 0;
    /* The Overflow flag is set to the Exclusive-OR of the original
       values of bits 6 and 7. */
    e_cpu_context.cc.v = ((reg_val & 0x80) ^
        (reg_val & 0x40) << 1) > 0;
    reg_val <<= 1;
    /* Rotate old carry flag to bit 0. */
    reg_val |= old_carry;
    /* The Negative flag is set equal to the new value of bit 7; previously
       bit 6. */
    e_cpu_context.cc.n = (reg_val & 0x80) > 0;
    /* The Zero flag is set if the new 8-bit value is zero; cleared
       otherwise. */
    e_cpu_context.cc.z = reg_val == 0;

    if (a_m == INHERENT) {
        set_reg_value_8(t_r, reg_val);
    }
    else {
        write_byte_to_memory(out_addr, reg_val);
    }
    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Rotate 8-Bit Accumulator or Memory Byte Right through Carry */
int ror(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) a_m; /* unused */
    e_cpu_context.pc++;

    uint16 out_addr = 0;
    uint8 reg_val = 0;
    uint8 extra_cycles = 0;
    if (a_m == INHERENT) {
        reg_val = get_reg_value_8(t_r);
    }
    else {
        out_addr = get_memory_address_from_postbyte(a_m, &extra_cycles);
        reg_val = read_byte_from_memory(out_addr);
    }

    uint8 old_carry = e_cpu_context.cc.c;
    /*The Carry flag receives the value shifted out of bit 0. */
    e_cpu_context.cc.c = (reg_val & 0x1);
    reg_val >>= 1;
    /* Rotate old carry flag to bit 7. */
    reg_val |= (old_carry ? 0x80 : 0);
    /* The Negative flag is set equal to the new value of bit 7; previously
       bit 6. */
    e_cpu_context.cc.n = (reg_val & 0x80) > 0;
    /* The Zero flag is set if the new 8-bit value is zero; cleared
       otherwise. */
    e_cpu_context.cc.z = reg_val == 0;

    if (a_m == INHERENT) {
        set_reg_value_8(t_r, reg_val);
    }
    else {
        write_byte_to_memory(out_addr, reg_val);
    }
    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Subtract Memory Byte and Carry from Accumulator A or B */
int sbc(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    e_cpu_context.pc++;

    uint8 extra_cycles = 0;
    uint8 reg_val = get_reg_value_8(t_r);
    uint8 memory_val = read_byte_handler(a_m, &extra_cycles);
    uint8 total_val = reg_val - memory_val -
        (e_cpu_context.cc.c ? 1 : 0);

    /* The Carry flag is set if a borrow into bit-7 was needed; cleared
       otherwise. */
    e_cpu_context.cc.c = memory_val + (e_cpu_context.cc.c ? 1 : 0) >
        reg_val;
    /* The Negative flag is set equal to the new value of bit 7 of
       the accumulator. */
    e_cpu_context.cc.n = (total_val & 0x80) > 1;
    /* The Zero flag is set if the new accumulator value is zero;
       cleared otherwise. */
    e_cpu_context.cc.z = total_val == 0;
    /* The Overflow flag is set if an overflow occurred; cleared otherwise. */
    /* If the difference of a positive and a negative number yields a negative
       result, the difference has overflowed. */
    uint8 pos_overflow = (reg_val & 0x80) == 0 && (memory_val & 0x80) > 0 &&
        e_cpu_context.cc.n;
    /* If the difference of a negative and a positive number yields a negative
       result, the difference has overflowed. */
    uint8 neg_overflow = (reg_val & 0x80) > 0 && (memory_val & 0x80) == 0 &&
        (total_val & 0x80) == 0;
    e_cpu_context.cc.v = pos_overflow || neg_overflow;

    set_reg_value_8(t_r, total_val);
    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Sign Extend the 8-bit Value in B to a 16-bit Value in D */
int sex(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) a_m; /* unused */
    (void) t_r; /* unused */

    e_cpu_context.pc++;
    e_cpu_context.cc.n = (e_cpu_context.d.byte_acc.b & 0x80) > 0;
    e_cpu_context.d.byte_acc.a = e_cpu_context.cc.n ? 0xFF : 0x0;

    /* The Zero flag is set if the new value of Accumulator D is zero
       (B was zero); cleared otherwise. */
    e_cpu_context.cc.z = e_cpu_context.d.d == 0;

    return opcode_table[opcode].cycle_count;
}

/* Store 8-Bit Accumulator to Memory */
int st(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    e_cpu_context.pc++;

    uint8 extra_cycles = 0;
    uint8 store_byte = get_reg_value_8(t_r);
    write_byte_handler(a_m, store_byte, &extra_cycles);

    /* The Negative flag is set equal to the new value of bit 7 of
       the accumulator. */
    e_cpu_context.cc.n = (store_byte & 0x80) > 1;
    /* The Zero flag is set if the new accumulator value is zero;
       cleared otherwise. */
    e_cpu_context.cc.z = store_byte == 0;
    /* The Overflow flag is cleared by this instruction. */
    e_cpu_context.cc.v = 0;

    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Store 16-Bit Register to Memory */
int st16(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    e_cpu_context.pc++;

    /* Need to reference the correct opcode table for the cycle lookup at the
       end of the function. */
    struct opcode_def* this_opcode_table = NULL;
    switch (t_r) {
    case REG_S:
        this_opcode_table = opcode_ext_x10_table;
        break;
    case REG_Y:
        this_opcode_table = opcode_ext_x10_table;
        break;
    default:
        this_opcode_table = opcode_table;
        break;
    }

    uint8 extra_cycles = 0;
    uint16 store_word = get_reg_value_16(t_r);
    write_word_handler(a_m, store_word, &extra_cycles);

    /* The Negative flag is set equal to the new value of bit 15 of
       the register. */
    e_cpu_context.cc.n = (store_word & 0x8000) > 1;
    /* The Zero flag is set if the new register value is zero;
       cleared otherwise. */
    e_cpu_context.cc.z = store_word == 0;
    /* The Overflow flag is cleared by this instruction. */
    e_cpu_context.cc.v = 0;

    return this_opcode_table[opcode].cycle_count + extra_cycles;
}

/* Subtract from value in 8-Bit Accumulator */
int sub(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    e_cpu_context.pc++;

    uint8 extra_cycles = 0;
    uint8 reg_val = get_reg_value_8(t_r);
    uint8 memory_val = read_byte_handler(a_m, &extra_cycles);
    uint8 total_val = reg_val - memory_val;

    /* The Carry flag is set if a borrow into bit-7 was needed; cleared
       otherwise. */
    e_cpu_context.cc.c = memory_val > reg_val;
    /* The Negative flag is set equal to the new value of bit 7 of
       the accumulator. */
    e_cpu_context.cc.n = (total_val & 0x80) > 1;
    /* The Zero flag is set if the new accumulator value is zero;
       cleared otherwise. */
    e_cpu_context.cc.z = total_val == 0;
    /* The Overflow flag is set if an overflow occurred; cleared otherwise. */
    /* If the difference of a positive and a negative number yields a negative
       result, the difference has overflowed. */
    uint8 pos_overflow = (reg_val & 0x80) == 0 && (memory_val & 0x80) > 0 &&
        e_cpu_context.cc.n;
    /* If the difference of a negative and a positive number yields a negative
       result, the difference has overflowed. */
    uint8 neg_overflow = (reg_val & 0x80) > 0 && (memory_val & 0x80) == 0 &&
        (total_val & 0x80) == 0;
    e_cpu_context.cc.v = pos_overflow || neg_overflow;

    set_reg_value_8(t_r, total_val);
    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Subtract from value in 16-Bit Accumulator */
int sub16(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    e_cpu_context.pc++;

    uint8 extra_cycles = 0;
    uint16 reg_val = get_reg_value_16(t_r);
    uint16 memory_val = read_word_handler(a_m, &extra_cycles);
    uint16 total_val = reg_val - memory_val;

    /* The Carry flag is set if a borrow into bit-7 was needed; cleared
       otherwise. */
    e_cpu_context.cc.c = memory_val > reg_val;
    /* The Negative flag is set equal to the new value of bit 7 of
       the accumulator. */
    e_cpu_context.cc.n = (total_val & 0x8000) > 1;
    /* The Zero flag is set if the new accumulator value is zero;
       cleared otherwise. */
    e_cpu_context.cc.z = total_val == 0;
    /* The Overflow flag is set if an overflow occurred; cleared otherwise. */
    /* If the difference of a positive and a negative number yields a negative
       result, the difference has overflowed. */
    uint8 pos_overflow = (reg_val & 0x8000) == 0 && (memory_val & 0x8000) > 0 &&
        e_cpu_context.cc.n;
    /* If the difference of a negative and a positive number yields a negative
       result, the difference has overflowed. */
    uint8 neg_overflow = (reg_val & 0x8000) > 0 && (memory_val & 0x8000) == 0 &&
        (total_val & 0x8000) == 0;
    e_cpu_context.cc.v = pos_overflow || neg_overflow;

    set_reg_value_16(t_r, total_val);
    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Transfer Register to Register */
int tfr(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) t_r; /* unused */
    (void) a_m; /* unused */
    e_cpu_context.pc++;

    uint8 extra_cycles = 0;
    uint8 post_byte = read_byte_handler(IMMEDIATE, &extra_cycles);
    enum target_register src;
    enum target_register trg;
    decode_source_target_postbyte(post_byte, &src, &trg);

    enum reg_size src_size = get_reg_size(src);
    enum reg_size trg_size = get_reg_size(trg);

    /* Tolerate one invalid register, not both */
    if (src_size == REG_SIZE_INVALID && trg_size != REG_SIZE_INVALID) {
        src_size = trg_size;
    }
    else if (trg_size == REG_SIZE_INVALID && src_size != REG_SIZE_INVALID) {
        trg_size = src_size;
    }
    else if (trg_size == REG_SIZE_INVALID && src_size == REG_SIZE_INVALID) {
        /* Do nothing when we get trolled */
        return opcode_table[opcode].cycle_count;
    }

    /* If an invalid register encoding is specified for either register, a
       constant value of FF or FFFF is used for the exchange.  */
    if (src_size == trg_size && src_size == REG_SIZE_8) {
        uint8 src_value = src == REG_NONE ? 0xFF : get_reg_value_8(src);
        if (trg != REG_NONE) {
            set_reg_value_8(trg, src_value);
        }
    }
    else if (src_size == trg_size && src_size == REG_SIZE_16) {
        uint16 src_value = src == REG_NONE ? 0xFFFF : get_reg_value_16(src);
        if (trg != REG_NONE) {
            set_reg_value_16(trg, src_value);
        }
    }
    else if (src_size == REG_SIZE_16) {
        uint16 src_value = get_reg_value_16(src);
        uint8 new_trg_value = (uint8) (src_value & 0xFF);
        set_reg_value_8(trg, new_trg_value);
    }
    else { /* src_size == REG_SIZE_8 && trg_size == REG_SIZE_16 */
        /* The 8-bit register is always exchanged with the lower half of the
           16-bit register, and the the upper half of the 16-bit register is
           then set to the value shown in the table below. */
        uint16 new_trg_value = 0;
        switch (src) {
        case REG_A:
        case REG_B:
            new_trg_value = 0xFF00 | get_reg_value_8(src);
            break;
        case REG_CC:
        case REG_DP:
            new_trg_value = (get_reg_value_8(src) << 8) | get_reg_value_8(src);
            break;
        default:
            assert(FALSE);
            break;
        }

        set_reg_value_16(trg, new_trg_value);
    }

    return opcode_table[opcode].cycle_count + extra_cycles;
}

/* Test Value in Accumulator */
int tst(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) a_m; /* unused */
    e_cpu_context.pc++;

    uint16 out_addr = 0;
    uint8 reg_val = 0;
    uint8 extra_cycles = 0;
    if (a_m == INHERENT) {
        reg_val = get_reg_value_8(t_r);
    }
    else {
        out_addr = get_memory_address_from_postbyte(a_m, &extra_cycles);
        reg_val = read_byte_from_memory(out_addr);
    }

    /* The Negative flag is set equal to the value of the accumulator’s
       high-order bit (sign bit). */
    e_cpu_context.cc.n = (reg_val & 0x80) > 0;
    /* The Zero flag is set if the new 8-bit value is zero; cleared
       otherwise. */
    e_cpu_context.cc.z = reg_val == 0;
    /* The Overflow flag is always cleared. */
    e_cpu_context.cc.v = 0;

    if (a_m == INHERENT) {
        set_reg_value_8(t_r, reg_val);
    }
    else {
        write_byte_to_memory(out_addr, reg_val);
    }

    return opcode_table[opcode].cycle_count + extra_cycles;
}
