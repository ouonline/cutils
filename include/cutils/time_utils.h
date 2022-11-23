#ifndef __CUTILS_TIME_UTILS_H__
#define __CUTILS_TIME_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <time.h>
#include <sys/time.h>

uint64_t str2gmtime(const char* time_str);
char* gmtime2str(uint64_t timeval, char* buf);

static inline uint64_t bjtime2gmtime(uint64_t ts) {
    return ts - 28800;
}

static inline uint64_t gmtime2bjtime(uint64_t ts) {
    return ts + 28800;
}

uint64_t diff_time_usec(struct timeval end, const struct timeval* begin);

/* time format: YYYY-MM-DD_hh:mm:ss.uuuuuu. buf size >= 27 */
void current_datetime(char buf[], struct tm* tp);

#ifdef __cplusplus
}
#endif

#endif
