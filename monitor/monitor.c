#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "headers.h"
#include "tests/test.h"

extern struct cpu_state e_cpu_context;
extern struct opcode_def opcode_table[];
void write_png(char *file_name);

void disassemble_pc(char* out_buf) {
    uint8 cur_pc_byte = read_byte_from_memory(get_reg_value_16(REG_PC));
    struct opcode_def od = opcode_table[cur_pc_byte];
    char* opcode = od.instruction;
    switch (od.mode) {
    case IMMEDIATE:
        sprintf(out_buf, "%s", opcode);
        break;
    case DIRECT:
        sprintf(out_buf, "%s", opcode);
        break;
    case INDEXED:
        sprintf(out_buf, "%s", opcode);
        break;
    case EXTENDED:
        sprintf(out_buf, "%s", opcode);
        break;
    case INHERENT:
        sprintf(out_buf, "%s", opcode);
        break;
    }
}

void print_state() {
    printf("A: $%.2X  B: $%.2X  D: $%.4X  X: $%.4X  Y: $%.4X  DP: $%.2X  U: $%.4X  S: $%.4X  PC: $%.4X  ",
           get_reg_value_8(REG_A), get_reg_value_8(REG_B), get_reg_value_16(REG_D),
           get_reg_value_16(REG_X), get_reg_value_16(REG_Y), get_reg_value_8(REG_DP),
           get_reg_value_16(REG_U), get_reg_value_16(REG_S), get_reg_value_16(REG_PC));
    printf("|  CC: $%.2X (e:%d f:%d h:%d i:%d n:%d z:%d v:%d c:%d)\n", get_reg_value_8(REG_CC),
           e_cpu_context.cc.e, e_cpu_context.cc.f, e_cpu_context.cc.h, e_cpu_context.cc.i,
           e_cpu_context.cc.n, e_cpu_context.cc.z, e_cpu_context.cc.v, e_cpu_context.cc.c);
    uint8 cur_pc_byte = read_byte_from_memory(get_reg_value_16(REG_PC));
    char dis_op[80];
    disassemble_pc(dis_op);
    printf("  memory[PC]: $%.2X (%s)  |  total cycles: %d, hsync cycles: %d, vsync cycles: %d)  ", cur_pc_byte,
           dis_op, e_cpu_context.cycle_count,
           e_cpu_context.hsync_cycles, e_cpu_context.vsync_cycles);
    printf("|  nmi: %d  firq: %d  irq: %d  halted state: %d\n", e_cpu_context.nmi,
           e_cpu_context.firq, e_cpu_context.irq, e_cpu_context.halted_state);
}

void print_prompt() {
    printf("> ");
}

void print_memory(uint16 root_address) {
    uint8 num_lines = 16;
    uint16 cur_address = root_address;

    for (int i=0; i<num_lines; i++) {
        uint16 line_address = cur_address;
        printf("$%.4hX:  ", line_address);
        for (int j=0; j<16; j++) {
            printf("%.2hX ", (uint16) read_byte_from_memory(line_address + j));
            if (cur_address == 0xFFFF) {
                break;
            }
            else {
                cur_address++;
            }
        }

        printf(" |  ");
        cur_address = line_address;
        for (int k=0; k<16; k++) {
            printf("%c ", read_byte_from_memory(line_address + k));
            if (cur_address == 0xFFFF) {
                break;
            }
            else {
                cur_address++;
            }
        }

        printf("\n");
        if (cur_address == 0xFFFF) {
            break;
        }
    }
}

void vdg_dump() {
    /* Dump the frame buffer to a raw binary file as well as png */
    const char* video_file = "/Users/simon/Dropbox/Programming/c/6809_core/picture.bin";
    FILE* handle = fopen(video_file, "w");
    if (!handle) {
        assert(FALSE);
        return;
    }

    size_t num_objects = fwrite((const void *) e_cpu_context.vdg_state.video_buf,
                                1, SCR_BUF_Y * SCR_BUF_X * 4, handle);
    if (!num_objects || num_objects < SCR_BUF_Y * SCR_BUF_X) {
        assert(FALSE);
    }

    fclose(handle);

    char* png_file = "/Users/simon/Dropbox/Programming/c/6809_core/picture.png";
    write_png(png_file);
}

int process_command() {
    size_t command_cap = 80;
    char* command = (char*) malloc(command_cap);
    getline(&command, &command_cap, stdin);
    /* printf("Command: %s", command); */
    int ret_val = 1;

    if (strncmp(command, "q", 1) == 0) {
        ret_val = 0;
    }
    else if (strncmp(command, "s", 1) == 0) {
        run_cycles(1);
    }
    else if (strncmp(command, "r", 1) == 0) {
        run_cycles(0xFFFFFFFF);
    }
    else if (strncmp(command, "x", 1) == 0) {
        uint16 arg1 = 0;
        int nTok = sscanf(command, "x %4hX", &arg1);
        if (nTok < 1) {
            printf("x (examine memory) usage: \"x 0400\"\n");
        }
        else {
            print_memory(arg1);
        }
    }
    else if (strncmp(command, "v", 1) == 0) {
        vdg_update();
        vdg_dump();
        printf("Dumped video buffer to file\n");
    }
    else {
        printf("Unrecognized command. Type 'h' for help.\n");
    }

    return ret_val;
}

void loop() {
    while (TRUE) {
        print_state();
        print_prompt();
        if (!process_command()) {
            core_destroy();
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    char* program_filename = argv[1];
    printf("Coco Monitor. Type 'h' for help.\nDECB file: %s\n", program_filename);

    core_init();
    load_roms();
    vdg_init();

    e_cpu_context.swi_hook = 1;

    uint16 preambles = init_from_decb_file(program_filename);
    printf("Loaded %d preambles.\n\n", preambles);

    loop();
    return 0;
}
