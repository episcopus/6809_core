#include "types.h"

struct cpu_state e_cpu_context;

void core_init() {
    e_cpu_context.x = 0;
    e_cpu_context.y = 0;
    e_cpu_context.u = 0;
    e_cpu_context.s = 0;
    e_cpu_context.pc = 0;
    e_cpu_context.d.d = 0;
    e_cpu_context.dp = 0;

    return;
}
