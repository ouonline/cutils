#include <stdio.h>
#include "cutils/time_utils.h"

int main(void) {
    const char* s = "2020-03-04 05:06:07";
    unsigned long ts;
    char buf[20];

    ts = str2gmtime(s);
    printf("%s -> %lu\n", s, ts);

    gmtime2str(ts, buf);
    printf("%lu -> %s\n", ts, buf);

    unsigned long bjts = gmtime2bjtime(ts);
    gmtime2str(bjts, buf);
    printf("bj time = %lu %s\n", bjts, buf);

    return 0;
}
