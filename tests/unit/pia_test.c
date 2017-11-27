#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>

#include "headers.h"
#include "tests/test.h"

extern struct cpu_state e_cpu_context;

void pia_basic_test(void **state) {
    (void) state; /* unused */

    /* Enable data direction register for PIA 1B */
    uint8 crb = coco_read_byte_from_memory(0xFF03);
    crb = crb & 0xFB;
    coco_write_byte_to_memory(0xFF03, crb);

    /* Set all DRB bits for output */
    coco_write_byte_to_memory(0xFF02, 0xFF);

    /* Turn data direction register back off */
    crb = coco_read_byte_from_memory(0xFF03);
    crb = crb | 0x4;
    coco_write_byte_to_memory(0xFF03, crb);

    /* Write some byte to the data register */
    coco_write_byte_to_memory(0xFF02, 0x69);

    /* Now check the values first the data direction */
    crb = coco_read_byte_from_memory(0xFF03);
    crb = crb & 0xFB;
    coco_write_byte_to_memory(0xFF03, crb);
    uint8 dddr = coco_read_byte_from_memory(0xFF02);
    assert_int_equal(dddr, 0xFF);

    /* Now check the data register */
    crb = coco_read_byte_from_memory(0xFF03);
    crb = crb | 0x4;
    coco_write_byte_to_memory(0xFF03, crb);
    uint8 ddr = coco_read_byte_from_memory(0xFF02);
    assert_int_equal(ddr, 0x69);
}

void pia_update_keyboard_test(void **state) {
    (void) state; /* unused */

    /* Initialize PIA1B to output, and PIA1A to input,
       in order to simulate keyboard strobing */
    pia_write_byte_to_memory(0xFF01, 0);
    pia_write_byte_to_memory(0xFF00, 0);
    pia_write_byte_to_memory(0xFF03, 0);
    pia_write_byte_to_memory(0xFF02, 0xFF);

    /* Put both PIA1 sides back in DDR mode */
    pia_write_byte_to_memory(0xFF01, 0x4);
    pia_write_byte_to_memory(0xFF03, 0x4);

    /* Strobe column 4 */
    pia_write_byte_to_memory(0xFF02, 0xEF);
    /* Have host 'T' pushed down */
    e_cpu_context.pia_state.host_keys[PIA_KEY_T] = 1;
    /* Have the update_keyboard do its thing */
    pia_update_keyboard();

    /* Read active rows */
    uint8 pia_byte = coco_read_byte_from_memory(0xFF00);
    /* strip joystick bit */
    pia_byte &= 0x7F;

    /* This is bit 2 low, which is 'T' in keyboard matrix */
    assert_int_equal(pia_byte, 0x7B);
}

void pia_update_keyboard_no_key_test(void **state) {
    (void) state; /* unused */

    /* Initialize PIA1B to output, and PIA1A to input,
       in order to simulate keyboard strobing */
    pia_write_byte_to_memory(0xFF01, 0);
    pia_write_byte_to_memory(0xFF00, 0);
    pia_write_byte_to_memory(0xFF03, 0);
    pia_write_byte_to_memory(0xFF02, 0xFF);

    /* Put both PIA1 sides back in DDR mode */
    pia_write_byte_to_memory(0xFF01, 0x4);
    pia_write_byte_to_memory(0xFF03, 0x4);

    /* Strobe column 4 */
    pia_write_byte_to_memory(0xFF02, 0xEF);
    /* Have the update_keyboard do its thing */
    pia_update_keyboard();

    /* Read active rows */
    uint8 pia_byte = coco_read_byte_from_memory(0xFF00);
    /* strip joystick bit */
    pia_byte &= 0x7F;

    /* No bits are low meaning no keys pushed in that column */
    assert_int_equal(pia_byte, 0x7F);
}

void pia_update_keyboard_no_strobe_or_key_test(void **state) {
    (void) state; /* unused */

    /* Initialize PIA1B to output, and PIA1A to input,
       in order to simulate keyboard strobing */
    pia_write_byte_to_memory(0xFF01, 0);
    pia_write_byte_to_memory(0xFF00, 0);
    pia_write_byte_to_memory(0xFF03, 0);
    pia_write_byte_to_memory(0xFF02, 0xFF);

    /* Put both PIA1 sides back in DDR mode */
    pia_write_byte_to_memory(0xFF01, 0x4);
    pia_write_byte_to_memory(0xFF03, 0x4);

    /* Have the update_keyboard do its thing */
    pia_update_keyboard();

    /* Read active rows */
    uint8 pia_byte = coco_read_byte_from_memory(0xFF00);
    /* strip joystick bit */
    pia_byte &= 0x7F;

    /* No bits are low meaning no keys pushed in that column */
    assert_int_equal(pia_byte, 0x7F);
}
