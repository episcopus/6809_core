#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "core.h"

struct cpu_state e_cpu_context;
extern struct opcode_def opcode_table[];
extern struct opcode_def opcode_ext_x10_table[];
extern struct opcode_def opcode_ext_x11_table[];

void core_init() {
    e_cpu_context.x = 0;
    e_cpu_context.y = 0;
    e_cpu_context.u = U_POINTER;
    e_cpu_context.s = S_POINTER;
    e_cpu_context.pc = USER_SPACE_ROOT;
    e_cpu_context.d.d = 0;
    e_cpu_context.dp = 0;
    e_cpu_context.cc.e = 0;
    e_cpu_context.cc.f = 0;
    e_cpu_context.cc.h = 0;
    e_cpu_context.cc.i = 0;
    e_cpu_context.cc.n = 0;
    e_cpu_context.cc.z = 0;
    e_cpu_context.cc.v = 0;
    e_cpu_context.cc.c = 0;
    e_cpu_context.memory = NULL;
    e_cpu_context.memory = (uint8*) malloc(MEMORY_SIZE);
    if (e_cpu_context.memory == NULL) {
        assert(FALSE);
    }
    /* Clear out the memory to since consecutive core_init() calls may
       resurface prior core memory */
    for (int i = 0; i < MEMORY_SIZE; i++) {
        e_cpu_context.memory[i] = 0;
    }
    e_cpu_context.cycle_count = 0;

    return;
}

void core_destroy() {
    free(e_cpu_context.memory);
}

int load_memory(struct mem_loader_def* defs, uint8 num_defs) {
    if (num_defs == 0) {
        return 0;
    }

    int loaded = 0;
    for (int i = 0; i < num_defs; i++) {
        if (!defs[i].byte_payload) {
            assert(FALSE);
            continue;
        }

        if (!defs[i].byte_size) {
            assert(FALSE);
            continue;
        }

        if (defs[i].root_address > MEMORY_SIZE - 1) {
            assert(FALSE);
            continue;
        }

        if (defs[i].root_address + defs[i].byte_size > MEMORY_SIZE - 1) {
            assert(FALSE);
            continue;
        }

        void* target_address = e_cpu_context.memory + defs[i].root_address;
        memcpy((void *) target_address, (void *) defs[i].byte_payload,
               defs[i].byte_size);
        loaded++;
    }

    return loaded;
}

uint8 read_byte_from_memory(uint16 address) {
    if (address > MEMORY_SIZE - 1) {
        assert(FALSE);
        return 0;
    }

    uint8 return_byte = e_cpu_context.memory[address];
    return return_byte;
}

void write_byte_to_memory(uint16 address, uint8 byte) {
    if (address > MEMORY_SIZE - 1) {
        assert(FALSE);
        return;
    }

    e_cpu_context.memory[address] = byte;
    return;
}

uint16 read_word_from_memory(uint16 address) {
    if (address > MEMORY_SIZE - 2) {
        assert(FALSE);
        return 0;
    }

    uint16 return_word = 0;
    /* data is stored big endian, needs to be flipped for little
       endian host emulator */
    return_word |= e_cpu_context.memory[address + 1];
    return_word |= (uint16) e_cpu_context.memory[address] << 8;
    return return_word;
}

void write_word_to_memory(uint16 address, uint16 word) {
    if (address > MEMORY_SIZE - 2) {
        assert(FALSE);
        return;
    }

    /* flip bytes due to differing endianess of CPU being emulated */
    e_cpu_context.memory[address + 1] = (uint8) word & 0xFF;
    e_cpu_context.memory[address] = (uint8) ((word >> 8) & 0xFF);
    return;
}

/* This memory accessor reads a byte from the appropriate location
   in memory based on the addressing mode. Will move the pc
   appropriately based on the addressing mode and postbyte opcode. */
uint8 read_byte_handler(enum addressing_mode am, uint8* out_extra_cycles) {
    uint8 return_byte = 0;
    uint16 byte_addr = get_memory_address_from_postbyte(am, out_extra_cycles);
    return_byte = read_byte_from_memory(byte_addr);

    return return_byte;
}

/* Write a byte to the appropriate location based on the decoded
   postbyte / addressing mode combination */
void write_byte_handler(enum addressing_mode am, uint8 byte, uint8* out_extra_cycles) {
    uint16 byte_addr = get_memory_address_from_postbyte(am, out_extra_cycles);
    write_byte_to_memory(byte_addr, byte);
}

/* This memory accessor reads a word from the appropriate location
   in memory based on the addressing mode. Will move the pc
   appropriately based on the addressing mode and postbyte opcode. */
uint16 read_word_handler(enum addressing_mode am, uint8* out_extra_cycles) {
    uint16 return_word = 0;
    uint16 word_addr = get_memory_address_from_postbyte(am, out_extra_cycles);
    switch (am) {
    case IMMEDIATE:
        /* word is located right at the pc */
        return_word = read_word_from_memory(word_addr);
        e_cpu_context.pc++;
        break;
    case DIRECT:
        /* byte following instruction is lower 8 bit of full address with
           upper 8 bit being direct page register */
    case EXTENDED:
        /* word following instruction is full 16 bit address of
           operand */
        return_word = read_word_from_memory(word_addr);
        break;
    default:
        assert(FALSE);
        break;
    }

    return return_word;
}

/* Write a word to the appropriate location based on the decoded
   postbyte / addressing mode combination */
void write_word_handler(enum addressing_mode am, uint16 word, uint8* out_extra_cycles) {
    uint16 word_addr = get_memory_address_from_postbyte(am, out_extra_cycles);
    write_word_to_memory(word_addr, word);
}

uint16 get_memory_address_from_postbyte(enum addressing_mode am, uint8* out_extra_cycles) {
    uint16 return_addr = 0;
    uint8 lower_byte = 0, upper_byte = 0;
    *out_extra_cycles = 0;
    switch (am) {
    case IMMEDIATE:
        /* byte is located right at the pc */
        return_addr = e_cpu_context.pc++;
        break;
    case DIRECT:
        /* byte following instruction is lower 8 bit of full address with
           upper 8 bit being direct page register */
        lower_byte = read_byte_from_memory(e_cpu_context.pc++);
        upper_byte = e_cpu_context.dp;
        return_addr = upper_byte << 8 | lower_byte;
        break;
    case EXTENDED:
        /* word following instruction is full 16 bit address of
           operand */
        return_addr = read_word_from_memory(e_cpu_context.pc);
        e_cpu_context.pc += 2;
        break;
    case INDEXED:
        return_addr = decode_indexed_addressing_postbyte(out_extra_cycles);
        break;
    default:
        assert(FALSE);
        break;
    }

    return return_addr;
}

uint16 decode_indexed_addressing_postbyte(uint8* out_extra_cycles) {
    uint8 postbyte = e_cpu_context.memory[e_cpu_context.pc];
    uint16 return_address = 0;
    *out_extra_cycles = 0;

    // Handle special / unique cases first and then switch on the main types.
    if (!(postbyte & 0x80)) {
        // 5 bit Constant Offset
        return_address = decode_constant_offset_postbyte(out_extra_cycles);
    }
    else if (postbyte == 0x9F) {
        // Extended Indirect
        *out_extra_cycles = 5;
    }
    else {
        uint8 lower_nibble = postbyte & 0xF;
        switch (lower_nibble) {
        case 0x4:
        case 0x8:
        case 0x9:
            return_address = decode_constant_offset_postbyte(out_extra_cycles);
            break;
        case 0x5:
        case 0x6:
        case 0xB:
            return_address = decode_accumulator_offset_postbyte(out_extra_cycles);
            break;
        }
    }

    return return_address;
}

uint16 decode_constant_offset_postbyte(uint8* out_extra_cycles) {
    uint8 postbyte = read_byte_from_memory(e_cpu_context.pc++);
    enum target_register tr = decode_register_from_indexed_postbyte(postbyte);
    short int offset = 0;
    uint16 return_address = 0;
    uint8 indirect = postbyte & 0x10;
    *out_extra_cycles = 0;

    if (!(postbyte & 0x80)) {
        // 5 bit Constant Offset
        offset = postbyte & 0x1F;
        // Poor attempt at 5-bit to 16-bit two's complement conversion follows.
        // Hope it works
        offset |= (offset & 0x10) ? 0xFFF0 : 0;
        *out_extra_cycles = 1;
        // 5-bit offset has no indirect mode
        indirect = 0;
    }
    else {
        uint8 lower_nibble = postbyte & 0xF;
        char one_byte_offset = 0;
        switch (lower_nibble) {
        case 0x4:
            offset = 0;
            *out_extra_cycles = 0;
            break;
        case 0x8:
            one_byte_offset = (char) read_byte_from_memory(e_cpu_context.pc++);
            offset = (int) one_byte_offset;
            *out_extra_cycles = 1;
            break;
        case 0x9:
            offset = (int) read_word_from_memory(e_cpu_context.pc);
            e_cpu_context.pc += 2;
            *out_extra_cycles = 4;
            break;
        }
    }

    uint16 base_address = get_reg_value_16(tr);
    return_address = base_address + offset;
    if (indirect) {
        return_address = read_word_from_memory(return_address);
        *out_extra_cycles += 3;
    }

    return return_address;
}

uint16 decode_accumulator_offset_postbyte(uint8* out_extra_cycles) {
    uint8 postbyte = read_byte_from_memory(e_cpu_context.pc++);
    enum target_register tr = decode_register_from_indexed_postbyte(postbyte);
    short int offset = 0;
    uint16 return_address = 0;
    uint8 indirect = postbyte & 0x10;
    *out_extra_cycles = 0;
    uint8 lower_nibble = postbyte & 0xF;

    switch (lower_nibble) {
    case 0x6:
        offset = (char) get_reg_value_8(REG_A);
        *out_extra_cycles = 1;
        break;
    case 0x5:
        offset = (char) get_reg_value_8(REG_B);
        *out_extra_cycles = 1;
        break;
    case 0xB:
        offset = get_reg_value_16(REG_D);
        *out_extra_cycles = 4;
        break;
    }
    uint16 base_address = get_reg_value_16(tr);
    return_address = base_address + offset;
    if (indirect) {
        return_address = read_word_from_memory(return_address);
        *out_extra_cycles += 3;
    }

    return return_address;
}

enum target_register decode_register_from_indexed_postbyte(uint8 postbyte) {
    uint8 masked = (postbyte & 0x60) >> 5;
    switch (masked) {
    case 0:
        return REG_X;
    case 1:
        return REG_Y;
    case 2:
        return REG_U;
    case 3:
        return REG_S;
    default:
        assert(FALSE);
        return REG_NONE;
    }
}

uint32 run_cycles(uint32 wanted_cycles) {
    uint32 completed_cycles = 0;

    while (completed_cycles < wanted_cycles) {
        uint8 opcode = e_cpu_context.memory[e_cpu_context.pc];
        struct opcode_def this_opcode = opcode_table[opcode];
        assert(strncmp("NOTIMPL", this_opcode.instruction, 7) != 0);

        int this_completed_cycles = this_opcode.func(opcode, this_opcode.t_r,
                                             this_opcode.mode);
        e_cpu_context.cycle_count += this_completed_cycles;
        completed_cycles += this_completed_cycles;
    }

    return completed_cycles;
}

int extended(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    /* Look up appropriate operation by dereferencing second byte */
    (void) a_m; /* unused */
    (void) t_r; /* unused */

    e_cpu_context.pc++;
    uint32 completed_cycles = 0;

    uint8 cur_opcode = e_cpu_context.memory[e_cpu_context.pc];
    struct opcode_def this_opcode;
    switch (opcode) {
    case OP_EXTENDED_X10:
        this_opcode = opcode_ext_x10_table[cur_opcode];
        break;
    case OP_EXTENDED_X11:
        this_opcode = opcode_ext_x11_table[cur_opcode];
        break;
    }

    assert(strncmp("NOTIMPL", this_opcode.instruction, 7) != 0);
    int this_completed_cycles = this_opcode.func(this_opcode.opcode,
                                                 this_opcode.t_r,
                                                 this_opcode.mode);
    e_cpu_context.cycle_count += this_completed_cycles;
    completed_cycles += this_completed_cycles;

    return completed_cycles;
}

enum target_register decode_target_register_from_postbyte(uint8 postbyte) {
    switch (postbyte) {
    case 0x0:
        return REG_D;
    case 0x1:
        return REG_X;
    case 0x2:
        return REG_Y;
    case 0x3:
        return REG_U;
    case 0x4:
        return REG_S;
    case 0x5:
        return REG_PC;
    case 0x8:
        return REG_A;
    case 0x9:
        return REG_B;
    case 0xA:
        return REG_CC;
    case 0xB:
        return REG_DP;
    default:
        return REG_NONE;
    }
}

void decode_source_target_postbyte(uint8 postbyte, enum target_register* out_source, enum target_register* out_target) {
    if (!out_source || !out_target) {
        assert(FALSE);
        return;
    }

    uint8 upper_nibble = postbyte >> 4;
    uint8 lower_nibble = postbyte & 0xF;

    *out_source = decode_target_register_from_postbyte(upper_nibble);
    *out_target = decode_target_register_from_postbyte(lower_nibble);
}

enum reg_size get_reg_size(enum target_register reg) {
    switch (reg) {
    case REG_A:
    case REG_B:
    case REG_DP:
    case REG_CC:
        return REG_SIZE_8;
    case REG_NONE:
        return REG_SIZE_INVALID;
    default:
        return REG_SIZE_16;
    }
}

uint8 get_reg_value_8(enum target_register reg) {
    switch (reg) {
    case REG_A:
        return e_cpu_context.d.byte_acc.a;
    case REG_B:
        return e_cpu_context.d.byte_acc.b;
    case REG_DP:
        return e_cpu_context.dp;
    case REG_CC:
        return *((uint8*) (&e_cpu_context.cc));
    default:
        assert(FALSE);
        return 0xFF;
    }
}

uint16 get_reg_value_16(enum target_register reg) {
    switch (reg) {
    case REG_X:
        return e_cpu_context.x;
    case REG_Y:
        return e_cpu_context.y;
    case REG_U:
        return e_cpu_context.u;
    case REG_S:
        return e_cpu_context.s;
    case REG_PC:
        return e_cpu_context.pc;
    case REG_D:
        return e_cpu_context.d.d;
    default:
        assert(FALSE);
        return 0xFFFF;
    }
}

void set_reg_value_8(enum target_register reg, uint8 value) {
    switch (reg) {
    case REG_A:
        e_cpu_context.d.byte_acc.a = value;
        return;
    case REG_B:
        e_cpu_context.d.byte_acc.b = value;
        return;
    case REG_DP:
        e_cpu_context.dp = value;
        return;
    case REG_CC:
        *((uint8*) (&e_cpu_context.cc)) = value;
        return;
    default:
        assert(FALSE);
        return;
    }
}

void set_reg_value_16(enum target_register reg, uint16 value) {
    switch (reg) {
    case REG_X:
        e_cpu_context.x = value;
        return;
    case REG_Y:
        e_cpu_context.y = value;
        return;
    case REG_U:
        e_cpu_context.u = value;
        return;
    case REG_S:
        e_cpu_context.s = value;
        return;
    case REG_PC:
        e_cpu_context.pc = value;
        return;
    case REG_D:
        e_cpu_context.d.d = value;
        return;
    default:
        assert(FALSE);
        return;
    }
}
