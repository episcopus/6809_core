#include <assert.h>
#include <stdio.h>

#include "opcodes.h"

extern struct opcode_def opcode_table[];
extern struct opcode_def opcode_ext_x10_table[];
extern struct opcode_def opcode_ext_x11_table[];
extern struct cpu_state e_cpu_context;

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
    offset = (char) read_byte_from_memory(e_cpu_context.pc++);

    switch (opcode) {
    case OP_BCC:
        /* Branch If Carry Clear */
        if (e_cpu_context.cc.c) {
            /* Reverse logic here to make it always read the postbyte (offset)
               and advance the PC, nullifying the branch in the negative
               logic case. */
            offset = 0;
        }
        break;
    case OP_BCS:
        /* Branch If Carry Set */
        if (!e_cpu_context.cc.c) {
            offset = 0;
        }
        break;
    case OP_BEQ:
        /* Branch If Equal to Zero */
        if (!e_cpu_context.cc.z) {
            offset = 0;
        }
        break;
    case OP_BGE:
        /* Branch If Greater than or Equal to Zero */
        if (e_cpu_context.cc.n != e_cpu_context.cc.v) {
            offset = 0;
        }
        break;
    case OP_BGT:
        /* Branch If Greater Than Zero */
        if ((e_cpu_context.cc.n != e_cpu_context.cc.v) || e_cpu_context.cc.z) {
            offset = 0;
        }
        break;
    case OP_BHI:
        /* Branch If Higher */
        if (e_cpu_context.cc.z || e_cpu_context.cc.c) {
            offset = 0;
        }
        break;
    case OP_BLE:
        /* Branch If Less than or Equal to Zero */
        if ((e_cpu_context.cc.n == e_cpu_context.cc.v) && e_cpu_context.cc.z == 0) {
            offset = 0;
        }
        break;
    case OP_BLS:
        /* Branch If Lower or Same */
        if (!e_cpu_context.cc.z && !e_cpu_context.cc.c) {
            offset = 0;
        }
        break;
    case OP_BLT:
        /* Branch If Less Than Zero */
        if (e_cpu_context.cc.n == e_cpu_context.cc.v) {
            offset = 0;
        }
        break;
    case OP_BMI:
        /* Branch If Minus */
        if (!e_cpu_context.cc.n) {
            offset = 0;
        }
        break;
    case OP_BNE:
        /* Branch If Not Equal to Zero */
        if (e_cpu_context.cc.z) {
            offset = 0;
        }
        break;
    case OP_BPL:
        /* Branch If Plus */
        if (e_cpu_context.cc.n) {
            offset = 0;
        }
        break;
    case OP_BRA:
        /* Branch Always */
        break;
    case OP_BRN:
        /* Branch Never */
        offset = 0;
        break;
    case OP_BSR:
        /* Branch to Subroutine */
        /* Save current PC onto hardware stack */
        e_cpu_context.s -= 2;
        write_word_to_memory(e_cpu_context.s, e_cpu_context.pc);
        break;
    case OP_BVC:
        /* Branch If Overflow Clear */
        if (e_cpu_context.cc.v) {
            offset = 0;
        }
        break;
    case OP_BVS:
        /* Branch If Overflow Set */
        if (!e_cpu_context.cc.v) {
            offset = 0;
        }
        break;
    default:
        /* Invalid jump instruction. */
        assert(FALSE);
        offset = 0;
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
    offset = (short int) read_word_from_memory(e_cpu_context.pc);
    e_cpu_context.pc += 2;

    switch (opcode) {
    case OP_LBCC:
        /* Long Branch If Carry Clear */
        if (e_cpu_context.cc.c) {
            offset = 0;
        }
        else {
            /* The 6809 requires an extra cycle if the branch is taken. */
            extra_cycles++;
        }
        break;
    case OP_LBCS:
        /* Long Branch If Carry Set */
        if (!e_cpu_context.cc.c) {
            offset = 0;
        }
        else {
            /* The 6809 requires an extra cycle if the branch is taken. */
            extra_cycles++;
        }
        break;
    case OP_LBEQ:
        /* Long Branch If Equal to Zero */
        if (!e_cpu_context.cc.z) {
            offset = 0;
        }
        else {
            /* The 6809 requires an extra cycle if the branch is taken. */
            extra_cycles++;
        }
        break;
    case OP_LBGE:
        /* Long Branch If Greater than or Equal to Zero */
        if (e_cpu_context.cc.n != e_cpu_context.cc.v) {
            offset = 0;
        }
        else {
            /* The 6809 requires an extra cycle if the branch is taken. */
            extra_cycles++;
        }
        break;
    case OP_LBGT:
        /* Long Branch If Greater Than Zero */
        if ((e_cpu_context.cc.n != e_cpu_context.cc.v) || e_cpu_context.cc.z) {
            offset = 0;
        }
        else {
            /* The 6809 requires an extra cycle if the branch is taken. */
            extra_cycles++;
        }
        break;
    case OP_LBHI:
        /* Long Branch If Higher */
        if (e_cpu_context.cc.z || e_cpu_context.cc.c) {
            offset = 0;
        }
        else {
            /* The 6809 requires an extra cycle if the branch is taken. */
            extra_cycles++;
        }
        break;
    case OP_LBLE:
        /* Long Branch If Less than or Equal to Zero */
        if ((e_cpu_context.cc.n == e_cpu_context.cc.v) && e_cpu_context.cc.z == 0) {
            offset = 0;
        }
        else {
            /* The 6809 requires an extra cycle if the branch is taken. */
            extra_cycles++;
        }
        break;
    case OP_LBLS:
        /* Long Branch If Lower or Same */
        if (!e_cpu_context.cc.z && !e_cpu_context.cc.c) {
            offset = 0;
        }
        else {
            /* The 6809 requires an extra cycle if the branch is taken. */
            extra_cycles++;
        }
        break;
    case OP_LBLT:
        /* Long Branch If Less Than Zero */
        if (e_cpu_context.cc.n == e_cpu_context.cc.v) {
            offset = 0;
        }
        else {
            /* The 6809 requires an extra cycle if the branch is taken. */
            extra_cycles++;
        }
        break;
    case OP_LBMI:
        /* Long Branch If Minus */
        if (!e_cpu_context.cc.n) {
            offset = 0;
        }
        else {
            /* The 6809 requires an extra cycle if the branch is taken. */
            extra_cycles++;
        }
        break;
    case OP_LBNE:
        /* Long Branch If Not Equal to Zero */
        if (e_cpu_context.cc.z) {
            offset = 0;
        }
        else {
            /* The 6809 requires an extra cycle if the branch is taken. */
            extra_cycles++;
        }
        break;
    case OP_LBPL:
        /* Long Branch If Plus */
        if (e_cpu_context.cc.n) {
            offset = 0;
        }
        else {
            /* The 6809 requires an extra cycle if the branch is taken. */
            extra_cycles++;
        }
        break;
    case OP_LBRA:
        /* Long Branch Always */
        break;
    case OP_LBRN:
        /* Long Branch Never */
        offset = 0;
        break;
    case OP_LBSR:
        /* Long Branch to Subroutine */
        /* Save current PC onto hardware stack */
        e_cpu_context.s -= 2;
        write_word_to_memory(e_cpu_context.s, e_cpu_context.pc);
        break;
    case OP_LBVC:
        /* Long Branch If Overflow Clear */
        if (e_cpu_context.cc.v) {
            offset = 0;
        }
        else {
            /* The 6809 requires an extra cycle if the branch is taken. */
            extra_cycles++;
        }
        break;
    case OP_LBVS:
        /* Long Branch If Overflow Set */
        if (!e_cpu_context.cc.v) {
            offset = 0;
        }
        else {
            /* The 6809 requires an extra cycle if the branch is taken. */
            extra_cycles++;
        }
        break;
    default:
        /* Invalid jump instruction. */
        assert(FALSE);
        offset = 0;
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

/* Clear Condition Code Bits and Wait for Interrupt */
int cwai(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) a_m; /* unused */
    (void) t_r; /* unused */

    /* Similar to SYNC but allows unsuppressing interrupts via an and of
       the postbyte with the CC (to clear any IRQ, FIRQ inhibits), and then
       pushes the entire CPU state to the stack before waiting for
       interrupt. It's important to note that any FIRQ will end up needing
       to pop all the state but that should happen anyway given we set e to 1
       here. */

    e_cpu_context.pc++;
    uint8 postbyte = read_byte_from_memory(get_reg_value_16(REG_PC));
    e_cpu_context.pc++;

    uint8 new_cc = postbyte & get_reg_value_8(REG_CC);
    set_reg_value_8(REG_CC, new_cc);

    e_cpu_context.cc.e = 1;
    push_registers_to_stack(0xFF, REG_S);
    e_cpu_context.halted_state = HS_CWAI;

    return opcode_table[opcode].cycle_count;
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
    push_word_to_stack(REG_S, e_cpu_context.pc);
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
    extra_cycles += push_registers_to_stack(postbyte, t_r);

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
    extra_cycles += pull_registers_from_stack(postbyte, t_r);

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

/* Return from Interrupt */
int rti(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) t_r; /* unused */
    (void) a_m; /* unused */
    e_cpu_context.pc++;
    uint8 cycles = opcode_table[opcode].cycle_count;

    /* Note: restoring the CC also clears any interrupt inhibit mask which would
       be set by interrupt routine thereby making program eligible for interrupts
       again */
    set_reg_value_8(REG_CC, pull_byte_from_stack(REG_S));
    /* The e flag will have been set in the case of the slower interrupt mode
       that pushes the full cpu state */
    if (e_cpu_context.cc.e) {
        set_reg_value_8(REG_A, pull_byte_from_stack(REG_S));
        set_reg_value_8(REG_B, pull_byte_from_stack(REG_S));
        set_reg_value_8(REG_DP, pull_byte_from_stack(REG_S));
        set_reg_value_16(REG_X, pull_word_from_stack(REG_S));
        set_reg_value_16(REG_Y, pull_word_from_stack(REG_S));
        set_reg_value_16(REG_U, pull_word_from_stack(REG_S));

        cycles += 9;
    }

    set_reg_value_16(REG_PC, pull_word_from_stack(REG_S));

    return cycles;
}

/* Return from Subroutine */
int rts(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) t_r; /* unused */
    (void) a_m; /* unused */
    e_cpu_context.pc++;

    set_reg_value_16(REG_PC, pull_word_from_stack(REG_S));

    return opcode_table[opcode].cycle_count;
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

/* Software Interrupt */
int swi(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) t_r; /* unused */
    (void) a_m; /* unused */
    e_cpu_context.pc++;

    return process_swi(SWI_1);
}

/* Software Interrupt */
int swi2(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) t_r; /* unused */
    (void) a_m; /* unused */
    e_cpu_context.pc++;

    return process_swi(SWI_2);
}

/* Software Interrupt */
int swi3(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) t_r; /* unused */
    (void) a_m; /* unused */
    e_cpu_context.pc++;

    return process_swi(SWI_3);
}

/* Synchronize with Interrupt */
int sync(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) t_r; /* unused */
    (void) a_m; /* unused */
    e_cpu_context.pc++;

    /* Sets processor in "sync" state, essentially hanging pending an interrupt.
       Execution will resume even when interrupt is suppressed, simply carrying
       on at the PC. Otherwise process interrupt as per usual. */

    /* See clock cycle limitation here
       https://github.com/episcopus/6809_core/issues/1 */

    e_cpu_context.halted_state = HS_SYNC;

    return opcode_table[opcode].cycle_count;
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
