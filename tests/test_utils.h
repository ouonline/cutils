#ifndef __CUTILS_TEST_UTILS_H__
#define __CUTILS_TEST_UTILS_H__

#include <string.h>
#include <stdlib.h>
#include "cutils/random/xoshiro256ss.h"

static const char* g_strs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_";

static inline char** gen_random_str(int num) {
    int len = strlen(g_strs);
    char** data = malloc(sizeof(char*) * num);

    struct xoshiro256ss rnd;
    xoshiro256ss_init(&rnd, (uint64_t)&rnd);

    for (int i = 0; i < num; ++i) {
        int slen = xoshiro256ss_next(&rnd) % 32 + 1;
        char* s = malloc(slen + 1);
        for (int j = 0; j < slen; ++j) {
            s[j] = g_strs[xoshiro256ss_next(&rnd) % len];
        }
        s[slen] = '\0';
        data[i] = s;
    }

    return data;
}

#endif
