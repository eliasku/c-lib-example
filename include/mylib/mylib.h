#ifndef MYLIB_H
#define MYLIB_H

#include "common.h"
#include <stdint.h>

MYLIB_BEGIN_C_DECLS

uint32_t mylib_init(void);
uint32_t mylib_shutdown(void);
uint32_t mylib_process(const char* text);

MYLIB_END_C_DECLS

#endif // MYLIB_H
