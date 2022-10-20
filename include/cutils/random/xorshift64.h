#ifndef __MATH_XORSHIFT64_H__
#define __MATH_XORSHIFT64_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct xorshift64 {
    uint64_t s;
};

void xorshift64_init(struct xorshift64* st, uint64_t seed);
uint64_t xorshift64_next(struct xorshift64* st);

#ifdef __cplusplus
typedef struct xorshift64 Xorshift64;
}
#endif

#endif
