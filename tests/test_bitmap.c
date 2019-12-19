#include <stdio.h>
#include "bitmap.h"

#define N 3

static inline void test(unsigned int offset) {
    printf("------------ offset[%u] ------------\n", offset);
    unsigned char buf[N] = {0};

    bitmap_set_r2l(buf, N, offset);
    int i = 0;
    for (i = 0; i < N; ++i) {
        printf("buf[%d] = %x\n", i, buf[i]);
    }

    if (bitmap_test_r2l(buf, N, offset)) {
        printf("%u is set\n", offset);
    } else {
        printf("%u is not set\n", offset);
    }

    bitmap_clear_r2l(buf, N, offset);

    if (bitmap_test_r2l(buf, N, offset)) {
        printf("%u is set\n", offset);
    } else {
        printf("%u is not set\n", offset);
    }
}

int main(void) {
    test(0);
    test(1);
    test(7);
    test(8);
    test(10);
    test(23);
    return 0;
}
