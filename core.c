#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "headers.h"

struct cpu_state e_cpu_context;
/* This struct captures the inverse order stacking order for register
   push and pull operations */
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

char register_names[11][4] = {
    "NONE",
    "X",
    "Y",
    "U",
    "S",
    "PC",
    "D",
    "DP",
    "A",
    "B",
    "CC"
};

extern struct opcode_def opcode_table[];
extern struct opcode_def opcode_ext_x10_table[];
extern struct opcode_def opcode_ext_x11_table[];

void setup_default_vector(uint16 source, uint16 dest) {
    e_cpu_context.memory[lookup_effective_address(source)] =
        (uint8) (dest >> 8);
    e_cpu_context.memory[lookup_effective_address(source+1)] =
        (uint8) (dest & 0xFF);
}

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
    else {
        /* Clear out the memory to since consecutive core_init() calls may
           resurface prior core memory */
        for (int i = 0; i < MEMORY_SIZE; i++) {
            e_cpu_context.memory[i] = 0;
        }
    }

    e_cpu_context.color_basic = NULL;
    e_cpu_context.extended_basic = NULL;

    e_cpu_context.cycle_count = 0;
    e_cpu_context.hsync_cycles = HSYNC_CYCLES_TOTAL;
    e_cpu_context.vsync_cycles = VSYNC_CYCLES_TOTAL;
    e_cpu_context.irq = 0;
    e_cpu_context.firq = 0;
    e_cpu_context.nmi = 0;
    e_cpu_context.halted_state = HS_NONE;

    e_cpu_context.memory_handler.read_byte_func = coco_read_byte_from_memory;
    e_cpu_context.memory_handler.write_byte_func = coco_write_byte_to_memory;
    e_cpu_context.memory_handler.read_word_func = coco_read_word_from_memory;
    e_cpu_context.memory_handler.write_word_func = coco_write_word_to_memory;

    e_cpu_context.vdg_state.video_buf = NULL;

    /* Memory map mode, RAM/ROM mode */
    e_cpu_context.sam_state.ty_control_bit = 0;

    /* Memory size, assign to 64K by default */
    e_cpu_context.sam_state.m1_control_bit = 1;
    e_cpu_context.sam_state.m0_control_bit = 0;

    /* MPU cycle rate, 0.89 MHz */
    e_cpu_context.sam_state.r1_control_bit = 0;
    e_cpu_context.sam_state.r0_control_bit = 0;

    /* Page number */
    e_cpu_context.sam_state.p1_control_bit = 0;

    /* Video display starting address, $0400 by default */
    e_cpu_context.sam_state.f6_control_bit = 0;
    e_cpu_context.sam_state.f5_control_bit = 0;
    e_cpu_context.sam_state.f4_control_bit = 0;
    e_cpu_context.sam_state.f3_control_bit = 0;
    e_cpu_context.sam_state.f2_control_bit = 0;
    e_cpu_context.sam_state.f1_control_bit = 1;
    e_cpu_context.sam_state.f0_control_bit = 0;

    /* Video display mode */
    e_cpu_context.sam_state.v2_control_bit = 0;
    e_cpu_context.sam_state.v1_control_bit = 0;
    e_cpu_context.sam_state.v0_control_bit = 0;

    /* Initialize PIA state to all zero's */
    e_cpu_context.pia_state.ddr_1_a = 0;
    e_cpu_context.pia_state.dddr_1_a = 0;
    e_cpu_context.pia_state.cr_1_a = 0;

    e_cpu_context.pia_state.ddr_1_b = 0;
    e_cpu_context.pia_state.dddr_1_b = 0;
    e_cpu_context.pia_state.cr_1_b = 0;

    e_cpu_context.pia_state.ddr_2_a = 0;
    e_cpu_context.pia_state.dddr_2_a = 0;
    e_cpu_context.pia_state.cr_2_a = 0;

    e_cpu_context.pia_state.ddr_2_b = 0;
    e_cpu_context.pia_state.dddr_2_b = 0;
    e_cpu_context.pia_state.cr_2_b = 0;

    for (int i=0; i<PIA_KEYBOARD_SIZE; i++) {
        e_cpu_context.pia_state.host_keys[i] = 0;
    }

    e_cpu_context.swi_hook = 0;
    e_cpu_context.swi_hook_set = 0;

    /* Set up default interrupt handlers, will be overridden by Basic */
    setup_default_vector(RESET_VECTOR, DEFAULT_RESET_VECTOR);
    setup_default_vector(NMI_VECTOR, DEFAULT_NMI_VECTOR);
    setup_default_vector(SWI_VECTOR, DEFAULT_SWI_VECTOR);
    setup_default_vector(IRQ_VECTOR, DEFAULT_IRQ_VECTOR);
    setup_default_vector(FIRQ_VECTOR, DEFAULT_FIRQ_VECTOR);
    setup_default_vector(SWI2_VECTOR, DEFAULT_SWI2_VECTOR);
    setup_default_vector(SWI3_VECTOR, DEFAULT_SWI3_VECTOR);

    return;
}

void core_destroy() {
    if (e_cpu_context.memory) {
        free(e_cpu_context.memory);
    }
    if (e_cpu_context.color_basic) {
        free(e_cpu_context.color_basic);
    }
    if (e_cpu_context.extended_basic) {
        free(e_cpu_context.extended_basic);
    }

    e_cpu_context.memory = NULL;
    e_cpu_context.color_basic = NULL;
    e_cpu_context.extended_basic = NULL;

    vdg_destroy();
}

void load_rom_to_address(const char* rom_path, uint8* target) {
    FILE* handle = fopen(rom_path, "rb");
    if (!handle) {
        assert(FALSE);
        return;
    }

    /* Figure out size of file in order to size buffer accordingly */
    int pos_beg = ftell(handle);
    if(fseek(handle, 0, SEEK_END)) {
        assert(FALSE);
        fclose(handle);
        return;
    }
    int pos_end = ftell(handle);
    int file_size = pos_end - pos_beg;
    rewind(handle);

    int read_size = fread((void *) target, 1, file_size, handle);
    if (!read_size || ferror(handle)) {
        assert(FALSE);
        fclose(handle);
        return;
    }
    fclose(handle);
}

void load_roms() {
    assert(!e_cpu_context.color_basic);
    assert(!e_cpu_context.extended_basic);

    const char* basic_rom = "/Users/simon/Dropbox/Programming/c/6809_core/roms/BASIC.ROM";
    const char* ext_rom = "/Users/simon/Dropbox/Programming/c/6809_core/roms/EXTBASIC.ROM";

    e_cpu_context.color_basic = (uint8*) malloc(ROM_SIZE);
    if (e_cpu_context.color_basic == NULL) {
        assert(FALSE);
        return;
    }

    e_cpu_context.extended_basic = (uint8*) malloc(ROM_SIZE);
    if (e_cpu_context.extended_basic == NULL) {
        assert(FALSE);
        return;
    }

    load_rom_to_address(basic_rom, e_cpu_context.color_basic);
    load_rom_to_address(ext_rom, e_cpu_context.extended_basic);
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
    return e_cpu_context.memory_handler.read_byte_func(address);
}

void write_byte_to_memory(uint16 address, uint8 byte) {
    return e_cpu_context.memory_handler.write_byte_func(address, byte);
}

uint16 read_word_from_memory(uint16 address) {
    return e_cpu_context.memory_handler.read_word_func(address);
}

void write_word_to_memory(uint16 address, uint16 word) {
    return e_cpu_context.memory_handler.write_word_func(address, word);
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
    case INDEXED:
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

void push_byte_to_stack(enum target_register t_r, uint8 data) {
    if (t_r != REG_S && t_r != REG_U) {
        assert(FALSE);
        return;
    }

    /* baseline stack pointer, decremented for each push, works for
       both u and s pointer */
    uint16 stack_pointer = get_reg_value_16(t_r);
    write_byte_to_memory(--stack_pointer, data);
    set_reg_value_16(t_r, stack_pointer);
}

uint8 pull_byte_from_stack(enum target_register t_r) {
    if (t_r != REG_S && t_r != REG_U) {
        assert(FALSE);
        return 0;
    }

    /* baseline stack pointer, incremented for each pull, works for
       both u and s pointer */
    uint16 stack_pointer = get_reg_value_16(t_r);
    uint8 this_val = read_byte_from_memory(stack_pointer);
    stack_pointer += 1;
    set_reg_value_16(t_r, stack_pointer);

    return this_val;
}

void push_word_to_stack(enum target_register t_r, uint16 data) {
    if (t_r != REG_S && t_r != REG_U) {
        assert(FALSE);
        return;
    }

    /* baseline stack pointer, decremented for each push, works for
       both u and s pointer */
    uint16 stack_pointer = get_reg_value_16(t_r);
    stack_pointer -= 2;
    write_word_to_memory(stack_pointer, data);
    set_reg_value_16(t_r, stack_pointer);
}

uint16 pull_word_from_stack(enum target_register t_r) {
    if (t_r != REG_S && t_r != REG_U) {
        assert(FALSE);
        return 0;
    }

    /* baseline stack pointer, incremented for each pull, works for
       both u and s pointer */
    uint16 stack_pointer = get_reg_value_16(t_r);
    uint16 this_val = read_word_from_memory(stack_pointer);
    stack_pointer += 2;
    set_reg_value_16(t_r, stack_pointer);

    return this_val;
}

uint8 push_registers_to_stack(uint8 reg_field, enum target_register reg_stack) {
    /* Utility function meant to centralize the push / pull order of registers
       to and from the stack. Used for interrupts, RTI, PSH, etc.
       The reg_field identifies which registers to push. The reg_context enables
       REG_U to be switched to REG_S when we are invoked in the context of
       the latter. */

    /* 1 cycle per byte pushed */
    uint8 extra_cycles = 0;

    /* number of possible registers to push - it's really 8 but
       the REG_S value is special and needs to toggle since can't push own
       stack pointer */
    int sizeof_table = sizeof(stack_op_pb_entry_table) /
        sizeof(struct stack_op_postbyte_entry);
    /* push all necessary registers as requested in the postbyte according
       to the 6809 spec. */
    for (int i = sizeof_table - 1; i >= 0; i--) {
        if (reg_field & (1 << i)) {
            enum target_register this_t_r = stack_op_pb_entry_table[i].reg;

            if (get_reg_size(this_t_r) == REG_SIZE_16) {
                /* REG_S in this table is a special value that flips to REG_U in
                   the case where the function is called with REG_S target
                   register. This is because the psh instruction cannot push
                   its own stack pointer */
                this_t_r == REG_S && reg_stack == REG_S ? this_t_r = REG_U :
                    REG_S;
                uint16 val = get_reg_value_16(this_t_r);

                push_word_to_stack(reg_stack, val);
                extra_cycles += 2;
            }
            else if (get_reg_size(this_t_r) == REG_SIZE_8) {
                uint8 val = get_reg_value_8(this_t_r);

                push_byte_to_stack(reg_stack, val);
                extra_cycles++;
            }
        }
    }

    return extra_cycles;
}

uint8 pull_registers_from_stack(uint8 reg_field, enum target_register reg_stack) {
    /* Utility function meant to centralize the push / pull order of registers
       to and from the stack. Used for interrupts, RTI, PSH, etc.
       The reg_field identifies which registers to push. The reg_context enables
       REG_U to be switched to REG_S when we are invoked in the context of
       the latter. */

    /* 1 cycle per byte pushed */
    uint8 extra_cycles = 0;

    /* number of possible registers to pull - it's really 8 but
       the REG_S value is special and needs to toggle since can't pull own
       stack pointer */
    int sizeof_table = sizeof(stack_op_pb_entry_table) /
        sizeof(struct stack_op_postbyte_entry);
    /* pull all necessary registers as requested in the postbyte according
       to the 6809 spec. */
    for (int i = 0; i < sizeof_table; i++) {
        if (reg_field & (1 << i)) {
            enum target_register this_t_r = stack_op_pb_entry_table[i].reg;

            if (get_reg_size(this_t_r) == REG_SIZE_16) {
                /* REG_S in this table is a special value that flips to REG_U in
                   the case where the function is called with REG_S target
                   register. This is because the pul instruction cannot pull
                   its own stack pointer */
                this_t_r == REG_S && reg_stack == REG_S ? this_t_r = REG_U :
                    REG_S;

                uint16 this_val = pull_word_from_stack(reg_stack);
                set_reg_value_16(this_t_r, this_val);

                extra_cycles += 2;
            }
            else if (get_reg_size(this_t_r) == REG_SIZE_8) {
                uint8 this_val = pull_byte_from_stack(reg_stack);
                set_reg_value_8(this_t_r, this_val);

                extra_cycles++;
            }
        }
    }

    return extra_cycles;
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
        e_cpu_context.pc++;
        return_address = decode_extended_indirect(out_extra_cycles);
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
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
            return_address = decode_inc_dec_offset_postbyte(out_extra_cycles);
            break;
        case 0xC:
        case 0xD:
            return_address = decode_constant_offset_from_pc(out_extra_cycles);
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

uint16 decode_inc_dec_offset_postbyte(uint8* out_extra_cycles) {
    uint8 postbyte = read_byte_from_memory(e_cpu_context.pc++);
    enum target_register tr = decode_register_from_indexed_postbyte(postbyte);
    uint16 return_address = 0;
    uint8 indirect = postbyte & 0x10;
    *out_extra_cycles = 0;

    uint8 lower_nibble = postbyte & 0xF;
    uint16 base_address = get_reg_value_16(tr);
    char post_delta = 0;
    char pre_delta = 0;

    switch (lower_nibble) {
    case 0x0:
        post_delta = 1;
        *out_extra_cycles = 2;
        break;
    case 0x1:
        post_delta = 2;
        *out_extra_cycles = 3;
        break;
    case 0x2:
        pre_delta = -1;
        *out_extra_cycles = 2;
        break;
    case 0x3:
        pre_delta = -2;
        *out_extra_cycles = 3;
        break;
    }

    if (pre_delta != 0) {
        base_address += pre_delta;
        set_reg_value_16(tr, base_address);
    }

    return_address = base_address;

    if (indirect) {
        return_address = read_word_from_memory(return_address);
        *out_extra_cycles += 3;
    }

    if (post_delta != 0) {
        base_address += post_delta;
        set_reg_value_16(tr, base_address);
    }

    return return_address;
}

uint16 decode_constant_offset_from_pc(uint8* out_extra_cycles) {
    uint8 postbyte = read_byte_from_memory(e_cpu_context.pc++);
    short int offset = 0;
    uint16 return_address = 0;
    uint8 indirect = postbyte & 0x10;
    *out_extra_cycles = 0;
    uint8 lower_nibble = postbyte & 0xF;
    char one_byte_offset = 0;

    switch (lower_nibble) {
    case 0xC:
        one_byte_offset = (char) read_byte_from_memory(e_cpu_context.pc++);
        offset = (int) one_byte_offset;
        *out_extra_cycles = 1;
        break;
    case 0xD:
        offset = (int) read_word_from_memory(e_cpu_context.pc);
        e_cpu_context.pc += 2;
        *out_extra_cycles = 5;
        break;
    }

    uint16 base_address = get_reg_value_16(REG_PC);
    return_address = base_address + offset;
    if (indirect) {
        return_address = read_word_from_memory(return_address);
        *out_extra_cycles += 3;
    }

    return return_address;
}

uint16 decode_extended_indirect(uint8* out_extra_cycles) {
    *out_extra_cycles = 5;
    uint16 base_address = read_word_from_memory(e_cpu_context.pc);
    uint16 return_address = read_word_from_memory(base_address);
    e_cpu_context.pc += 2;

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
        int this_completed_cycles = 0;

        if (e_cpu_context.swi_hook_set) {
            /* Execution breaks to the monitor since SWI was called after the
               swi_hook was turned on */
            break;
        }

        if (e_cpu_context.nmi || e_cpu_context.firq ||
            e_cpu_context.irq) {
            this_completed_cycles = process_interrupts();
            completed_cycles += this_completed_cycles;
            perform_tick_housekeeping(this_completed_cycles);
        }

        if (completed_cycles >= wanted_cycles) {
            break;
        }

        if (e_cpu_context.halted_state) {
            /* Execution is halted pending an interrupt. See sync() or
               cwai(). */
            break;
        }

        uint8 opcode = e_cpu_context.memory[e_cpu_context.pc];
        struct opcode_def this_opcode = opcode_table[opcode];
        assert(strncmp("NOTIMPL", this_opcode.instruction, 7) != 0);

        this_completed_cycles = this_opcode.func(opcode, this_opcode.t_r,
                                                  this_opcode.mode);
        completed_cycles += this_completed_cycles;
        perform_tick_housekeeping(this_completed_cycles);
    }

    return completed_cycles;
}

uint32 run_hsync_interval() {
    /* Runs one HSYNC worth of cycles, which is around 57, do various
       housekeeping, meant to be called by emulator host at the
       appropriate frequency (15,750 Hz) */
    uint32 this_cycles = run_cycles(e_cpu_context.hsync_cycles);
    return this_cycles;
}

void perform_tick_housekeeping(uint32 cycles) {
    e_cpu_context.cycle_count += cycles;
    perform_hsync_housekeeping(cycles);
    perform_vsync_housekeeping(cycles);
}

void perform_hsync_housekeeping(uint32 cycles) {
    e_cpu_context.hsync_cycles -= cycles;

    if (e_cpu_context.hsync_cycles <= 0) {
        /* Accumulate any deviation to the next call */
        e_cpu_context.hsync_cycles += HSYNC_CYCLES_TOTAL;

        /* Now do any HSYNC housekeeping */
        uint8 pia1_cra = pia_read_byte_from_memory(0xFF01);
        uint8 hsync_irq_enabled = pia1_cra & 0x1;
        if (!e_cpu_context.cc.i && hsync_irq_enabled) {
            /* Trigger HSYNC IRQ if enabled at the PIA */
            e_cpu_context.irq = 1;
        }
        /* Set the interrupt flag in the PIA regardless */
        pia_write_byte_to_memory(0xFF01, pia1_cra | 0x8);
    }
}

void perform_vsync_housekeeping(uint32 cycles) {
    e_cpu_context.vsync_cycles -= cycles;

    if (e_cpu_context.vsync_cycles <= 0) {
        /* Accumulate any deviation to the next call */
        e_cpu_context.vsync_cycles += VSYNC_CYCLES_TOTAL;

        /* Now do any VSYNC housekeeping */
        uint8 pia1_crb = pia_read_byte_from_memory(0xFF03);
        uint8 vsync_irq_enabled = pia1_crb & 0x1;
        if (!e_cpu_context.cc.i && vsync_irq_enabled) {
            /* Trigger VSYNC IRQ if enabled at the PIA */
            e_cpu_context.irq = 1;
        }
        /* Set the interrupt flag in the PIA regardless */
        pia_write_byte_to_memory(0xFF03, pia1_crb | 0x8);

        /* TODO update screen */
    }
}

uint32 process_interrupts() {
    uint32 completed_cycles = 0;
    if (e_cpu_context.halted_state == HS_SYNC) {
        /* Receiving an interrupt unblocks the SYNC blockage, regardless whether
           of whether interrupt is suppressed. */
        e_cpu_context.halted_state = HS_NONE;
    }

    if (e_cpu_context.nmi) {
        if (e_cpu_context.halted_state != HS_CWAI) {
            /* Push all registers, inhibit FIRQ and IRQ and set up interrupt
               vector, but not in the CWAI case which did the pushing already */
            e_cpu_context.cc.e = 1;
            completed_cycles += push_registers_to_stack(0xFF, REG_S);
        }

        e_cpu_context.halted_state = HS_NONE;
        e_cpu_context.cc.i = 1;
        e_cpu_context.cc.f = 1;
        set_reg_value_16(REG_PC, read_word_from_memory(NMI_VECTOR));
        /* This interrupt is only edge sensitive therefore will only activate
           once. */
        e_cpu_context.nmi = 0;
    }
    else if (e_cpu_context.firq && !e_cpu_context.cc.f) {
        if (e_cpu_context.halted_state != HS_CWAI) {
            /* Fast interrupt only pushes PC and CC and inhibits further
               IRQ and FIRQ to occur, however the CWAI instruction will have
               pushed all registers already so skip this. */
            e_cpu_context.cc.e = 0;
            completed_cycles += push_registers_to_stack(0x81, REG_S);
        }

        e_cpu_context.halted_state = HS_NONE;
        e_cpu_context.cc.i = 1;
        e_cpu_context.cc.f = 1;
        set_reg_value_16(REG_PC, read_word_from_memory(FIRQ_VECTOR));
        e_cpu_context.firq = 0;
    }
    else if (e_cpu_context.irq && !e_cpu_context.cc.i) {
        if (e_cpu_context.halted_state != HS_CWAI) {
            /* IRQ pushes all the registers, inhibits further IRQ's,
               but not if CWAI already did this */
            e_cpu_context.cc.e = 1;
            completed_cycles += push_registers_to_stack(0xFF, REG_S);
        }

        e_cpu_context.halted_state = HS_NONE;
        e_cpu_context.cc.i = 1;
        set_reg_value_16(REG_PC, read_word_from_memory(IRQ_VECTOR));
        e_cpu_context.irq = 0;
    }

    return completed_cycles;
}

uint32 process_swi(enum swi_type swi) {
    uint32 swi_vector = SWI_VECTOR;
    switch (swi) {
    case SWI_1:
        swi_vector = SWI_VECTOR;
        break;
    case SWI_2:
        swi_vector = SWI2_VECTOR;
        break;
    case SWI_3:
        swi_vector = SWI3_VECTOR;
        break;
    default:
        assert(FALSE);
        break;
    }

    /* Special trap for monitor execution */
    if (e_cpu_context.swi_hook) {
        e_cpu_context.swi_hook_set = 1;
        return 0;
    }

    /* Saves CPU state, inhibits interrupts and branches to the SWI vector */
    e_cpu_context.cc.e = 1;
    push_registers_to_stack(0xFF, REG_S);
    if (swi == SWI_1) {
        e_cpu_context.cc.i = 1;
        e_cpu_context.cc.f = 1;
    }
    set_reg_value_16(REG_PC, read_word_from_memory(swi_vector));

    uint32 cycles = swi == SWI_1 ? opcode_table[OP_SWI].cycle_count :
        opcode_ext_x10_table[OP_SWI2].cycle_count;

    return cycles;
}

int extended(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    /* Look up appropriate operation by dereferencing second byte */
    (void) a_m; /* unused */
    (void) t_r; /* unused */
    e_cpu_context.pc++;

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
    return this_completed_cycles;
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

uint8 disassemble_instruction(uint16 pc, char* decoded) {
    uint8 opcode = e_cpu_context.memory[pc++];
    struct opcode_def this_opcode = opcode_table[opcode];
    uint8 num_bytes = 1;

    if (this_opcode.opcode == OP_EXTENDED_X10) {
        opcode = e_cpu_context.memory[pc++];
        this_opcode = opcode_ext_x10_table[opcode];
        num_bytes++;
    }
    else if (this_opcode.opcode == OP_EXTENDED_X11) {
        opcode = e_cpu_context.memory[pc++];
        this_opcode = opcode_ext_x11_table[opcode];
        num_bytes++;
    }

    if (strncmp("NOTIMPL", this_opcode.instruction, 7) == 0) {
        if (num_bytes == 2) {
            uint8 prev_opcode = e_cpu_context.memory[pc - 2];
            sprintf(decoded, "$%.2X $%.2X", prev_opcode, opcode);
        }
        else {
            sprintf(decoded, "$%.2X", opcode);
        }
    }
    else {
        switch (this_opcode.mode) {
        case IMMEDIATE:
            if (get_reg_size(this_opcode.t_r) == REG_SIZE_8) {
                num_bytes++;
                uint8 operand = e_cpu_context.memory[pc++];
                sprintf(decoded, "%s #$%.2X", this_opcode.instruction, operand);
            }
            else {
                num_bytes += 2;
                uint16 operand = e_cpu_context.memory[pc] << 8 | e_cpu_context.memory[pc + 1];
                sprintf(decoded, "%s #$%.4X", this_opcode.instruction, operand);
            }
            break;
        case DIRECT:
            num_bytes++;
            uint8 dp = get_reg_value_8(REG_DP);
            uint16 operand = dp << 8 | e_cpu_context.memory[pc];
            sprintf(decoded, "%s >$%.4X", this_opcode.instruction, operand);
            break;
        case INDEXED:
            sprintf(decoded, "%s ", this_opcode.instruction);
            uint8 offset = strlen(decoded);
            num_bytes += disassemble_indexed_addressing_postbyte(pc, decoded + offset);
            break;
        case EXTENDED:
            num_bytes += 2;
            operand = e_cpu_context.memory[pc] << 8 | e_cpu_context.memory[pc + 1];
            sprintf(decoded, "%s >$%.4X", this_opcode.instruction, operand);
            break;
        case INHERENT:
            sprintf(decoded, "%s", this_opcode.instruction);
            break;
        }
    }

    return num_bytes;
}

uint8 disassemble_indexed_addressing_postbyte(uint16 pc, char* decoded) {
    uint8 postbyte = e_cpu_context.memory[pc];
    uint8 return_bytes = 1;

    // Handle special / unique cases first and then switch on the main types.
    if (!(postbyte & 0x80)) {
        // 5 bit Constant Offset
        return_bytes = disassemble_constant_offset_postbyte(pc, decoded);
    }
    else if (postbyte == 0x9F) {
        // Extended Indirect
        return_bytes = disassemble_extended_indirect(pc, decoded);
    }
    else {
        uint8 lower_nibble = postbyte & 0xF;
        switch (lower_nibble) {
        case 0x4:
        case 0x8:
        case 0x9:
            return_bytes = disassemble_constant_offset_postbyte(pc, decoded);
            break;
        case 0x5:
        case 0x6:
        case 0xB:
            /* return_address = decode_accumulator_offset_postbyte(out_extra_cycles); */
            /* break; */
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
            /* return_address = decode_inc_dec_offset_postbyte(out_extra_cycles); */
            /* break; */
        case 0xC:
        case 0xD:
            /* return_address = decode_constant_offset_from_pc(out_extra_cycles); */
            /* break; */
            break;
        }
    }

    return return_bytes;
}

uint8 disassemble_constant_offset_postbyte(uint16 pc, char* decoded) {
    uint8 postbyte = read_byte_from_memory(pc++);
    enum target_register tr = decode_register_from_indexed_postbyte(postbyte);
    short int offset = 0;
    uint8 return_bytes = 1;
    uint8 indirect = postbyte & 0x10;

    if (!(postbyte & 0x80)) {
        // 5 bit Constant Offset
        offset = postbyte & 0x1F;
        // Poor attempt at 5-bit to 16-bit two's complement conversion follows.
        // Hope it works
        offset |= (offset & 0x10) ? 0xFFF0 : 0;
        // 5-bit offset has no indirect mode
        indirect = 0;
    }
    else {
        uint8 lower_nibble = postbyte & 0xF;
        char one_byte_offset = 0;
        switch (lower_nibble) {
        case 0x4:
            offset = 0;
            break;
        case 0x8:
            one_byte_offset = (char) read_byte_from_memory(pc++);
            return_bytes++;
            offset = (int) one_byte_offset;
            break;
        case 0x9:
            offset = (int) read_word_from_memory(pc);
            pc += 2;
            return_bytes += 2;
            break;
        }
    }

    /* TODO indirect addressing */
    char offset_string[80] = { 0 };
    if (offset != 0) {
        sprintf(offset_string, "%d", offset);
    }
    char* register_string = register_names[tr];
    if (indirect) {
        sprintf(decoded, "[%s,%s]", offset_string, register_string);
    }
    else {
        sprintf(decoded, "%s,%s", offset_string, register_string);
    }

    return return_bytes;
}

uint8 disassemble_extended_indirect(uint16 pc, char* decoded) {
    uint16 base_address = read_word_from_memory(pc + 1);
    sprintf(decoded, "[$%.4X]", base_address);

    return 3;
}

uint16 init_from_decb_memory(const uint8* buffer, uint16 buffer_size) {
    /*
    From the LWASM documentation:

    A DECB binary is compatible with the LOADM command in Disk Extended
    Basic on the CoCo. They are also compatible with CLOADM from
    Extended Color Basic. These binaries include the load address of
    the binary as well as encoding an execution address. These binaries
    may contain multiple loadable sections, each of which has its own
    load address.

    Each binary starts with a preamble. Each preamble is five bytes
    long. The first byte is zero. The next two bytes specify the number
    of bytes to load and the last two bytes specify the address to load
    the bytes at. Then, a string of bytes follows. After this string of
    bytes, there may be another preamble or a postamble. A postamble is
    also five bytes in length. The first byte of the postamble is $FF,
    the next two are zero, and the last two are the execution address
    for the binary.
    */

    uint16 read_bytes = 0;
    uint16 read_preambles = 0;
    uint16 new_pc = 0;
    while (read_bytes < buffer_size) {
        uint16 bytes_to_read = 0;
        uint16 target_address = 0;
        uint16 local_read_bytes = 0;
        /* Attempt to read a preamble or postamble */
        if (buffer[read_bytes] != 0 && buffer[read_bytes] != 0xFF) {
            /* Invalid, expected 0 or 0xFF */
            assert(FALSE);
            read_preambles = 0;
            break;
        }

        if (buffer[read_bytes] == 0) {
            if (read_bytes + 4 >= buffer_size) {
                /* Invalid, not enough size for preamble */
                assert(FALSE);
                read_preambles = 0;
                break;
            }

            /* It's a preamble so read in that chunk */
            read_bytes++;

            /* Convert low endian assumed value to big endian */
            bytes_to_read = buffer[read_bytes] << 8 | buffer[read_bytes + 1];
            read_bytes += 2;
            target_address = buffer[read_bytes] << 8 | buffer[read_bytes + 1];
            read_bytes += 2;
            if (read_bytes + bytes_to_read >= buffer_size) {
                /* Invalid, not enough size for buffer */
                assert(FALSE);
                read_preambles = 0;
                break;
            }

            while (local_read_bytes < bytes_to_read) {
                write_byte_to_memory(target_address + local_read_bytes,
                                     buffer[read_bytes++]);
                local_read_bytes++;
            }

            read_preambles++;
        }
        else {
            if (read_bytes + 4 != buffer_size - 1) {
                /* Invalid, not enough size for postamble */
                assert(FALSE);
                read_preambles = 0;
                break;
            }

            /* It's a postamble so just make sure it's alright and process
               address */
            read_bytes++;

            if (buffer[read_bytes] != 0 || buffer[read_bytes + 1] != 0) {
                /* Invalid, expected 0 or 0xFF */
                assert(FALSE);
                read_preambles = 0;
                break;
            }

            read_bytes += 2;
            new_pc = buffer[read_bytes] << 8 | buffer[read_bytes + 1];
            read_bytes += 2;
        }
    }

    e_cpu_context.pc = new_pc;
    return read_preambles;
}

uint16 init_from_decb_file(const char* filename) {
    FILE* handle = fopen(filename, "rb");
    if (!handle) {
        assert(FALSE);
        return 0;
    }

    /* Figure out size of file in order to size buffer accordingly */
    int pos_beg = ftell(handle);
    if(fseek(handle, 0, SEEK_END)) {
        assert(FALSE);
        fclose(handle);
        return 0;
    }
    int pos_end = ftell(handle);
    int file_size = pos_end - pos_beg;
    rewind(handle);

    uint8* my_buffer = (uint8*) malloc(file_size);
    int read_size = fread((void *) my_buffer, 1, file_size, handle);
    if (!read_size || ferror(handle)) {
        assert(FALSE);
        free((void *) my_buffer);
        fclose(handle);
        return 0;
    }

    uint16 ret_preambles = init_from_decb_memory(my_buffer, (uint16) read_size);

    free((void *) my_buffer);
    fclose(handle);

    return ret_preambles;
}
