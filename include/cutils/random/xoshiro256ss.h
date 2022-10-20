#ifndef __MATH_XOSHIRO256SS_H__
#define __MATH_XOSHIRO256SS_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct xoshiro256ss {
    uint64_t s[4];
};

void xoshiro256ss_init(struct xoshiro256ss* st, uint64_t seed);
uint64_t xoshiro256ss_next(struct xoshiro256ss* st);

#ifdef __cplusplus
typedef struct xoshiro256ss Xoshiro256ss;
}
#endif

#endif
