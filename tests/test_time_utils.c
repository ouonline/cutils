#include <stdio.h>
#include "cutils/time_utils.h"

static void test_diff_time(void) {
    struct timeval begin, end;
    begin.tv_sec = 0;
    begin.tv_usec = 999999;
    end.tv_sec = 1;
    end.tv_usec = 0;
    uint64_t diff = diff_time_usec(end, begin);
    printf("diff = %lu\n", diff);
}

int main(void) {
    const char* s = "2020-03-04 05:06:07";
    time_t ts;
    char buf[20];

    ts = str2gmtime(s);
    printf("%s -> %lu\n", s, ts);

    gmtime2str(ts, buf);
    printf("%lu -> %s\n", ts, buf);

    time_t bjts = gmtime2bjtime(ts);
    gmtime2str(bjts, buf);
    printf("bj time = %lu %s\n", bjts, buf);

    test_diff_time();

    return 0;
}
