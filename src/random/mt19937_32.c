#include "cutils/random/mt19937_32.h"
#define UIntType uint32_t
#define MTStruct struct mt19937_32
#define MT_W 32
#define MT_N 624
#define MT_M 397
#define MT_R 31
#define MT_A 0x9908b0df
#define MT_U 11
#define MT_D 0xffffffff
#define MT_S 7
#define MT_B 0x9d2c5680
#define MT_T 15
#define MT_C 0xefc60000
#define MT_L 18
#define MT_F 1812433253
#define MT_W_MASK 0xffffffff
#define MT_LOWER_MASK ((1ul << MT_R) - 1)
#define MT_UPPER_MASK ((~MT_LOWER_MASK) & MT_W_MASK)
#include "mersenne_twister_template.h"

void mt19937_init32(struct mt19937_32* st, uint32_t seed) {
    mersenne_twister_init(st, seed);
}

uint32_t mt19937_next32(struct mt19937_32* st) {
    return mersenne_twister_extract_number(st);
}
