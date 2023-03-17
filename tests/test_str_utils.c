#include <string.h>
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

static void test_sunday_matching() {
    const char *text = "ABCZABCDAEZABCDABCDABDE", *pattern = "ABCDABD";

    struct sunday_context ctx;
    sunday_preprocess(&ctx, pattern, strlen(pattern));
    unsigned int ret = sunday_match(&ctx, text, strlen(text));
    assert(ret == 15);
}

int main(void) {
    test_dec2long();
    test_sunday_matching();
    return 0;
}
