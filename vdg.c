#include <stdlib.h>

#include "consts.h"
#include "types.h"
#include "pia.h"
#include "memory.h"
#include "vdg.h"
#include "core.h"

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

enum vdg_mode get_vdg_mode() {
    uint8 sam_vdg_mode = get_vdg_mode_from_sam();
    uint8 pia_vdg_mode = e_cpu_context.pia_state.ddr_2_b;
    enum vdg_mode ret_mode = VDG_AI;
    switch (sam_vdg_mode) {
    case 0:
        if (pia_vdg_mode & 0x10) {
            ret_mode = VDG_SG6;
        }
        else {
            ret_mode = VDG_AI;
        }
        break;
    case 1:
        if (pia_vdg_mode & 0x10) {
            ret_mode = VDG_G1R;
        }
        else {
            ret_mode = VDG_G1C;
        }
        break;
    case 2:
        if ((pia_vdg_mode & 0xA0) == 0xA0) {
            ret_mode = VDG_G2C;
        }
        else {
            ret_mode = VDG_SG8;
        }
        break;
    case 3:
        ret_mode = VDG_G2R;
        break;
    case 4:
        if ((pia_vdg_mode & 0xC0) == 0xC0) {
            ret_mode = VDG_G3C;
        }
        else {
            ret_mode = VDG_SG12;
        }
        break;
    case 5:
        ret_mode = VDG_G3R;
        break;
    case 6:
        if ((pia_vdg_mode & 0xF0) == 0xF0) {
            ret_mode = VDG_G6R;
        }
        else if ((pia_vdg_mode & 0xE0) == 0xE0) {
            ret_mode = VDG_G6C;
        }
        else {
            ret_mode = VDG_SG24;
        }
        break;
    }

    return ret_mode;
}

void vdg_init() {
    e_cpu_context.vdg_state.video_buf = NULL;
    e_cpu_context.vdg_state.video_buf = (uint32*) malloc(SCR_BUF_X *
                                                         SCR_BUF_Y * 4);
    if (e_cpu_context.vdg_state.video_buf == NULL) {
        assert(FALSE);
        return;
    }
    else {
        /* Clear out the memory to since consecutive core_init() calls may
           resurface prior core memory */
        for (int i = 0; i < SCR_BUF_X * SCR_BUF_Y; i++) {
            e_cpu_context.vdg_state.video_buf[i] = 0;
        }
    }
}

void vdg_destroy() {
    if (e_cpu_context.vdg_state.video_buf) {
        free(e_cpu_context.vdg_state.video_buf);
    }
    e_cpu_context.vdg_state.video_buf = NULL;
}
