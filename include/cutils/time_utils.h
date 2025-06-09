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

/* str format: %d-%d-%d %d:%d:%d */
time_t str2gmtime(const char* str);

/* str format: %04d-%02d-%02d %02d:%02d:%02d */
char* gmtime2str(time_t ts, char* buf);
char* localtime2str(time_t ts, char* buf);

static inline time_t bjtime2gmtime(time_t ts) {
    return ts - 28800;
}

static inline time_t gmtime2bjtime(time_t ts) {
    return ts + 28800;
}

uint64_t diff_time_usec(struct timeval end, struct timeval begin);

#ifdef __cplusplus
}
#endif

#endif
