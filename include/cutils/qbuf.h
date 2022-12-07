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

void qbuf_init(struct qbuf* q);
void qbuf_destroy(struct qbuf* q);
int qbuf_reserve(struct qbuf* q, unsigned long expected_size);
int qbuf_resize(struct qbuf* q, unsigned long expected_size);
int qbuf_append(struct qbuf* q, const void* data, unsigned long size);
int qbuf_append_c(struct qbuf* q, char c);

static inline void qbuf_clear(struct qbuf* q) {
    q->__size__ = 0;
}

static inline int qbuf_assign(struct qbuf* q, const void* data, unsigned long size) {
    qbuf_clear(q);
    return qbuf_append(q, data, size);
}

static inline void* qbuf_data(const struct qbuf* q) {
    return q->__base__;
}

static inline unsigned long qbuf_size(const struct qbuf* q) {
    return q->__size__;
}

static inline int qbuf_empty(const struct qbuf* q) {
    return (q->__size__ == 0);
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
