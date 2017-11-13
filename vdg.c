#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "headers.h"
#include "charset.h"
#include "semi_graphics.h"

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
    e_cpu_context.vdg_state.video_buf = (uint8*) malloc(SCR_BUF_X *
                                                        SCR_BUF_Y * 4);
    if (e_cpu_context.vdg_state.video_buf == NULL) {
        assert(FALSE);
        return;
    }
    else {
        /* Clear out the memory to since consecutive core_init() calls may
           resurface prior core memory */
        for (int i = 0; i < SCR_BUF_X * SCR_BUF_Y * 4; i++) {
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

void vdg_update() {
    if (!e_cpu_context.vdg_state.video_buf) {
        assert(FALSE);
        return;
    }

    enum vdg_mode mode = get_vdg_mode();
    uint16 buf_addr = get_video_starting_address_from_sam();
    uint32 border_color = vdg_get_bg_color(mode);

    vdg_update_by_mode(mode, buf_addr, border_color);
}

uint32 vdg_get_bg_color(enum vdg_mode mode) {
    uint8 pia_vdg_mode = e_cpu_context.pia_state.ddr_2_b;
    uint8 c = pia_vdg_mode & 0x08;

    switch (mode) {
    case VDG_AI:
    case VDG_SG4:
    case VDG_SG6:
    case VDG_SG8:
    case VDG_SG12:
    case VDG_SG24:
        return VDG_BLACK;
    case VDG_G1C:
    case VDG_G1R:
        return c ? VDG_WHITE : VDG_GREEN;
    case VDG_G2C:
        return c ? VDG_BLACK : VDG_GREEN;
    case VDG_G2R:
    case VDG_G3C:
    case VDG_G3R:
    case VDG_G6C:
    case VDG_G6R:
        return c ? VDG_WHITE : VDG_GREEN;
    }
}

void vdg_buf_set_pixel(uint16 x, uint16 y, uint32 color) {
    e_cpu_context.vdg_state.video_buf[(SCR_BUF_X * y * 4) + x * 4] = (color & 0xFF000000) >> 24;
    e_cpu_context.vdg_state.video_buf[(SCR_BUF_X * y * 4) + x * 4 + 1] = (color & 0x00FF0000) >> 16;
    e_cpu_context.vdg_state.video_buf[(SCR_BUF_X * y * 4) + x * 4 + 2] = (color & 0x0000FF00) >> 8;
    e_cpu_context.vdg_state.video_buf[(SCR_BUF_X * y * 4) + x * 4 + 3] = (color & 0x000000FF);
}

void vdg_clear_buffer(uint32 border_color) {
    /* Paint border and blank screen */
    for (int i=0; i<SCR_BUF_Y; i++) {
        for (int j=0; j<SCR_BUF_X; j++) {
            uint32 color = VDG_GREEN;
            if (i < ACTIVE_BUF_OFFSET ||
                i >= VDG_BUF_Y + ACTIVE_BUF_OFFSET ||
                j < ACTIVE_BUF_OFFSET ||
                j >= VDG_BUF_X + ACTIVE_BUF_OFFSET) {
                color = border_color;
            }

            vdg_buf_set_pixel(j, i, color);
        }
    }
}

void vdg_update_ai_sg4_mode(uint16 buf_addr) {
    int VDG_AI_X = 32;
    int VDG_AI_Y = 16;

    int base_y = ACTIVE_BUF_OFFSET;
    int base_x = ACTIVE_BUF_OFFSET;

    uint32 fg_color = VDG_AI_INV;
    uint32 bg_color = VDG_GREEN;

    /* Walk memory and paint characters on screen */
    for (int i=0; i<VDG_AI_Y; i++) {
        for (int j=0; j<VDG_AI_X; j++) {
            uint8 character = read_byte_from_memory(buf_addr + VDG_AI_X * i + j);
            uint8 is_sg = character & 0x80;

            /* Draw the full character */
            for (int y=0; y<12; y++) {
                for (int x=7; x>=0; x--) {
                    if (is_sg) {
                        uint8 key = vdg_sg_characters[character & 0x7F][8 * y + 7 - x];
                        uint32 color = vdg_sg_color_key[key];
                        vdg_buf_set_pixel(base_x + 7 - x, base_y + y, color);
                    }
                    else {
                        uint8 set = vdg_ai_characters[character][y] & (1 << x);
                        uint32 color = set ? fg_color : bg_color;
                        vdg_buf_set_pixel(base_x + 7 - x, base_y + y, color);
                    }
                }
            }

            base_x += 8;
        }

        base_y += 12;
        base_x = ACTIVE_BUF_OFFSET;
    }
}

void vdg_update_by_mode(enum vdg_mode mode, uint16 buf_addr, uint32 border_color) {
    vdg_clear_buffer(border_color);

    switch(mode) {
    case VDG_AI:
    case VDG_SG4:
        vdg_update_ai_sg4_mode(buf_addr);
        return;
    case VDG_SG6:
    case VDG_SG8:
    case VDG_SG12:
    case VDG_SG24:
    case VDG_G1C:
    case VDG_G1R:
    case VDG_G2C:
    case VDG_G2R:
    case VDG_G3C:
    case VDG_G3R:
    case VDG_G6C:
    case VDG_G6R:
        assert(FALSE);
        return;
    }
}
