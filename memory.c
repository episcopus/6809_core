#include <stdio.h>

#include "consts.h"
#include "types.h"
#include "memory.h"
#include "core.h"

extern struct cpu_state e_cpu_context;
const struct memory_range_handler_struct memory_handler_table[] = {
    { 0x0, 0xFFFF, MT_RAM }
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

uint8 coco_read_byte_from_memory(uint16 address) {
    enum memory_handler_type et = MT_RAM;
    read_byte_function bf = NULL;
    for (int i=0; i<memory_table_size; i++) {
        if (address >= memory_handler_table[i].low_addr &&
            address <= memory_handler_table[i].high_addr) {
            et = memory_handler_table[i].et;
            bf = basic_read_byte_from_memory;
            break;
        }
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
    if (address > MEMORY_SIZE - 1) {
        assert(FALSE);
        return;
    }

    e_cpu_context.memory[address] = byte;
    return;
}
