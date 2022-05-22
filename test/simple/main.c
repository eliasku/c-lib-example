#include <mylib/mylib.h>
#include <mylib/bitset.h>
#include <stdlib.h>

#define UNIT_IMPL
#include "../unit.h"

suite(mylib) {
    it("has process function") {
        const uint32_t r = mylib_process("test string");
        //echo("result: %u", r);
        warn_ne(r, 0, is random value);
    }
}
