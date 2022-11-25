#include <stdio.h>
#include "cutils/str_utils.h"

#undef NDEBUG
#include <assert.h>

static void test_dec2long() {
    assert(dec2long("12345") == 12345);
    assert(dec2long("-6534") == -6534);
    assert(dec2long("+43275925076582765") == 43275925076582765);
    assert(dec2long("     +542j") == 542);
    assert(dec2long("     k-563n33j") == 0);
    assert(dec2long("     -563n33j") == -563);
}

int main(void) {
    test_dec2long();

    const char* str[] = {"cc", "e8", "cf", "7f", NULL};
    for (int i = 0; str[i]; ++i) {
        long result = hex2long(str[i]);
        printf("%s -> %ld\n", str[i], result);
    }

    return 0;
}
