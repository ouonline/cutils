#include "cutils/random/xorshift32.h"

void xorshift32_init(struct xorshift32* st, uint32_t seed) {
    if (seed == 0) {
        seed = ((unsigned long)st) & 0xffffffff;
    }
    st->s = seed;
}

uint32_t xorshift32_next(struct xorshift32* st) {
    uint32_t x = st->s;
    x ^= (x << 13);
    x ^= (x >> 17);
    x ^= (x << 5);
    st->s = x;
    return x;
}
