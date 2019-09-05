#include "time_utils.h"
#include <stdio.h>
#include <sys/time.h>

/* implementation of mktime() from linux kernel */

/**
 * mktime() implementation from kernel/time.c.
 * Converts Gregorian date to seconds since 1970-01-01 00:00:00.
 **/
static inline unsigned long
__mktime(const unsigned int year0, const unsigned int mon0,
         const unsigned int day, const unsigned int hour,
         const unsigned int min, const unsigned int sec) {
    unsigned int mon = mon0, year = year0;

    /* 1..12 -> 11,12,1..10 */
    if (0 >= (int) (mon -= 2)) {
        mon += 12;      /* Puts Feb last since it has leap day */
        year -= 1;
    }

    return ((((unsigned long long)
              (year/4 - year/100 + year/400 + 367*mon/12 + day) +
              year*365 - 719499
             )*24 + hour /* now have hours */
            )*60 + min /* now have minutes */
           )*60 + sec; /* finally seconds */
}

unsigned long str2gmtime(const char* time_str) {
    int year, mon, day, hour, min, sec;

    sscanf(time_str, "%d-%d-%d %d:%d:%d", &year, &mon, &day,
           &hour, &min, &sec);

    return __mktime(year, mon, day, hour, min, sec);
}

void gmtime2str(unsigned long timeval, void* buf, int buflen) {
    struct tm* tp;

    tp = gmtime((const time_t*)(&timeval));
    strftime(buf, buflen, "%F %T", tp);
}

/* time format: YYYY-MM-DD hh:mm:ss.uuuuuu
 * buf size >= 27 */
void current_datetime(char buf[], struct tm* tp) {
    int len;
    struct timeval tv;
    struct tm ltm;

    if (!tp) {
        tp = &ltm;
    }

    gettimeofday(&tv, NULL);
    localtime_r(&tv.tv_sec, tp);
    len = strftime(buf, 27, "%F %T", tp);
    sprintf((char*)buf + len, ".%06ld", tv.tv_usec);
}
