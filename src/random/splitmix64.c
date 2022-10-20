#include "cutils/random/splitmix64.h"

void splitmix64_init(struct splitmix64* st, uint64_t seed) {
    st->s = seed;
}

uint64_t splitmix64_next(struct splitmix64* st) {
    st->s += 0x9e3779b97f4a7c15;
    uint64_t result = st->s;
    result = (result ^ (result >> 30)) * 0xbf58476d1ce4e5b9;
    result = (result ^ (result >> 27)) * 0x94d049bb133111eb;
    return result ^ (result >> 31);
}
