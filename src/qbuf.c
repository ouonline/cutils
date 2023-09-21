#include "cutils/qbuf.h"
#include <stdlib.h>
#include <string.h>

#define MAX_INLINE_SIZE sizeof(unsigned long)

void qbuf_init(struct qbuf* q) {
    q->__base__ = NULL;
    q->__size__ = 0;
    q->__capacity__ = 0;
}

void qbuf_destroy(struct qbuf* q) {
    if (q->__base__) {
        free(q->__base__);
        qbuf_init(q);
    }
}

int qbuf_reserve(struct qbuf* q, unsigned long expected_size) {
    if (expected_size <= q->__capacity__) {
        return 0;
    }

    unsigned long real_capacity;
    if (q->__capacity__ == 0) {
        real_capacity = 4;
    } else {
        real_capacity = q->__capacity__;
    }

    do {
        real_capacity <<= 1;
    } while (real_capacity < expected_size);

    void* new_base = realloc(q->__base__, real_capacity);
    if (!new_base) {
        return -1;
    }

    q->__base__ = new_base;
    q->__capacity__ = real_capacity;

    return 0;
}

int qbuf_resize(struct qbuf* q, unsigned long expected_size) {
    if (qbuf_reserve(q, expected_size) != 0) {
        return -1;
    }

    q->__size__ = expected_size;
    return 0;
}

int qbuf_append(struct qbuf* q, const void* data, unsigned long size) {
    const unsigned long new_size = q->__size__ + size;
    if (qbuf_reserve(q, new_size) != 0) {
        return -1;
    }

    memcpy((char*)(q->__base__) + q->__size__, data, size);
    q->__size__ = new_size;
    return 0;
}

int qbuf_append_c(struct qbuf* q, char c) {
    const unsigned long new_size = q->__size__ + 1;
    if (qbuf_reserve(q, new_size) != 0) {
        return -1;
    }

    *((char*)q->__base__ + q->__size__) = c;
    q->__size__ = new_size;
    return 0;
}

int qbuf_equal(const struct qbuf* a, const struct qbuf* b) {
    if (a->__size__ != b->__size__) {
        return -1;
    }

    return memcmp(a->__base__, b->__base__, a->__size__);
}
