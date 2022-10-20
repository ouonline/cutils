#ifndef __MATH_SPLITMIX64_H__
#define __MATH_SPLITMIX64_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct splitmix64 {
    uint64_t s;
};

void splitmix64_init(struct splitmix64* st, uint64_t seed);
uint64_t splitmix64_next(struct splitmix64* st);

#ifdef __cplusplus
typedef struct splitmix64 Splitmix64;
}
#endif

#endif
