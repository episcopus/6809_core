#include "consts.h"
#include "types.h"
#include "pia.h"
#include "memory.h"
#include "core.h"
#include "vdg.h"

extern struct cpu_state e_cpu_context;

uint8 get_vdg_mode_from_sam() {
    uint8 return_val = 0;
    return_val |= e_cpu_context.sam_state.v0_control_bit;
    return_val |= e_cpu_context.sam_state.v1_control_bit << 1;
    return_val |= e_cpu_context.sam_state.v2_control_bit << 2;

    return return_val;
}
