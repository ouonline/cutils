#ifndef __MATH_XORSHIFT32_H__
#define __MATH_XORSHIFT32_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct xorshift32 {
    uint32_t s;
};

void xorshift32_init(struct xorshift32* st, uint32_t seed);
uint32_t xorshift32_next(struct xorshift32* st);

#ifdef __cplusplus
typedef struct xorshift32 Xorshift32;
}
#endif

#endif
