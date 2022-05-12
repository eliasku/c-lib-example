#include "mylib.h"
#include <stdio.h>

MYLIB_BEGIN_C_DECLS

uint32_t mylib_init(void) {
    puts("my library initialize...\n");
    return 0;
}

uint32_t mylib_shutdown(void) {
    puts("my library shutdown...\n");
    return 0;
}

MYLIB_END_C_DECLS
