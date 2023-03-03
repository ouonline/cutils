#ifndef __CUTILS_TIME_UTILS_H__
#define __CUTILS_TIME_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <time.h>

#ifdef _MSC_VER
#include <winsock.h> /* struct timeval */
#else
#include <sys/time.h>
#endif

uint64_t str2gmtime(const char* time_str);
char* gmtime2str(uint64_t timeval, char* buf);

static inline uint64_t bjtime2gmtime(uint64_t ts) {
    return ts - 28800;
}

static inline uint64_t gmtime2bjtime(uint64_t ts) {
    return ts + 28800;
}

uint64_t diff_time_usec(struct timeval end, const struct timeval* begin);

#ifdef __cplusplus
}
#endif

#endif
