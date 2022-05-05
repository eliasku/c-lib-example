#include <mylib/mylib.h>
#include <assert.h>

int main(void) {
    uint32_t r = mylib_init();
    assert(r == 0);
    r = mylib_shutdown();
    assert(r == 0);
    return 0;
}
