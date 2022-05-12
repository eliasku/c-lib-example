#ifndef MYLIB_BITSET_H
#define MYLIB_BITSET_H

#include "common.h"
#include <stdint.h>
#include <stdbool.h>

MYLIB_BEGIN_C_DECLS

uint32_t bitset_index(uint32_t position);

uint32_t bitset_byte_size(uint32_t bits);

uint64_t bitset_mask(uint32_t position);

bool bitset_get(const uint64_t* bitset, uint32_t position);

void bitset_set(uint64_t* bitset, uint32_t position);

void bitset_unset(uint64_t* bitset, uint32_t position);

void bitset_flip(uint64_t* bitset, uint32_t position);

MYLIB_END_C_DECLS

#endif // MYLIB_BITSET_H
