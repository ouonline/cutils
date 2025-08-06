#include "cutils/qbuf.h"
#include <stddef.h>

#undef NDEBUG
#include <assert.h>

static void test_generic() {
    struct qbuf a;
    qbuf_init(&a);
    unsigned long a_size = 3;
    qbuf_append(&a, "123", a_size);
    assert(qbuf_data(&a) != NULL);
    assert(qbuf_size(&a) >= a_size);
    qbuf_destroy(&a);
}

int main(void) {
    test_generic();
    return 0;
}
