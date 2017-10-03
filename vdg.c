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

uint16 get_video_starting_address_from_sam() {
    uint16 return_val = 0;
    return_val |= e_cpu_context.sam_state.f0_control_bit << 9;
    return_val |= e_cpu_context.sam_state.f1_control_bit << 10;
    return_val |= e_cpu_context.sam_state.f2_control_bit << 11;
    return_val |= e_cpu_context.sam_state.f3_control_bit << 12;
    return_val |= e_cpu_context.sam_state.f4_control_bit << 13;
    return_val |= e_cpu_context.sam_state.f5_control_bit << 14;
    return_val |= e_cpu_context.sam_state.f6_control_bit << 15;

    return return_val;
}
