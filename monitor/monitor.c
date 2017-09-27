#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "consts.h"
#include "types.h"
#include "pia.h"
#include "memory.h"
#include "core.h"
#include "tests/test.h"

int main(int argc, char* argv[]) {
    char* program_filename = argv[1];
    printf("Coco Monitor. Type 'h' for help.\nDECB file: %s\n", program_filename);

    core_init();

    uint16 preambles = init_from_decb_file(program_filename);
    printf("Loaded %d preambles.\n", preambles);

    return 0;
}
