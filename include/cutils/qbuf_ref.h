#ifndef __CUTILS_QBUF_REF_H__
#define __CUTILS_QBUF_REF_H__

#ifdef __cplusplus
extern "C" {
#endif

struct qbuf_ref {
    const void* base;
    unsigned long size;
};

#ifdef __cplusplus
typedef struct qbuf_ref QBufRef;
#endif

static inline void qbuf_ref_reset(struct qbuf_ref* r) {
    r->base = 0;
    r->size = 0;
}

static inline void qbuf_ref_swap(struct qbuf_ref* a, struct qbuf_ref* b) {
    struct qbuf_ref tmp = *a;
    *a = *b;
    *b = tmp;
}

#ifdef __cplusplus
}
#endif

#endif
