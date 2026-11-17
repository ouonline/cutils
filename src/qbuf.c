#include "cutils/qbuf.h"
#include <stdlib.h>
#include <string.h>

void qbuf_init(struct qbuf* q) {
    q->__base__ = NULL;
    q->__size__ = 0;
    q->__capacity__ = 0;
}

void qbuf_destroy(struct qbuf* q) {
    if (q->__base__) {
        free(q->__base__);
    }
}

int qbuf_reserve(struct qbuf* q, uint64_t expected_size) {
    if (expected_size <= q->__capacity__) {
        return 0;
    }

    uint64_t real_capacity;
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

int qbuf_resize(struct qbuf* q, uint64_t expected_size) {
    if (qbuf_reserve(q, expected_size) != 0) {
        return -1;
    }

    q->__size__ = expected_size;
    return 0;
}

int qbuf_append(struct qbuf* q, const void* data, uint64_t size) {
    const uint64_t new_size = q->__size__ + size;
    uint64_t offset = UINT64_MAX;

    // source data is in [base, base + capacity)
    if ((uintptr_t)data >= (uintptr_t)(q->__base__) &&
        (uintptr_t)data < (uintptr_t)(q->__base__) + q->__capacity__) {
        offset = (uintptr_t)data - (uintptr_t)(q->__base__);
    }

    if (qbuf_reserve(q, new_size) != 0) {
        return -1;
    }

    if (offset != UINT64_MAX) {
        memmove((char*)(q->__base__) + q->__size__,
                (const char*)(q->__base__) + offset, size);
    } else {
        memcpy((char*)(q->__base__) + q->__size__, data, size);
    }

    q->__size__ = new_size;

    return 0;
}

int qbuf_append_c(struct qbuf* q, char c) {
    const uint64_t new_size = q->__size__ + 1;
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
