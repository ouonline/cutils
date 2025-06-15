#include <stdio.h>
#include "cutils/time_utils.h"
#include <string.h>

#undef NDEBUG
#include <assert.h>

static void test_diff_time(void) {
    struct timeval begin, end;
    begin.tv_sec = 0;
    begin.tv_usec = 999999;
    end.tv_sec = 1;
    end.tv_usec = 0;
    uint64_t diff = diff_time_usec(end, begin);
    assert(diff == 1);
}

static void test_time_str(void) {
    const char* s = "2020-03-04 05:06:07";
    time_t ts, ts2;
    char buf[32];

    ts = str2time(s);
    time2str(ts, buf);
    assert(strcmp(s, buf) == 0);

    ts2 = str2time(buf);
    assert(ts == ts2);
}

int main(void) {
    test_time_str();
    test_diff_time();
    return 0;
}
