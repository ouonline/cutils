#ifndef __CUTILS_UTILS_H__
#define __CUTILS_UTILS_H__

#include <stdint.h>
#include <stddef.h> // offsetof

#ifdef __cplusplus
namespace cutils {
#endif

static inline int is2exp(uint64_t n) {
    return ((n > 0) ? (!(n & (n - 1))) : 0);
}

static inline uint64_t align(uint64_t n, uint64_t alignment) {
    return ((n + alignment - 1) & (~(alignment - 1)));
}

static inline uint64_t upper(uint64_t size, uint64_t n) {
    return ((size + n - 1) / n);
}

#define container_of(ptr, type, member)                      \
    ((type*)((unsigned long)(ptr) - offsetof(type, member)))

#define swap_value(x, y)       \
    do {                       \
        typeof(x) _tmp_ = (x); \
        (x) = (y);             \
        (y) = _tmp_;           \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif
