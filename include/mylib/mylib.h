#ifndef MYLIB_H
#define MYLIB_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

uint32_t mylib_init(void);
uint32_t mylib_shutdown(void);
uint32_t mylib_process(const char* text);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MYLIB_H
