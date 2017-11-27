#include <stdio.h>

#include "headers.h"

extern struct cpu_state e_cpu_context;

uint8 pia_direction(uint16 control_register_address) {
    uint8 control_register =
        coco_read_byte_from_memory(control_register_address);

    /* 0 means the register is in data direction mode, otherwise
       it is in data register mode */
    uint8 return_pia_direction = control_register & 0x4;
    return return_pia_direction;
}

uint8 pia_read_byte_from_memory(uint16 address) {
    uint8 return_byte = 0;

    /* Master the control registers as memory reads, otherwise it is dependent
       on the state of the data direction register. */
    switch (address) {
    case 0xFF00:
        return_byte = (pia_direction(address + 1) ? e_cpu_context.pia_state.ddr_1_a :
                       e_cpu_context.pia_state.dddr_1_a);
        break;
    case 0xFF02:
        return_byte = (pia_direction(address + 1) ? e_cpu_context.pia_state.ddr_1_b :
                       e_cpu_context.pia_state.dddr_1_b);
        break;
    case 0xFF20:
        return_byte = (pia_direction(address + 1) ? e_cpu_context.pia_state.ddr_2_a :
                       e_cpu_context.pia_state.dddr_2_a);
        break;
    case 0xFF22:
        return_byte = (pia_direction(address + 1) ? e_cpu_context.pia_state.ddr_2_b :
                       e_cpu_context.pia_state.dddr_2_b);
        break;
    case 0xFF01:
        return_byte = e_cpu_context.pia_state.cr_1_a;
        break;
    case 0xFF03:
        return_byte = e_cpu_context.pia_state.cr_1_b;
        break;
    case 0xFF21:
        return_byte = e_cpu_context.pia_state.cr_2_a;
        break;
    case 0xFF23:
        return_byte = e_cpu_context.pia_state.cr_2_b;
        break;
    }

    /* TODO: Some reads will have side effects, i.e. clearing the IRQ bits. */
    return return_byte;
}

void pia_write_byte_to_memory(uint16 address, uint8 byte) {
    /* Master the control registers as memory reads, otherwise it is dependent
       on the state of the data direction register. */
    switch (address) {
    case 0xFF00:
        if (pia_direction(address + 1)) {
            e_cpu_context.pia_state.ddr_1_a = byte;
        }
        else {
            e_cpu_context.pia_state.dddr_1_a = byte;
        }
        break;
    case 0xFF02:
        if (pia_direction(address + 1)) {
            e_cpu_context.pia_state.ddr_1_b = byte;
        }
        else {
            e_cpu_context.pia_state.dddr_1_b = byte;
        }
        break;
    case 0xFF20:
        if (pia_direction(address + 1)) {
            e_cpu_context.pia_state.ddr_2_a = byte;
        }
        else {
            e_cpu_context.pia_state.dddr_2_a = byte;
        }
        break;
    case 0xFF22:
        if (pia_direction(address + 1)) {
            e_cpu_context.pia_state.ddr_2_b = byte;
        }
        else {
            e_cpu_context.pia_state.dddr_2_b = byte;
        }
        break;
    case 0xFF01:
        e_cpu_context.pia_state.cr_1_a = byte;
        break;
    case 0xFF03:
        e_cpu_context.pia_state.cr_1_b = byte;
        break;
    case 0xFF21:
        e_cpu_context.pia_state.cr_2_a = byte;
        break;
    case 0xFF23:
        e_cpu_context.pia_state.cr_2_b = byte;
        break;
    }
}

void pia_update_keyboard() {
    /* Update keyboard state based on host keys. Algorithm:
       Determine PIA direction strobe (control register) direction.
       Determine keyboard column being strobed.
       Walk through keys of said column clearing input bits for
       those keys that are down according to host.
       Update input data register. */

    /* TODO flip based on actual control register direction */
    uint8* output_ddr = &e_cpu_context.pia_state.ddr_1_b;
    uint8* input_ddr = &e_cpu_context.pia_state.ddr_1_a;

    /* 0 is set for down key, by default nothing will be down. */
    uint8 input_value = 0xFF;
    uint8 output_value = ~*output_ddr;

    for (uint8 iter_value = 0x1, less_sig_nibble = 0; iter_value != 0;
         iter_value <<= 1, less_sig_nibble++) {
        /* The iter_value column is being strobed (low), assess any keys within
           it */
        if (!(iter_value & output_value)) {
            /* Nothing to do for this column */
            continue;
        }

        for (uint8 most_sig_nibble = 0, inner_mask = 0x1; most_sig_nibble +
                 less_sig_nibble < PIA_KEYBOARD_SIZE;
             most_sig_nibble += 8, inner_mask <<= 1) {
            uint8 table_lookup = most_sig_nibble + less_sig_nibble;

            /* look up in host's key state */
            if (e_cpu_context.pia_state.host_keys[table_lookup]) {
                /* Clear corresponding PIA input bit */
                input_value ^= inner_mask;
            }
        }

        break;
    }

    /* Update input data register. */
    *input_ddr = input_value;
}
