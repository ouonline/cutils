#include "cutils/random/mt19937_64.h"
#define UIntType uint64_t
#define MTStruct struct mt19937_64
#define MT_W 64
#define MT_N 312
#define MT_M 156
#define MT_R 31
#define MT_A 0xb5026f5aa96619e9
#define MT_U 29
#define MT_D 0x5555555555555555
#define MT_S 17
#define MT_B 0x71d67fffeda60000
#define MT_T 37
#define MT_C 0xfff7eee000000000
#define MT_L 43
#define MT_F 6364136223846793005
#define MT_W_MASK 0xffffffffffffffff
#define MT_LOWER_MASK ((1ull << MT_R) - 1)
#define MT_UPPER_MASK ((~MT_LOWER_MASK) & MT_W_MASK)
#include "mersenne_twister_template.h"

void mt19937_init64(struct mt19937_64* st, uint64_t seed) {
    mersenne_twister_init(st, seed);
}

uint64_t mt19937_next64(struct mt19937_64* st) {
    return mersenne_twister_extract_number(st);
}
