#include <mylib/mylib.h>
#include <mylib/bitset.h>
#include <stdlib.h>

#define UNIT_IMPL
#include "../unit.h"

suite(bitset) {
    it("calculates correct index and offset") {
        check_eq(bitset_index(65), 1);
        check_eq(bitset_mask(65), 2);
    }
    it("controls boolean elements"){
        uint64_t* bitset = (uint64_t*) calloc(bitset_byte_size(1000000), 1);
        require((void*)bitset);

        // flip bit
        bitset_flip(bitset, 64 + 31);
        check_eq(bitset[1], 0x80000000);

        // flip back
        bitset_flip(bitset, 64 + 31);
        check_eq(bitset[1], 0);

        bitset_set(bitset, 111);
        bool b = bitset_get(bitset, 111);

        check_eq(b, 1);
        bitset_unset(bitset, 111);
        b = bitset_get(bitset, 111);
        check_eq(b, 0);

        free(bitset);
    }
}

suite(mylib) {
    it("has process function") {
        const uint32_t r = mylib_process("test string");
        //echo("result: %u", r);
        warn_ne(r, 0, is random value);
    }
}
