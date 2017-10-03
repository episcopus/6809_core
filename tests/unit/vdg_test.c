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

    /* RAM / ROM mode and normal page order */
    e_cpu_context.sam_state.v0_control_bit = 1;
    e_cpu_context.sam_state.v1_control_bit = 0;
    e_cpu_context.sam_state.v2_control_bit = 0;

    assert_int_equal(get_vdg_mode_from_sam(), 0x1);
}

void get_vdg_mode_from_sam_2_test(void **state) {
    (void) state; /* unused */

    /* RAM / ROM mode and normal page order */
    e_cpu_context.sam_state.v0_control_bit = 1;
    e_cpu_context.sam_state.v1_control_bit = 0;
    e_cpu_context.sam_state.v2_control_bit = 1;

    assert_int_equal(get_vdg_mode_from_sam(), 0x5);
}
