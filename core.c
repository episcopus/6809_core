#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "types.h"
#include "consts.h"

struct cpu_state e_cpu_context;
extern struct opcode_def opcode_table[];

void core_init() {
    e_cpu_context.x = 0;
    e_cpu_context.y = 0;
    e_cpu_context.u = 0;
    e_cpu_context.s = 0;
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

/* This memory accessor reads a byte from the appropriate location
   in memory based on the addressing mode. Will move the pc
   appropriately based on the addressing mode and postbyte opcode. */
uint8 read_byte_handler(enum addressing_mode am) {
    uint8 return_byte = 0;
    switch (am) {
    case IMMEDIATE:
        /* byte is located right at the pc */
        return_byte = read_byte_from_memory(e_cpu_context.pc++);
        break;
    default:
        assert(FALSE);
        break;
    }

    return return_byte;
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
