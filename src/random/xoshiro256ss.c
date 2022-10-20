#include "cutils/random/xoshiro256ss.h"
#include "cutils/random/splitmix64.h"

void xoshiro256ss_init(struct xoshiro256ss* st, uint64_t seed) {
    struct splitmix64 sm;
    splitmix64_init(&sm, seed);
    st->s[0] = splitmix64_next(&sm);
    st->s[1] = splitmix64_next(&sm);
    st->s[2] = splitmix64_next(&sm);
    st->s[3] = splitmix64_next(&sm);
}

static inline uint64_t rol64(uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
}

static inline uint64_t times9(uint64_t x) {
    return (x << 3) + x;
}

static inline uint64_t times5(uint64_t x) {
    return (x << 2) + x;
}

uint64_t xoshiro256ss_next(struct xoshiro256ss* st) {
    uint64_t* s = st->s;
    const uint64_t result = times9(rol64(times5(s[1]), 7));
    const uint64_t t = s[1] << 17;

    s[2] ^= s[0];
    s[3] ^= s[1];
    s[1] ^= s[2];
    s[0] ^= s[3];

    s[2] ^= t;
    s[3] = rol64(s[3], 45);

    return result;
}
