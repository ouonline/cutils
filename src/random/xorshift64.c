#include "cutils/random/xorshift64.h"

void xorshift64_init(struct xorshift64* st, uint64_t seed) {
    if (seed == 0) {
        seed = ((unsigned long)st) & 0xffffffffffffffff;
    }
    st->s = seed;
}

uint64_t xorshift64_next(struct xorshift64* st) {
    uint64_t x = st->s;
    x ^= (x << 13);
    x ^= (x >> 7);
    x ^= (x << 17);
    st->s = x;
    return x;
}
