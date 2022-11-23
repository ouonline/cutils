#include <stdio.h>
#include <stdlib.h>
#include "test_utils.h"
#include "cutils/time_utils.h"
#include "cutils/random/mt19937_32.h"
#include "cutils/random/mt19937_64.h"
#include "cutils/random/xorshift32.h"
#include "cutils/random/xorshift64.h"
#include "cutils/random/splitmix64.h"
#include "cutils/random/xoshiro256ss.h"

#define LOOPS 5555555

static void test_rand() {
    srand(31415926);

    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    for (int i = 0; i < LOOPS; ++i) {
        rand();
    }
    gettimeofday(&end, NULL);
    printf("rand() cost %f ms.\n", diff_time_usec(end, &begin) / 1000.0);
}

static void test_xorshift32() {
    struct xorshift32 st;
    xorshift32_init(&st, 31415926);

    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    for (int i = 0; i < LOOPS; ++i) {
        xorshift32_next(&st);
    }
    gettimeofday(&end, NULL);
    printf("xorshift32 cost %f ms.\n", diff_time_usec(end, &begin) / 1000.0);
}

static void test_xorshift64() {
    struct xorshift64 st;
    xorshift64_init(&st, 31415926);

    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    for (int i = 0; i < LOOPS; ++i) {
        xorshift64_next(&st);
    }
    gettimeofday(&end, NULL);
    printf("xorshift64 cost %f ms.\n", diff_time_usec(end, &begin) / 1000.0);
}

static void test_splitmix64() {
    struct splitmix64 st;
    splitmix64_init(&st, 31415926);

    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    for (int i = 0; i < LOOPS; ++i) {
        splitmix64_next(&st);
    }
    gettimeofday(&end, NULL);
    printf("splitmix64 cost %f ms.\n", diff_time_usec(end, &begin) / 1000.0);
}

static void test_xoshiro256ss() {
    struct xoshiro256ss st;
    xoshiro256ss_init(&st, 31415926);

    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    for (int i = 0; i < LOOPS; ++i) {
        xoshiro256ss_next(&st);
    }
    gettimeofday(&end, NULL);
    printf("xoshiro256ss cost %f ms.\n", diff_time_usec(end, &begin) / 1000.0);
}

static void test_mt19937_32() {
    struct mt19937_32 st;
    mt19937_init32(&st, 31415926);

    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    for (int i = 0; i < LOOPS; ++i) {
        mt19937_next32(&st);
    }
    gettimeofday(&end, NULL);
    printf("mt19937_32 cost %f ms.\n", diff_time_usec(end, &begin) / 1000.0);
}

static void test_mt19937_64() {
    struct mt19937_64 st;
    mt19937_init64(&st, 31415926);

    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    for (int i = 0; i < LOOPS; ++i) {
        mt19937_next64(&st);
    }
    gettimeofday(&end, NULL);
    printf("mt19937_64 cost %f ms.\n", diff_time_usec(end, &begin) / 1000.0);
}

int main(void) {
    test_rand();
    test_xorshift32();
    test_xorshift64();
    test_splitmix64();
    test_xoshiro256ss();
    test_mt19937_32();
    test_mt19937_64();
    return 0;
}
