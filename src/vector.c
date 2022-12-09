#include "cutils/vector.h"
#include <stddef.h> /* NULL */

void vector_init(struct vector* vec, unsigned int sizeof_item) {
    qbuf_init(&vec->buf);
    vec->sizeof_item = sizeof_item;
}

int vector_push_back(struct vector* vec, const void* item) {
    return qbuf_append(&vec->buf, item, vec->sizeof_item);
}

int vector_pop_back(struct vector* vec, void* item) {
    if (qbuf_empty(&vec->buf)) {
        return -1;
    }

    unsigned long new_size = qbuf_size(&vec->buf) - vec->sizeof_item;
    if (item) {
        memcpy(item, (char*)qbuf_data(&vec->buf) + new_size, vec->sizeof_item);
    }

    qbuf_resize(&vec->buf, new_size);
    return 0;
}

void vector_destroy(struct vector* vec, void* arg, void (*destructor)(void* item, void* arg)) {
    void* cursor = qbuf_data(&vec->buf);
    const void* end = (const char*)cursor + qbuf_size(&vec->buf);
    for (; cursor < end; cursor = (char*)cursor + vec->sizeof_item) {
        if (destructor) {
            destructor(cursor, arg);
        }
    }

    qbuf_destroy(&vec->buf);
    vec->sizeof_item = 0;
}
