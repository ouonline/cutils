#ifndef __MATH_MERSENNE_TWISTER_TEMPLATE_H__
#define __MATH_MERSENNE_TWISTER_TEMPLATE_H__

static void mersenne_twister_init(MTStruct* st, UIntType seed) {
    st->index = 0;
    st->mt[0] = seed;

    uint32_t i;
    for (i = 1; i < MT_N; ++i) {
        UIntType prev = st->mt[i - 1];
        st->mt[i] = MT_F * (prev ^ (prev >> (MT_W - 2))) + i;
    }
}

static void mersenne_twister_twist(MTStruct* st) {
    uint32_t i;
    for (i = 0; i < MT_N; ++i) {
        UIntType x = (st->mt[i] & MT_UPPER_MASK) +
            (st->mt[(i + 1) % MT_N] & MT_LOWER_MASK);
        UIntType xA = (x >> 1);
        if (x & 1u) {
            xA = xA ^ MT_A;
        }
        st->mt[i] = st->mt[(i + MT_M) % MT_N] ^ xA;
    }
}

static UIntType mersenne_twister_extract_number(MTStruct* st) {
    if (st->index == 0) {
        mersenne_twister_twist(st);
    }

    UIntType y = st->mt[st->index];
    y = y ^ ((y >> MT_U) & MT_D);
    y = y ^ ((y << MT_S) & MT_B);
    y = y ^ ((y << MT_T) & MT_C);
    y = y ^ (y >> MT_L);

    st->index = (st->index + 1) % MT_N;
    return y;
}

#endif
