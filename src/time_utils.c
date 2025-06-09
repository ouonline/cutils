#include "cutils/time_utils.h"
#include <stdio.h>

/* implementation of mktime() from linux kernel */

/**
 * mktime() implementation from kernel/time.c.
 * Converts Gregorian date to seconds since 1970-01-01 00:00:00.
 **/
static time_t
__mktime(const unsigned int year0, const unsigned int mon0,
         const unsigned int day, const unsigned int hour,
         const unsigned int min, const unsigned int sec) {
    unsigned int mon = mon0, year = year0;

    /* 1..12 -> 11,12,1..10 */
    if (0 >= (int) (mon -= 2)) {
        mon += 12;      /* Puts Feb last since it has leap day */
        year -= 1;
    }

    return ((((year/4 - year/100 + year/400 + 367*mon/12 + day) +
              year*365 - 719499
              )*24 + hour /* now have hours */
             )*60 + min /* now have minutes */
            )*60 + sec; /* finally seconds */
}

time_t str2gmtime(const char* time_str) {
    int year, mon, day, hour, min, sec;

    sscanf(time_str, "%d-%d-%d %d:%d:%d", &year, &mon, &day,
           &hour, &min, &sec);

    return __mktime(year, mon, day, hour, min, sec);
}

char* gmtime2str(time_t ts, char* buf) {
    struct tm tm;
#ifdef _MSC_VER
    gmtime_s(&tm, &ts);
#else
    gmtime_r(&ts, &tm);
#endif
    sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);
    return buf;
}

char* localtime2str(time_t ts, char* buf) {
    struct tm tm;
#ifdef _MSC_VER
    localtime_s(&tm, &ts);
#else
    localtime_r(&ts, &tm);
#endif
    sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);
    return buf;
}

uint64_t diff_time_usec(struct timeval end, struct timeval begin) {
    if (end.tv_usec < begin.tv_usec) {
        --end.tv_sec;
        end.tv_usec += 1000000;
    }
    return (end.tv_sec - begin.tv_sec) * 1000000 + (end.tv_usec - begin.tv_usec);
}
