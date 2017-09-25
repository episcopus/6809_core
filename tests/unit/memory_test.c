#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>

#include "consts.h"
#include "types.h"
#include "memory.h"
#include "core.h"
#include "tests/test.h"

extern struct cpu_state e_cpu_context;

void sam_read_byte_test(void **state) {
    (void) state; /* unused */

    assert_int_equal(sam_read_byte_from_memory(0xFFDD),
                     e_cpu_context.sam_state.m1_control_bit);
    assert_int_equal(sam_read_byte_from_memory(0xFFDB),
                     e_cpu_context.sam_state.m0_control_bit);

    e_cpu_context.sam_state.m1_control_bit = 0;
    e_cpu_context.sam_state.m0_control_bit = 1;

    assert_int_equal(sam_read_byte_from_memory(0xFFDD),
                     0);
    assert_int_equal(sam_read_byte_from_memory(0xFFDB),
                     1);
}

void sam_write_byte_test(void **state) {
    (void) state; /* unused */

    assert_int_equal(e_cpu_context.sam_state.ty_control_bit, 0);
    sam_write_byte_to_memory(0xFFDF, 0x69);
    assert_int_equal(e_cpu_context.sam_state.ty_control_bit, 1);

    sam_write_byte_to_memory(0xFFDE, 0x69);
    assert_int_equal(e_cpu_context.sam_state.ty_control_bit, 0);
}

void sam_memory_handler_byte_from_core_test(void **state) {
    (void) state; /* unused */

    /* this goes through the main memory handler delegation process and
       should percolate down to SAM flags */
    assert_int_equal(e_cpu_context.sam_state.ty_control_bit, 0);
    write_byte_to_memory(0xFFDF, 0x69);
    assert_int_equal(e_cpu_context.sam_state.ty_control_bit, 1);
    assert_int_equal(read_byte_from_memory(0xFFDF), 1);

    write_byte_to_memory(0xFFDE, 0x69);
    assert_int_equal(e_cpu_context.sam_state.ty_control_bit, 0);
    assert_int_equal(read_byte_from_memory(0xFFDF), 0);
}

void sam_not_redirected_byte_test(void **state) {
    (void) state; /* unused */

    /* Redirects are not followed in memory map mode */
    e_cpu_context.sam_state.ty_control_bit = 1;

    coco_write_byte_to_memory(0xFF04, 0x69);

    assert_int_equal(coco_read_byte_from_memory(0xFF00),
                     0x00);
    assert_int_equal(coco_read_byte_from_memory(0xFF04),
                     0x69);
}

void sam_redirected_byte_test(void **state) {
    (void) state; /* unused */

    /* Redirects are followed when not in memory map mode */
    e_cpu_context.sam_state.ty_control_bit = 0;

    coco_write_byte_to_memory(0xFF04, 0x69);

    assert_int_equal(coco_read_byte_from_memory(0xFF00),
                     0x69);
    assert_int_equal(coco_read_byte_from_memory(0xFF04),
                     0x69);
}

void sam_irq_redirected_byte_test(void **state) {
    (void) state; /* unused */

    /* Redirects for the IRQ vector are unconditionnal */
    e_cpu_context.sam_state.ty_control_bit = 0;

    coco_write_byte_to_memory(0xFFF2, 0x69);

    assert_int_equal(coco_read_byte_from_memory(0xFFF2),
                     0x69);
    assert_int_equal(coco_read_byte_from_memory(0xBFF2),
                     0x69);
}

void sam_irq_redirected_byte_test_2(void **state) {
    (void) state; /* unused */

    /* Redirects for the IRQ vector are unconditionnal */
    e_cpu_context.sam_state.ty_control_bit = 1;

    coco_write_byte_to_memory(0xFFF2, 0x69);

    assert_int_equal(coco_read_byte_from_memory(0xFFF2),
                     0x69);
    assert_int_equal(coco_read_byte_from_memory(0xBFF2),
                     0x69);
}
