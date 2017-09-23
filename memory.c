#include <stdio.h>

#include "consts.h"
#include "types.h"
#include "memory.h"
#include "core.h"

extern struct cpu_state e_cpu_context;
const struct memory_range_handler_struct memory_handler_table[] = {
    { 0x0, 0xFFBF, MT_RAM },
    { 0xFFC0, 0xFFDF, MT_SAM },
    { 0xFFE0, 0xFFFF, MT_RAM }
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

uint8 coco_read_byte_from_memory(uint16 address) {
    read_byte_function bf = NULL;
    switch (get_mh_type_from_address(address)) {
    case MT_RAM:
        bf = basic_read_byte_from_memory;
        break;
    case MT_SAM:
        bf = sam_read_byte_from_memory;
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
