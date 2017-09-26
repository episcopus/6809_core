#include <stdio.h>

#include "consts.h"
#include "types.h"
#include "pia.h"
#include "memory.h"
#include "core.h"

extern struct cpu_state e_cpu_context;
static const struct memory_range_handler_struct memory_handler_table[] = {
    { 0x0,    0xFEFF, MT_RAM   },
    { 0xFF00, 0xFF03, MT_PIA   },
    { 0xFF04, 0xFF1F, MT_DEDIC },
    { 0xFF20, 0xFF23, MT_PIA   },
    { 0xFF24, 0xFFBF, MT_DEDIC },
    { 0xFFC0, 0xFFDF, MT_SAM   },
    { 0xFFE0, 0xFFFF, MT_DEDIC }
};

static const uint16 memory_redirect_table[256][2] = {
    {0x0, 0x0},
    {0x1, 0x1},
    {0x2, 0x2},
    {0x3, 0x3},
    {0x4, 0x0},
    {0x5, 0x1},
    {0x6, 0x2},
    {0x7, 0x3},
    {0x8, 0x0},
    {0x9, 0x1},
    {0xA, 0x2},
    {0xB, 0x3},
    {0xC, 0x0},
    {0xD, 0x1},
    {0xE, 0x2},
    {0xF, 0x3},
    {0x10, 0x0},
    {0x11, 0x1},
    {0x12, 0x2},
    {0x13, 0x3},
    {0x14, 0x0},
    {0x15, 0x1},
    {0x16, 0x2},
    {0x17, 0x3},
    {0x18, 0x0},
    {0x19, 0x1},
    {0x1A, 0x2},
    {0x1B, 0x3},
    {0x1C, 0x0},
    {0x1D, 0x1},
    {0x1E, 0x2},
    {0x1F, 0x3},
    {0x20, 0x20},
    {0x21, 0x21},
    {0x22, 0x22},
    {0x23, 0x23},
    {0x24, 0x20},
    {0x25, 0x21},
    {0x26, 0x22},
    {0x27, 0x23},
    {0x28, 0x20},
    {0x29, 0x21},
    {0x2A, 0x22},
    {0x2B, 0x23},
    {0x2C, 0x20},
    {0x2D, 0x21},
    {0x2E, 0x22},
    {0x2F, 0x23},
    {0x30, 0x20},
    {0x31, 0x21},
    {0x32, 0x22},
    {0x33, 0x23},
    {0x34, 0x20},
    {0x35, 0x21},
    {0x36, 0x22},
    {0x37, 0x23},
    {0x38, 0x20},
    {0x39, 0x21},
    {0x3A, 0x22},
    {0x3B, 0x23},
    {0x3C, 0x20},
    {0x3D, 0x21},
    {0x3E, 0x22},
    {0x3F, 0x23},
    {0x40, 0x40},
    {0x41, 0x40},
    {0x42, 0x40},
    {0x43, 0x40},
    {0x44, 0x40},
    {0x45, 0x40},
    {0x46, 0x40},
    {0x47, 0x40},
    {0x48, 0x48},
    {0x49, 0x49},
    {0x4A, 0x4A},
    {0x4B, 0x4B},
    {0x4C, 0x48},
    {0x4D, 0x49},
    {0x4E, 0x4A},
    {0x4F, 0x4B},
    {0x50, 0x40},
    {0x51, 0x40},
    {0x52, 0x40},
    {0x53, 0x40},
    {0x54, 0x40},
    {0x55, 0x40},
    {0x56, 0x40},
    {0x57, 0x40},
    {0x58, 0x48},
    {0x59, 0x49},
    {0x5A, 0x4A},
    {0x5B, 0x4B},
    {0x5C, 0x48},
    {0x5D, 0x49},
    {0x5E, 0x4A},
    {0x5F, 0x4B},
    {0x60, 0x60},
    {0x61, 0x61},
    {0x62, 0x62},
    {0x63, 0x63},
    {0x64, 0x64},
    {0x65, 0x65},
    {0x66, 0x66},
    {0x67, 0x67},
    {0x68, 0x68},
    {0x69, 0x69},
    {0x6A, 0x6A},
    {0x6B, 0x6B},
    {0x6C, 0x6C},
    {0x6D, 0x6D},
    {0x6E, 0x6E},
    {0x6F, 0x6F},
    {0x70, 0x70},
    {0x71, 0x71},
    {0x72, 0x72},
    {0x73, 0x73},
    {0x74, 0x74},
    {0x75, 0x75},
    {0x76, 0x76},
    {0x77, 0x77},
    {0x78, 0x78},
    {0x79, 0x79},
    {0x7A, 0x7A},
    {0x7B, 0x7B},
    {0x7C, 0x7C},
    {0x7D, 0x7D},
    {0x7E, 0x7E},
    {0x7F, 0x7F},
    {0x80, 0x80},
    {0x81, 0x81},
    {0x82, 0x82},
    {0x83, 0x83},
    {0x84, 0x84},
    {0x85, 0x85},
    {0x86, 0x86},
    {0x87, 0x87},
    {0x88, 0x88},
    {0x89, 0x89},
    {0x8A, 0x8A},
    {0x8B, 0x8B},
    {0x8C, 0x8C},
    {0x8D, 0x8D},
    {0x8E, 0x8E},
    {0x8F, 0x8F},
    {0x90, 0x90},
    {0x91, 0x91},
    {0x92, 0x92},
    {0x93, 0x93},
    {0x94, 0x94},
    {0x95, 0x95},
    {0x96, 0x96},
    {0x97, 0x97},
    {0x98, 0x98},
    {0x99, 0x99},
    {0x9A, 0x9A},
    {0x9B, 0x9B},
    {0x9C, 0x9C},
    {0x9D, 0x9D},
    {0x9E, 0x9E},
    {0x9F, 0x9F},
    {0xA0, 0xA0},
    {0xA1, 0xA1},
    {0xA2, 0xA2},
    {0xA3, 0xA3},
    {0xA4, 0xA4},
    {0xA5, 0xA5},
    {0xA6, 0xA6},
    {0xA7, 0xA7},
    {0xA8, 0xA8},
    {0xA9, 0xA9},
    {0xAA, 0xAA},
    {0xAB, 0xAB},
    {0xAC, 0xAC},
    {0xAD, 0xAD},
    {0xAE, 0xAE},
    {0xAF, 0xAF},
    {0xB0, 0xB0},
    {0xB1, 0xB1},
    {0xB2, 0xB2},
    {0xB3, 0xB3},
    {0xB4, 0xB4},
    {0xB5, 0xB5},
    {0xB6, 0xB6},
    {0xB7, 0xB7},
    {0xB8, 0xB8},
    {0xB9, 0xB9},
    {0xBA, 0xBA},
    {0xBB, 0xBB},
    {0xBC, 0xBC},
    {0xBD, 0xBD},
    {0xBE, 0xBE},
    {0xBF, 0xBF},
    {0xC0, 0xC0},
    {0xC1, 0xC1},
    {0xC2, 0xC2},
    {0xC3, 0xC3},
    {0xC4, 0xC4},
    {0xC5, 0xC5},
    {0xC6, 0xC6},
    {0xC7, 0xC7},
    {0xC8, 0xC8},
    {0xC9, 0xC9},
    {0xCA, 0xCA},
    {0xCB, 0xCB},
    {0xCC, 0xCC},
    {0xCD, 0xCD},
    {0xCE, 0xCE},
    {0xCF, 0xCF},
    {0xD0, 0xD0},
    {0xD1, 0xD1},
    {0xD2, 0xD2},
    {0xD3, 0xD3},
    {0xD4, 0xD4},
    {0xD5, 0xD5},
    {0xD6, 0xD6},
    {0xD7, 0xD7},
    {0xD8, 0xD8},
    {0xD9, 0xD9},
    {0xDA, 0xDA},
    {0xDB, 0xDB},
    {0xDC, 0xDC},
    {0xDD, 0xDD},
    {0xDE, 0xDE},
    {0xDF, 0xDF},
    {0xE0, 0xE0},
    {0xE1, 0xE1},
    {0xE2, 0xE2},
    {0xE3, 0xE3},
    {0xE4, 0xE4},
    {0xE5, 0xE5},
    {0xE6, 0xE6},
    {0xE7, 0xE7},
    {0xE8, 0xE8},
    {0xE9, 0xE9},
    {0xEA, 0xEA},
    {0xEB, 0xEB},
    {0xEC, 0xEC},
    {0xED, 0xED},
    {0xEE, 0xEE},
    {0xEF, 0xEF},
    {0xF0, 0xF0},
    {0xF1, 0xF1},
    {0xF2, 0xF2},
    {0xF3, 0xF3},
    {0xF4, 0xF4},
    {0xF5, 0xF5},
    {0xF6, 0xF6},
    {0xF7, 0xF7},
    {0xF8, 0xF8},
    {0xF9, 0xF9},
    {0xFA, 0xFA},
    {0xFB, 0xFB},
    {0xFC, 0xFC},
    {0xFD, 0xFD},
    {0xFE, 0xFE},
    {0xFF, 0xFF}
};

const uint8 memory_table_size = sizeof(memory_handler_table) /
    sizeof(struct memory_range_handler_struct);

uint8 basic_read_byte_from_memory(uint16 address) {
    if (address > MEMORY_SIZE - 1) {
        assert(FALSE);
        return 0;
    }

    uint8 return_byte = e_cpu_context.memory[address];
    return return_byte;
}

void basic_write_byte_to_memory(uint16 address, uint8 byte) {
    if (address > MEMORY_SIZE - 1) {
        assert(FALSE);
        return;
    }

    e_cpu_context.memory[address] = byte;
    return;
}

uint16 basic_read_word_from_memory(uint16 address) {
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

void basic_write_word_to_memory(uint16 address, uint16 word) {
    if (address > MEMORY_SIZE - 2) {
        assert(FALSE);
        return;
    }

    /* flip bytes due to differing endianess of CPU being emulated */
    e_cpu_context.memory[address + 1] = (uint8) word & 0xFF;
    e_cpu_context.memory[address] = (uint8) ((word >> 8) & 0xFF);
    return;
}

uint8 sam_read_byte_from_memory(uint16 address) {
    switch (address) {
    case 0xFFDF:
    case 0xFFDE:
        return e_cpu_context.sam_state.ty_control_bit;
    case 0xFFDD:
    case 0xFFDC:
        return e_cpu_context.sam_state.m1_control_bit;
    case 0xFFDB:
    case 0xFFDA:
        return e_cpu_context.sam_state.m0_control_bit;
    case 0xFFD9:
    case 0xFFD8:
        return e_cpu_context.sam_state.r1_control_bit;
    case 0xFFD7:
    case 0xFFD6:
        return e_cpu_context.sam_state.r0_control_bit;
    case 0xFFD5:
    case 0xFFD4:
        return e_cpu_context.sam_state.p1_control_bit;
    case 0xFFD3:
    case 0xFFD2:
        return e_cpu_context.sam_state.f6_control_bit;
    case 0xFFD1:
    case 0xFFD0:
        return e_cpu_context.sam_state.f5_control_bit;
    case 0xFFCF:
    case 0xFFCE:
        return e_cpu_context.sam_state.f4_control_bit;
    case 0xFFCD:
    case 0xFFCC:
        return e_cpu_context.sam_state.f3_control_bit;
    case 0xFFCB:
    case 0xFFCA:
        return e_cpu_context.sam_state.f2_control_bit;
    case 0xFFC9:
    case 0xFFC8:
        return e_cpu_context.sam_state.f1_control_bit;
    case 0xFFC7:
    case 0xFFC6:
        return e_cpu_context.sam_state.f0_control_bit;
    case 0xFFC5:
    case 0xFFC4:
        return e_cpu_context.sam_state.v2_control_bit;
    case 0xFFC3:
    case 0xFFC2:
        return e_cpu_context.sam_state.v1_control_bit;
    case 0xFFC1:
    case 0xFFC0:
        return e_cpu_context.sam_state.v0_control_bit;
    default:
        assert(FALSE);
        return 0;
    }
}

void sam_write_byte_to_memory(uint16 address, uint8 byte) {
    switch (address) {
    case 0xFFDF:
        e_cpu_context.sam_state.ty_control_bit = 1;
        break;
    case 0xFFDE:
        e_cpu_context.sam_state.ty_control_bit = 0;
        break;
    case 0xFFDD:
        e_cpu_context.sam_state.m1_control_bit = 1;
        break;
    case 0xFFDC:
        e_cpu_context.sam_state.m1_control_bit = 0;
        break;
    case 0xFFDB:
        e_cpu_context.sam_state.m0_control_bit = 1;
        break;
    case 0xFFDA:
        e_cpu_context.sam_state.m0_control_bit = 0;
        break;
    case 0xFFD9:
        e_cpu_context.sam_state.r1_control_bit = 1;
        break;
    case 0xFFD8:
        e_cpu_context.sam_state.r1_control_bit = 0;
        break;
    case 0xFFD7:
        e_cpu_context.sam_state.r0_control_bit = 1;
        break;
    case 0xFFD6:
        e_cpu_context.sam_state.r0_control_bit = 0;
        break;
    case 0xFFD5:
        e_cpu_context.sam_state.p1_control_bit = 1;
        break;
    case 0xFFD4:
        e_cpu_context.sam_state.p1_control_bit = 0;
        break;
    case 0xFFD3:
        e_cpu_context.sam_state.f6_control_bit = 1;
        break;
    case 0xFFD2:
        e_cpu_context.sam_state.f6_control_bit = 0;
        break;
    case 0xFFD1:
        e_cpu_context.sam_state.f5_control_bit = 1;
        break;
    case 0xFFD0:
        e_cpu_context.sam_state.f5_control_bit = 0;
        break;
    case 0xFFCF:
        e_cpu_context.sam_state.f4_control_bit = 1;
        break;
    case 0xFFCE:
        e_cpu_context.sam_state.f4_control_bit = 0;
        break;
    case 0xFFCD:
        e_cpu_context.sam_state.f3_control_bit = 1;
        break;
    case 0xFFCC:
        e_cpu_context.sam_state.f3_control_bit = 0;
        break;
    case 0xFFCB:
        e_cpu_context.sam_state.f2_control_bit = 1;
        break;
    case 0xFFCA:
        e_cpu_context.sam_state.f2_control_bit = 0;
        break;
    case 0xFFC9:
        e_cpu_context.sam_state.f1_control_bit = 1;
        break;
    case 0xFFC8:
        e_cpu_context.sam_state.f1_control_bit = 0;
        break;
    case 0xFFC7:
        e_cpu_context.sam_state.f0_control_bit = 1;
        break;
    case 0xFFC6:
        e_cpu_context.sam_state.f0_control_bit = 0;
        break;
    case 0xFFC5:
        e_cpu_context.sam_state.v2_control_bit = 1;
        break;
    case 0xFFC4:
        e_cpu_context.sam_state.v2_control_bit = 0;
        break;
    case 0xFFC3:
        e_cpu_context.sam_state.v1_control_bit = 1;
        break;
    case 0xFFC2:
        e_cpu_context.sam_state.v1_control_bit = 0;
        break;
    case 0xFFC1:
        e_cpu_context.sam_state.v0_control_bit = 1;
        break;
    case 0xFFC0:
        e_cpu_context.sam_state.v0_control_bit = 0;
        break;
    default:
        assert(FALSE);
    }
}

enum memory_handler_type get_mh_type_from_address(uint16 address) {
    enum memory_handler_type et = MT_RAM;
    for (int i=0; i<memory_table_size; i++) {
        if (address >= memory_handler_table[i].low_addr &&
            address <= memory_handler_table[i].high_addr) {
            et = memory_handler_table[i].et;
            break;
        }
    }

    return et;
}

uint16 lookup_effective_address(uint16 address) {
    uint16 effective_address = address;
    if (address >= 0xFFF2) {
        /* This region is unconditionally redirected AFAIK for interrupt
           vectors to the Basic ROM area of A000-BFFF.
           Push 0xFFFx address down to the 0xBFFx vectors. */
        effective_address -= 0x4000;
    }
    else {
        /* Look up any address indirection due to memory map mode */
        if (!e_cpu_context.sam_state.ty_control_bit) {
            uint16 lookup_index = address - 0xFF00;
            effective_address = 0xFF00 + (memory_redirect_table[lookup_index][1]);
        }
    }

    return effective_address;
}

uint8 dedicated_read_byte_from_memory(uint16 address) {
    uint16 effective_address = lookup_effective_address(address);

    if (address != effective_address) {
        return coco_read_byte_from_memory(effective_address);
    }
    else {
        return basic_read_byte_from_memory(effective_address);
    }
}

void dedicated_write_byte_to_memory(uint16 address, uint8 byte) {
    uint16 effective_address = lookup_effective_address(address);

    if (address != effective_address) {
        return coco_write_byte_to_memory(effective_address, byte);
    }
    else {
        return basic_write_byte_to_memory(effective_address, byte);
    }
}

uint16 dedicated_read_word_from_memory(uint16 address) {
    uint16 effective_address = lookup_effective_address(address);

    if (address != effective_address) {
        return coco_read_word_from_memory(effective_address);
    }
    else {
        return basic_read_word_from_memory(effective_address);
    }
}

void dedicated_write_word_to_memory(uint16 address, uint16 word) {
    uint16 effective_address = lookup_effective_address(address);

    if (address != effective_address) {
        return coco_write_word_to_memory(effective_address, word);
    }
    else {
        return basic_write_word_to_memory(effective_address, word);
    }
}

uint8 coco_read_byte_from_memory(uint16 address) {
    read_byte_function bf = NULL;
    switch (get_mh_type_from_address(address)) {
    case MT_RAM:
        bf = basic_read_byte_from_memory;
        break;
    case MT_SAM:
        bf = sam_read_byte_from_memory;
        break;
    case MT_DEDIC:
        bf = dedicated_read_byte_from_memory;
        break;
    case MT_PIA:
        bf = pia_read_byte_from_memory;
        break;
    default:
        assert(FALSE);
        break;
    }

    uint8 return_byte = 0x0;
    if (bf == NULL) {
        assert(FALSE);
        return return_byte;
    }

    return_byte = bf(address);
    return return_byte;
}

void coco_write_byte_to_memory(uint16 address, uint8 byte) {
    write_byte_function bf = NULL;
    switch (get_mh_type_from_address(address)) {
    case MT_RAM:
        bf = basic_write_byte_to_memory;
        break;
    case MT_SAM:
        bf = sam_write_byte_to_memory;
        break;
    case MT_DEDIC:
        bf = dedicated_write_byte_to_memory;
        break;
    case MT_PIA:
        bf = pia_write_byte_to_memory;
        break;
    default:
        assert(FALSE);
        break;
    }

    if (bf == NULL) {
        assert(FALSE);
        return;
    }

    bf(address, byte);
}

uint16 coco_read_word_from_memory(uint16 address) {
    read_word_function wf = NULL;
    switch (get_mh_type_from_address(address)) {
    case MT_RAM:
        wf = basic_read_word_from_memory;
        break;
    case MT_DEDIC:
        wf = dedicated_read_word_from_memory;
        break;
        /* Don't allow word based access to SAM and PIA for now, let's see how
           this goes. */
    default:
        assert(FALSE);
        break;
    }

    uint16 return_word = 0x0;
    if (wf == NULL) {
        assert(FALSE);
        return return_word;
    }

    return_word = wf(address);
    return return_word;
}

void coco_write_word_to_memory(uint16 address, uint16 word) {
    write_word_function bf = NULL;
    switch (get_mh_type_from_address(address)) {
    case MT_RAM:
        bf = basic_write_word_to_memory;
        break;
    case MT_DEDIC:
        bf = dedicated_write_word_to_memory;
        break;
        /* Don't allow word based access to SAM and PIA for now, let's see how
           this goes. */
    default:
        assert(FALSE);
        break;
    }

    if (bf == NULL) {
        assert(FALSE);
        return;
    }

    bf(address, word);
}
