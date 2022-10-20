#ifndef __MATH_MT19937_64_H__
#define __MATH_MT19937_64_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct mt19937_64 {
    uint64_t index;
    uint64_t mt[312];
};

void mt19937_init64(struct mt19937_64* st, uint64_t seed);
uint64_t mt19937_next64(struct mt19937_64* st);

#ifdef __cplusplus
typedef struct mt19937_64 MT19937_64;
}
#endif

#endif
