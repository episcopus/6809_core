#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "types.h"
#include "consts.h"

struct cpu_state e_cpu_context;

void core_init() {
    e_cpu_context.x = 0;
    e_cpu_context.y = 0;
    e_cpu_context.u = 0;
    e_cpu_context.s = 0;
    e_cpu_context.pc = 0;
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
