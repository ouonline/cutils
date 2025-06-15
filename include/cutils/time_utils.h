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
time_t str2time(const char* str);

/*
  str format: %04d-%02d-%02d %02d:%02d:%02d
  returns the number of bytes written to `buf`.
*/
uint32_t time2str(time_t, char* buf);

uint64_t diff_time_usec(struct timeval end, struct timeval begin);

#ifdef __cplusplus
}
#endif

#endif
