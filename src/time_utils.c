#include "cutils/time_utils.h"
#include <stdio.h>

time_t str2time(const char* str) {
    struct tm tm = {0};
    sscanf(str, "%d-%d-%d %d:%d:%d", &tm.tm_year, &tm.tm_mon,
           &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
    tm.tm_year -= 1900;
    --tm.tm_mon;
    return mktime(&tm);
}

uint32_t time2str(time_t ts, char* buf) {
    struct tm tm = {0};

#ifdef _MSC_VER
    localtime_s(&tm, &ts);
#else
    localtime_r(&ts, &tm);
#endif

    return sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d",
                   tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                   tm.tm_hour, tm.tm_min, tm.tm_sec);
}

uint64_t diff_time_usec(struct timeval end, struct timeval begin) {
    if (end.tv_usec < begin.tv_usec) {
        --end.tv_sec;
        end.tv_usec += 1000000;
    }
    return (end.tv_sec - begin.tv_sec) * 1000000 + (end.tv_usec - begin.tv_usec);
}
