#ifndef __UTILS_TIME_UTILS_H__
#define __UTILS_TIME_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>

unsigned long str2gmtime(const char* time_str);
void gmtime2str(unsigned long timeval, void* buf, int buflen);

/* buf size >= 27, tp can be NULL */
void current_datetime(char buf[27], struct tm* tp);

#ifdef __cplusplus
}
#endif

#endif
