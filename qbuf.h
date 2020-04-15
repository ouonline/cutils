#ifndef __QBUF_H__
#define __QBUF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>

struct qbuf { /* DONOT visit fileds directly */
    void* __base__;
    unsigned long __size__;
    unsigned long __capacity__;
};

struct qbuf_ref {
    const void* base;
    unsigned long size;
};

#ifdef __cplusplus
typedef struct qbuf QBuf;
typedef struct qbuf_ref QBufRef;
#endif

static inline void qbuf_ref_init(struct qbuf_ref* r) {
    r->base = NULL;
    r->size = 0;
}

static inline void qbuf_ref_destroy(struct qbuf_ref* r) {
    r->base = NULL;
    r->size = 0;
}

static inline void qbuf_ref_swap(struct qbuf_ref* a, struct qbuf_ref* b) {
    struct qbuf_ref tmp = *a;
    *a = *b;
    *b = tmp;
}

static inline void qbuf_init(struct qbuf* q) {
    q->__base__ = NULL;
    q->__size__ = 0;
    q->__capacity__ = 0;
}

static inline void qbuf_destroy(struct qbuf* q) {
    if (q->__base__) {
        free(q->__base__);
        qbuf_init(q);
    }
}

static inline unsigned long qbuf_size(const struct qbuf* q) {
    return q->__size__;
}

static inline void* qbuf_data(const struct qbuf* q) {
    return q->__base__;
}

static inline int qbuf_empty(const struct qbuf* q) {
    return (q->__size__ == 0);
}

static inline void qbuf_clear(struct qbuf* q) {
    q->__size__ = 0;
}

static inline int qbuf_reserve(struct qbuf* q, unsigned long expected_size) {
    if (expected_size > q->__capacity__) {
        unsigned long new_capacity;
        if (q->__capacity__ == 0) {
            new_capacity = 4;
        } else {
            new_capacity = q->__capacity__;
        }

        do {
            new_capacity <<= 1;
        } while (new_capacity < expected_size);

        void* new_base = realloc(q->__base__, new_capacity);
        if (!new_base) {
            return -1;
        }

        q->__base__ = new_base;
        q->__capacity__ = new_capacity;
    }

    return 0;
}

static inline int qbuf_resize(struct qbuf* q, unsigned long expected_size) {
    if (qbuf_reserve(q, expected_size) != 0) {
        return -1;
    }

    q->__size__ = expected_size;
    return 0;
}

static inline int qbuf_append(struct qbuf* q, const void* data, unsigned long size) {
    const unsigned long new_size = q->__size__ + size;
    if (qbuf_reserve(q, new_size) != 0) {
        return -1;
    }

    memcpy((char*)(q->__base__) + q->__size__, data, size);
    q->__size__ = new_size;
    return 0;
}

static inline int qbuf_assign(struct qbuf* q, const void* data, unsigned long size) {
    qbuf_clear(q);
    return qbuf_append(q, data, size);
}

static inline void qbuf_swap(struct qbuf* a, struct qbuf* b) {
    struct qbuf tmp = *a;
    *a = *b;
    *b = tmp;
}

#ifdef __cplusplus
}
#endif

#endif
