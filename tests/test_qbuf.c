#include "cutils/qbuf.h"

#undef NDEBUG
#include <assert.h>

static void test_inline() {
    struct qbuf b;
    qbuf_init(&b);

    qbuf_append(&b, "123", 3);
    assert(b.__base__ == &b.__capacity__);

    qbuf_clear(&b);
    qbuf_append(&b, "12345678", 8);
    assert(b.__base__ == &b.__capacity__);

    qbuf_destroy(&b);
}

static void test_no_inline() {
    struct qbuf b;
    qbuf_init(&b);

    qbuf_append(&b, "123", 3);
    assert(b.__base__ == &b.__capacity__);

    qbuf_append(&b, "12345678", 8);
    assert(b.__base__ != &b.__capacity__);

    qbuf_destroy(&b);
}

static void test_swap() {
    struct qbuf inline_b;
    qbuf_init(&inline_b);
    unsigned long inline_size = 3;
    qbuf_append(&inline_b, "123", inline_size);
    assert(inline_b.__base__ == &inline_b.__capacity__);

    struct qbuf non_inline_b;
    qbuf_init(&non_inline_b);
    unsigned long non_inline_size = 10;
    qbuf_append(&non_inline_b, "0123456789", non_inline_size);
    assert(non_inline_b.__base__ != &non_inline_b.__capacity__);
    void* non_inline_mem = non_inline_b.__base__;

    qbuf_swap(&inline_b, &non_inline_b);
    assert(inline_b.__base__ != &inline_b.__capacity__);
    assert(inline_b.__base__ == non_inline_mem);
    assert(inline_b.__size__ == non_inline_size);
    assert(non_inline_b.__base__ == &non_inline_b.__capacity__);
    assert(non_inline_b.__size__ == inline_size);

    qbuf_destroy(&inline_b);
    qbuf_destroy(&non_inline_b);
}

int main(void) {
    test_inline();
    test_no_inline();
    test_swap();
    return 0;
}
