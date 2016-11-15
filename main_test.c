#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>

#include "test.h"

int main(void) {
    return cmocka_run_group_tests(tests, NULL, NULL);
}
