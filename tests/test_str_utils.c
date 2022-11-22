#include <stdio.h>
#include "utils/str_utils.c"

int main(void)
{
    int i;
    const char* str[] = {"cc", "e8", "cf", "7f", NULL};

    for (i = 0; str[i]; ++i) {
        long result = hex2long(str[i]);
        printf("%s -> %ld\n", str[i], result);
    }

    return 0;
}
