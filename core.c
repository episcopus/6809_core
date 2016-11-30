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
        e_cpu_context.memory[i++] = 0;
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
uint8 read_byte_handler(enum addressing_mode am) {
    uint8 return_byte = 0, lower_byte = 0, upper_byte = 0;
    switch (am) {
    case IMMEDIATE:
        /* byte is located right at the pc */
        return_byte = read_byte_from_memory(e_cpu_context.pc++);
        break;
    case DIRECT:
        /* byte following instruction is lower 8 bit of full address with
           upper 8 bit being direct page register */
        lower_byte = read_byte_from_memory(e_cpu_context.pc++);
        upper_byte = e_cpu_context.dp;
        return_byte = read_byte_from_memory(upper_byte << 8 | lower_byte);
        break;
    default:
        assert(FALSE);
        break;
    }

    return return_byte;
}

/* This memory accessor reads a word from the appropriate location
   in memory based on the addressing mode. Will move the pc
   appropriately based on the addressing mode and postbyte opcode. */
uint16 read_word_handler(enum addressing_mode am) {
    uint16 return_word = 0;
    switch (am) {
    case IMMEDIATE:
        /* word is located right at the pc */
        return_word = read_word_from_memory(e_cpu_context.pc);
        e_cpu_context.pc += 2;
        break;
    default:
        assert(FALSE);
        break;
    }

    return return_word;
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
