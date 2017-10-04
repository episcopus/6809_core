#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>

#include "headers.h"
#include "tests/test.h"

extern struct cpu_state e_cpu_context;

void get_vdg_mode_from_sam_test(void **state) {
    (void) state; /* unused */

    e_cpu_context.sam_state.v0_control_bit = 1;
    e_cpu_context.sam_state.v1_control_bit = 0;
    e_cpu_context.sam_state.v2_control_bit = 0;

    assert_int_equal(get_vdg_mode_from_sam(), 0x1);
}

void get_vdg_mode_from_sam_2_test(void **state) {
    (void) state; /* unused */

    e_cpu_context.sam_state.v0_control_bit = 1;
    e_cpu_context.sam_state.v1_control_bit = 0;
    e_cpu_context.sam_state.v2_control_bit = 1;

    assert_int_equal(get_vdg_mode_from_sam(), 0x5);
}

void get_video_starting_address_from_sam_test(void **state) {
    (void) state; /* unused */

    e_cpu_context.sam_state.f0_control_bit = 1;
    e_cpu_context.sam_state.f1_control_bit = 0;
    e_cpu_context.sam_state.f2_control_bit = 1;
    e_cpu_context.sam_state.f3_control_bit = 0;
    e_cpu_context.sam_state.f4_control_bit = 0;
    e_cpu_context.sam_state.f5_control_bit = 0;
    e_cpu_context.sam_state.f6_control_bit = 0;

    assert_int_equal(get_video_starting_address_from_sam(), 0xA00);
}

void get_video_starting_address_from_sam_2_test(void **state) {
    (void) state; /* unused */

    e_cpu_context.sam_state.f0_control_bit = 0;
    e_cpu_context.sam_state.f1_control_bit = 0;
    e_cpu_context.sam_state.f2_control_bit = 0;
    e_cpu_context.sam_state.f3_control_bit = 1;
    e_cpu_context.sam_state.f4_control_bit = 1;
    e_cpu_context.sam_state.f5_control_bit = 0;
    e_cpu_context.sam_state.f6_control_bit = 1;

    assert_int_equal(get_video_starting_address_from_sam(), 0xB000);
}

void get_vdg_mode_1_test(void **state) {
    (void) state; /* unused */

    e_cpu_context.sam_state.v0_control_bit = 0;
    e_cpu_context.sam_state.v1_control_bit = 0;
    e_cpu_context.sam_state.v2_control_bit = 0;

    e_cpu_context.pia_state.ddr_2_b = 0;

    assert_int_equal(get_vdg_mode(), VDG_AI);
}

void get_vdg_mode_2_test(void **state) {
    (void) state; /* unused */

    e_cpu_context.sam_state.v0_control_bit = 0;
    e_cpu_context.sam_state.v1_control_bit = 0;
    e_cpu_context.sam_state.v2_control_bit = 0;

    e_cpu_context.pia_state.ddr_2_b = 0x18;

    assert_int_equal(get_vdg_mode(), VDG_SG6);
}

void get_vdg_mode_3_test(void **state) {
    (void) state; /* unused */

    e_cpu_context.sam_state.v0_control_bit = 0;
    e_cpu_context.sam_state.v1_control_bit = 1;
    e_cpu_context.sam_state.v2_control_bit = 1;

    e_cpu_context.pia_state.ddr_2_b = 0xF8;

    assert_int_equal(get_vdg_mode(), VDG_G6R);
}

void get_vdg_mode_4_test(void **state) {
    (void) state; /* unused */

    e_cpu_context.sam_state.v0_control_bit = 0;
    e_cpu_context.sam_state.v1_control_bit = 1;
    e_cpu_context.sam_state.v2_control_bit = 1;

    e_cpu_context.pia_state.ddr_2_b = 0xE8;

    assert_int_equal(get_vdg_mode(), VDG_G6C);
}

void vdg_init_test(void **state) {
    (void) state; /* unused */

    assert_int_equal(e_cpu_context.vdg_state.video_buf, NULL);
    vdg_init();
    assert_int_not_equal(e_cpu_context.vdg_state.video_buf, NULL);
    assert_int_equal(e_cpu_context.vdg_state.video_buf[0], 0);
    vdg_destroy();
    assert_int_equal(e_cpu_context.vdg_state.video_buf, NULL);
}

void vdg_update_test(void **state) {
    (void) state; /* unused */

    vdg_init();
    vdg_update();

    assert_int_not_equal(e_cpu_context.vdg_state.video_buf, NULL);

    /* Assume default AI video mode, go look for border color and background
       color */
    uint32 border_color = VDG_BLACK;
    uint16 root_border_address = 0;
    assert_int_equal(e_cpu_context.vdg_state.video_buf[root_border_address], VDG_R_BYTE(border_color));
    assert_int_equal(e_cpu_context.vdg_state.video_buf[root_border_address + 1], VDG_G_BYTE(border_color));
    assert_int_equal(e_cpu_context.vdg_state.video_buf[root_border_address + 2], VDG_B_BYTE(border_color));
    assert_int_equal(e_cpu_context.vdg_state.video_buf[root_border_address + 3], VDG_A_BYTE(border_color));

    uint16 root_bg_address = (SCR_BUF_X * ACTIVE_BUF_OFFSET * 4) + ACTIVE_BUF_OFFSET * 4;
    uint32 bg_color = VDG_GREEN;
    assert_int_equal(e_cpu_context.vdg_state.video_buf[root_bg_address], VDG_R_BYTE(bg_color));
    assert_int_equal(e_cpu_context.vdg_state.video_buf[root_bg_address + 1], VDG_G_BYTE(bg_color));
    assert_int_equal(e_cpu_context.vdg_state.video_buf[root_bg_address + 2], VDG_B_BYTE(bg_color));
    assert_int_equal(e_cpu_context.vdg_state.video_buf[root_bg_address + 3], VDG_A_BYTE(bg_color));
}

void vdg_update_g1c_test(void **state) {
    (void) state; /* unused */

    vdg_init();
    /* Initialize G1C mode w/ white border */
    e_cpu_context.sam_state.v0_control_bit = 1;
    e_cpu_context.sam_state.v1_control_bit = 0;
    e_cpu_context.sam_state.v2_control_bit = 0;

    uint8 pia_vdg_mode = e_cpu_context.pia_state.ddr_2_b;
    pia_vdg_mode |= 0x08;
    e_cpu_context.pia_state.ddr_2_b = pia_vdg_mode;

    vdg_update();

    assert_int_not_equal(e_cpu_context.vdg_state.video_buf, NULL);

    /* Assume default AI video mode, go look for border color and background
       color */
    uint32 border_color = VDG_WHITE;
    uint16 root_border_address = 0;
    assert_int_equal(e_cpu_context.vdg_state.video_buf[root_border_address], VDG_R_BYTE(border_color));
    assert_int_equal(e_cpu_context.vdg_state.video_buf[root_border_address + 1], VDG_G_BYTE(border_color));
    assert_int_equal(e_cpu_context.vdg_state.video_buf[root_border_address + 2], VDG_B_BYTE(border_color));
    assert_int_equal(e_cpu_context.vdg_state.video_buf[root_border_address + 3], VDG_A_BYTE(border_color));

    uint16 root_bg_address = (SCR_BUF_X * ACTIVE_BUF_OFFSET * 4) + ACTIVE_BUF_OFFSET * 4;
    uint32 bg_color = VDG_GREEN;
    assert_int_equal(e_cpu_context.vdg_state.video_buf[root_bg_address], VDG_R_BYTE(bg_color));
    assert_int_equal(e_cpu_context.vdg_state.video_buf[root_bg_address + 1], VDG_G_BYTE(bg_color));
    assert_int_equal(e_cpu_context.vdg_state.video_buf[root_bg_address + 2], VDG_B_BYTE(bg_color));
    assert_int_equal(e_cpu_context.vdg_state.video_buf[root_bg_address + 3], VDG_A_BYTE(bg_color));
}
