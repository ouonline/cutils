#ifndef __CUTILS_QBUF_OL_H__
#define __CUTILS_QBUF_OL_H__

struct qbuf_ol {
    unsigned long off;
    unsigned long len;
};

/* returns 0 if equal */
static inline int qbuf_ol_equal(const struct qbuf_ol* a, const struct qbuf_ol* b) {
    return (a->off == b->off && a->len == b->len);
}

static inline void qbuf_ol_reset(struct qbuf_ol* b) {
    b->off = 0;
    b->len = 0;
}

static inline void qbuf_ol_swap(struct qbuf_ol* a, struct qbuf_ol* b) {
    struct qbuf_ol tmp = *a;
    *a = *b;
    *b = tmp;
}

#endif
