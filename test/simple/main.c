#include <mylib/mylib.h>
#include <mylib/bitset.h>
#include <stdio.h>
#include <stdlib.h>
#include "../mytest.h"

TEST(bitset, main) {
    uint64_t* bitset = (uint64_t*) calloc(bitset_byte_size(1000000), 1);
    if (!bitset) {
        puts("ERROR\n");
        return;
    }
    {
        TEST_EQ(bitset_index(65), 1);
        TEST_EQ(bitset_mask(65), 2);
    }
    {
        // flip bit
        bitset_flip(bitset, 64 + 31);
        TEST_EQ(bitset[1], 0x80000000);
        // flip back
        bitset_flip(bitset, 64 + 31);
        TEST_EQ(bitset[1], 0);
    }
    {
        bitset_set(bitset, 111);
        bool b = bitset_get(bitset, 111);
        TEST_EQ(b, 1);
        bitset_unset(bitset, 111);
        b = bitset_get(bitset, 111);
        TEST_EQ(b, 0);
    }
    free(bitset);
}

TEST(mylib, process) {
    const uint32_t r = mylib_process("test string");
    printf("result: %u\n", r);
}
