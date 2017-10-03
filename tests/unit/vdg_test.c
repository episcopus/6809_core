#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>

#include "consts.h"
#include "types.h"
#include "pia.h"
#include "memory.h"
#include "core.h"
#include "vdg.h"
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
