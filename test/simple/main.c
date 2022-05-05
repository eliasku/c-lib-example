#include <mylib/mylib.h>
#include <stdio.h>

int main(void) {
//    mylib_init();
    const uint32_t r = mylib_process("test string");
    printf("result: %u\n", r);
//    mylib_shutdown();
    return 0;
}
