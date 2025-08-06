#ifndef __CUTILS_QBUF_H__
#define __CUTILS_QBUF_H__

#ifdef __cplusplus
extern "C" {
#endif

struct qbuf { /* DONOT visit fileds directly */
    void* __base__;
    unsigned long __size__;
    unsigned long __capacity__;
};

#ifdef __cplusplus
typedef struct qbuf QBuf;
#endif

/** the following functions return 0 or -ENOMEM, unless otherwise specified. */

void qbuf_init(struct qbuf* q);
void qbuf_destroy(struct qbuf* q);

static inline void qbuf_clear(struct qbuf* q) {
    q->__size__ = 0;
}

int qbuf_reserve(struct qbuf* q, unsigned long expected_size);
int qbuf_resize(struct qbuf* q, unsigned long expected_size);

int qbuf_append(struct qbuf* q, const void* data, unsigned long size);
int qbuf_append_c(struct qbuf* q, char c);

static inline void* qbuf_data(const struct qbuf* q) {
    return q->__base__;
}

static inline unsigned long qbuf_size(const struct qbuf* q) {
    return q->__size__;
}

static inline unsigned long qbuf_capacity(const struct qbuf* q) {
    return q->__capacity__;
}

static inline int qbuf_empty(const struct qbuf* q) {
    return (q->__size__ == 0);
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

static inline int qbuf_copy_construct(struct qbuf* src_item, struct qbuf* new_item) {
    qbuf_init(new_item);
    return qbuf_assign(new_item, qbuf_data(src_item), qbuf_size(src_item));
}

static inline void qbuf_move_construct(struct qbuf* src_item, struct qbuf* new_item) {
    *new_item = *src_item;
    qbuf_init(src_item);
}

static inline void qbuf_move(struct qbuf* src, struct qbuf* dst) {
    qbuf_destroy(dst);
    qbuf_move_construct(src, dst);
}

/* returns 0 if equal */
int qbuf_equal(const struct qbuf* a, const struct qbuf* b);

#include "qbuf_ref.h"

static inline struct qbuf_ref qbuf_get_ref(const struct qbuf* q) {
    const struct qbuf_ref ref = {
        .base = q->__base__,
        .size = q->__size__,
    };
    return ref;
}

#ifdef __cplusplus
}
#endif

#endif
