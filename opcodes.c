#include "functions.h"

extern struct opcode_def opcode_table[];

int nop(uint8 opcode, enum target_register t_r, enum addressing_mode a_m) {
    (void) t_r; /* unused */
    (void) a_m; /* unused */

    /* TODO advance program counter */

    return opcode_table[opcode].cycle_count;
}
