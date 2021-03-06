#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>

#include "headers.h"
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

    coco_write_byte_to_memory(0xFF58, 0x69);

    assert_int_equal(coco_read_byte_from_memory(0xFF58),
                     0x69);
    assert_int_equal(coco_read_byte_from_memory(0xFF48),
                     0x0);
}

void sam_redirected_byte_test(void **state) {
    (void) state; /* unused */

    /* Redirects are followed when not in memory map mode */
    e_cpu_context.sam_state.ty_control_bit = 0;

    e_cpu_context.memory[0xFF48] = 0x69;

    assert_int_equal(coco_read_byte_from_memory(0xFF58),
                     0x69);
    assert_int_equal(coco_read_byte_from_memory(0xFF48),
                     0x69);
}

void sam_irq_redirected_byte_test(void **state) {
    (void) state; /* unused */

    /* Redirects for the IRQ vector are unconditionnal */
    e_cpu_context.sam_state.ty_control_bit = 0;

    /* Writes not normally allowed because of ROM when !ty */
    e_cpu_context.memory[0xBFF2] = 0x69;

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

void sam_page_number_test(void **state) {
    (void) state; /* unused */

    /* Tests out default mode where there is no page switching */
    e_cpu_context.sam_state.p1_control_bit = 0;
    /* RAM/ROM memory map mode */
    e_cpu_context.sam_state.ty_control_bit = 0;
    write_byte_to_memory(0x1000, 0x69);
    assert_int_equal(read_byte_from_memory(0x1000), 0x69);
    assert_int_equal(e_cpu_context.memory[0x1000], 0x69);
    assert_int_equal(read_byte_from_memory(0x9000), 0x0);
    assert_int_equal(e_cpu_context.memory[0x9000], 0x0);

    /* Do not mess with the upper addresses when memory map mode is 0 */
    /* write_byte_to_memory(0xA000, 0x79); */
    /* assert_int_equal(read_byte_from_memory(0xA000), 0x79); */
    /* assert_int_equal(e_cpu_context.memory[0xA000], 0x79); */
    /* assert_int_equal(read_byte_from_memory(0x2000), 0x0); */
    /* assert_int_equal(e_cpu_context.memory[0x2000], 0x0); */
}

void sam_page_number_page_flip_test(void **state) {
    (void) state; /* unused */

    /* Now tests the page flip */
    e_cpu_context.sam_state.p1_control_bit = 1;
    /* RAM/ROM memory map mode */
    e_cpu_context.sam_state.ty_control_bit = 0;

    write_byte_to_memory(0x1000, 0x69);

    /* read_byte follows the redirection */
    assert_int_equal(read_byte_from_memory(0x1000), 0x69);
    assert_int_equal(e_cpu_context.memory[0x1000], 0x0);
    assert_int_equal(e_cpu_context.memory[0x9000], 0x69);

    /* Do not mess with the upper addresses when memory map mode is 0 */
    /* write_byte_to_memory(0xA000, 0x79); */
    /* assert_int_equal(read_byte_from_memory(0xA000), 0x79); */
    /* assert_int_equal(e_cpu_context.memory[0x2000], 0x79); */
}

void sam_page_number_word_test(void **state) {
    (void) state; /* unused */

    /* Tests out default mode where there is no page switching */
    e_cpu_context.sam_state.p1_control_bit = 0;
    /* RAM/ROM memory map mode */
    e_cpu_context.sam_state.ty_control_bit = 0;
    write_word_to_memory(0x1000, 0x69AB);

    assert_int_equal(read_word_from_memory(0x1000), 0x69AB);
    assert_int_equal(e_cpu_context.memory[0x1000] << 8 |
                     e_cpu_context.memory[0x1001], 0x69AB);
    assert_int_equal(read_word_from_memory(0x9000), 0x0);
    assert_int_equal(e_cpu_context.memory[0x9000] << 8 |
                     e_cpu_context.memory[0x9001], 0x0);

    /* Do not mess with the upper addresses when memory map mode is 0 */
    /* write_word_to_memory(0xA000, 0x79); */
    /* assert_int_equal(read_word_from_memory(0xA000), 0x79); */
    /* assert_int_equal(e_cpu_context.memory[0xA000], 0x79); */
    /* assert_int_equal(read_word_from_memory(0x2000), 0x0); */
    /* assert_int_equal(e_cpu_context.memory[0x2000], 0x0); */
}

void sam_page_number_page_flip_word_test(void **state){
    (void) state; /* unused */

    /* Now tests the page flip */
    e_cpu_context.sam_state.p1_control_bit = 1;
    /* RAM/ROM memory map mode */
    e_cpu_context.sam_state.ty_control_bit = 0;

    write_word_to_memory(0x1000, 0x69AB);

    /* read_word follows the redirection */
    assert_int_equal(read_word_from_memory(0x1000), 0x69AB);
    assert_int_equal(e_cpu_context.memory[0x1000] << 8 |
                     e_cpu_context.memory[0x1001], 0x0);
    assert_int_equal(e_cpu_context.memory[0x9000] << 8 |
                     e_cpu_context.memory[0x9001], 0x69AB);

    /* Do not mess with the upper addresses when memory map mode is 0 */
    /* write_word_to_memory(0xA000, 0x79); */
    /* assert_int_equal(read_word_from_memory(0xA000), 0x79); */
    /* assert_int_equal(e_cpu_context.memory[0x2000], 0x79); */
}

void sam_ty_true_no_basic_test(void **state) {
    (void) state; /* unused */

    load_roms();

    /* RAM / ROM mode and normal page order */
    e_cpu_context.sam_state.ty_control_bit = 1;
    e_cpu_context.sam_state.p1_control_bit = 0;

    /* For this test we are going to 64K mode (ty==1) so we don't expect to read
       the Basic ROM bytes */
    assert_int_equal(coco_read_byte_from_memory(0xA000), 0);
    assert_int_equal(coco_read_byte_from_memory(0xA001), 0);
    /* Default RESET vector set up in core_init() */
    assert_int_equal(coco_read_byte_from_memory(0xBFFE), 0xA0);
    assert_int_equal(coco_read_byte_from_memory(0xBFFF), 0x27);

    /* No extended basic ROM either */
    assert_int_equal(coco_read_byte_from_memory(0x8000), 0);
    assert_int_equal(coco_read_byte_from_memory(0x8001), 0);
    assert_int_equal(coco_read_byte_from_memory(0x9FFE), 0);
    assert_int_equal(coco_read_byte_from_memory(0x9FFF), 0);
}
