#include <assert.h>
#include "functions.h"

extern struct opcode_def opcode_table[];
extern struct cpu_state e_cpu_context;

int abx(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) t_r; /* unused */
    (void) a_m; /* unused */

    e_cpu_context.pc++;
    e_cpu_context.x += e_cpu_context.d.byte_acc.b;
    return opcode_table[opcode].cycle_count;
}

int asla(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) t_r; /* unused */
    (void) a_m; /* unused */

    e_cpu_context.pc++;
    e_cpu_context.x += e_cpu_context.d.byte_acc.b;
    return opcode_table[opcode].cycle_count;
}

int nop(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) t_r; /* unused */
    (void) a_m; /* unused */

    e_cpu_context.pc++;
    return opcode_table[opcode].cycle_count;
}

int notimpl(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) t_r; /* unused */
    (void) a_m; /* unused */

    /* Not yet implemented opcode */
    assert(FALSE);

    return opcode_table[opcode].cycle_count;
}
