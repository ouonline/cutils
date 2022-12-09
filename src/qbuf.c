#include "cutils/qbuf.h"
#include <stdlib.h>
#include <string.h>

#define MAX_INLINE_SIZE sizeof(unsigned long)

void qbuf_init(struct qbuf* q) {
    q->__size__ = 0;
    q->__base__ = &q->__capacity__;
}

void qbuf_destroy(struct qbuf* q) {
    if (q->__base__ != &q->__capacity__) {
        free(q->__base__);
        qbuf_init(q);
    }
}

int qbuf_reserve(struct qbuf* q, unsigned long expected_size) {
    unsigned long real_capacity;
    if (q->__base__ == &q->__capacity__) {
        real_capacity = MAX_INLINE_SIZE;
    } else {
        real_capacity = q->__capacity__;
    }

    if (expected_size <= real_capacity) {
        return 0;
    }

    do {
        real_capacity <<= 1;
    } while (real_capacity < expected_size);

    void* new_base;
    if (q->__base__ == &q->__capacity__) {
        new_base = malloc(real_capacity);
        if (!new_base) {
            return -1;
        }
        *(unsigned long*)new_base = q->__capacity__; /* copy data */
    } else {
        new_base = realloc(q->__base__, real_capacity);
        if (!new_base) {
            return -1;
        }
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

#define do_swap_value(type, __a__, __b__)       \
    do {                                        \
        type ___tmp___ = (__a__);               \
        (__a__) = (__b__);                      \
        (__b__) = ___tmp___;                    \
    } while (0)

#define do_swap2(type, inline_ptr, non_inline_ptr)                      \
    do {                                                                \
        type ___tmp___ = *(inline_ptr);                                 \
        *(inline_ptr) = *(non_inline_ptr);                              \
        (non_inline_ptr)->__base__ = &(non_inline_ptr)->__capacity__;   \
        (non_inline_ptr)->__capacity__ = ___tmp___.__capacity__;        \
        (non_inline_ptr)->__size__ = ___tmp___.__size__;                \
    } while (0)

void qbuf_swap(struct qbuf* a, struct qbuf* b) {
    if (a->__base__ == &a->__capacity__) {
        if (b->__base__ == &b->__capacity__) {
            do_swap_value(unsigned long, a->__capacity__, b->__capacity__);
            do_swap_value(unsigned long, a->__size__, b->__size__);
        } else {
            do_swap2(struct qbuf, a, b);
        }
    } else {
        if (b->__base__ == &b->__capacity__) {
            do_swap2(struct qbuf, b, a);
        } else {
            do_swap_value(struct qbuf, *a, *b);
        }
    }
}

void qbuf_move(struct qbuf* src, struct qbuf* dst) {
    qbuf_destroy(dst);
    if (src->__base__ == &src->__capacity__) {
        dst->__size__ = src->__size__;
        dst->__capacity__ = src->__capacity__;
        dst->__base__ = &dst->__capacity__;
        src->__size__ = 0;
    } else {
        *dst = *src;
        qbuf_init(src);
    }
}
