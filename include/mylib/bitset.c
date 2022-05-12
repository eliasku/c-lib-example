#include "bitset.h"

MYLIB_BEGIN_C_DECLS

uint32_t bitset_index(uint32_t position) {
    return position >> 6u; // div 64
}

uint32_t bitset_byte_size(uint32_t bits) {
    // add ((8 bytes * 8 bits) - 1) to find ceil size
    bits += (sizeof(uint64_t) << 3u) - 1u;
    // divide by 64
    bits >>= 6u;
    // multiply by `sizeof(uint64_t)` bytes (8 bytes per 1 word)
    bits <<= 3u;
    return bits;
}

uint64_t bitset_mask(uint32_t position) {
    return ((uint64_t) 1u) << (position & 63u);
}

bool bitset_get(const uint64_t* bitset, uint32_t position) {
    const uint32_t idx = bitset_index(position);
    const uint64_t mask = bitset_mask(position);
    return (bitset[idx] & mask) != 0;
}

void bitset_set(uint64_t* bitset, uint32_t position) {
    const uint32_t idx = bitset_index(position);
    const uint64_t mask = bitset_mask(position);
    bitset[idx] |= mask;
}

void bitset_unset(uint64_t* bitset, uint32_t position) {
    const uint32_t idx = bitset_index(position);
    const uint64_t mask = bitset_mask(position);
    bitset[idx] &= ~mask;
}

void bitset_flip(uint64_t* bitset, uint32_t position) {
    const uint32_t idx = bitset_index(position);
    const uint64_t mask = bitset_mask(position);
    bitset[idx] ^= mask;
}

MYLIB_END_C_DECLS
