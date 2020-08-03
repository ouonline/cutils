#ifndef __UTILS_UTILS_H__
#define __UTILS_UTILS_H__

#include <stdint.h>

#ifdef __cplusplus
namespace outils {
#endif

static inline int is2exp(uint64_t n) {
    return ((n > 0) ? (!(n & (n - 1))) : 0);
}

static inline uint64_t align(uint64_t n, uint32_t alignment) {
    return ((n + alignment - 1) & (~(alignment - 1)));
}

static inline uint64_t upper(uint64_t size, uint64_t n) {
    return ((size + n - 1) / n);
}

#ifndef offsetof
#define offsetof(type, member) ((unsigned long)(&(((type*)0)->member)))
#endif

#define container_of(ptr, type, member)                         \
    ((type*)((unsigned long)(ptr) - offsetof(type, member)))

#ifdef __cplusplus
}
#endif

#endif
