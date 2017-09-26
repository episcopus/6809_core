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
