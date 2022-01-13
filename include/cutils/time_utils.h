#ifndef __UTILS_TIME_UTILS_H__
#define __UTILS_TIME_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

uint64_t str2gmtime(const char* time_str);
char* gmtime2str(uint64_t timeval, char* buf);

static inline uint64_t bjtime2gmtime(uint64_t ts) {
    return ts - 28800;
}

static inline uint64_t gmtime2bjtime(uint64_t ts) {
    return ts + 28800;
}

#ifdef __cplusplus
}
#endif

#endif
