#ifndef __CUTILS_QBUF_REF_H__
#define __CUTILS_QBUF_REF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h> /* memcmp() */

struct qbuf_ref {
    const void* base;
    unsigned long size;
};

#ifdef __cplusplus
typedef struct qbuf_ref QBufRef;
#endif

static inline void qbuf_ref_reset(struct qbuf_ref* r) {
    r->base = NULL;
    r->size = 0;
}

/* returns 0 if equal */
static inline int qbuf_ref_equal(const struct qbuf_ref* a, const struct qbuf_ref* b) {
    if (a->size != b->size) {
        return -1;
    }

    return memcmp(a->base, b->base, a->size);
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
