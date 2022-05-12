#include "mylib.h"

MYLIB_BEGIN_C_DECLS

uint32_t mylib_process(const char* text) {
    uint32_t r = 0;
    while(*text) {
        r += *text * 31u;
        ++text;
    }
    return r;
}

MYLIB_END_C_DECLS
