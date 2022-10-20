#ifndef __MATH_MT19937_32_H__
#define __MATH_MT19937_32_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct mt19937_32 {
    uint32_t index;
    uint32_t mt[624];
};

void mt19937_init32(struct mt19937_32* st, uint32_t seed);
uint32_t mt19937_next32(struct mt19937_32* st);

#ifdef __cplusplus
typedef struct mt19937_32 MT19937_32;
}
#endif

#endif
