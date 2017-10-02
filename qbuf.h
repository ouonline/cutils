#ifndef __QBUF_H__
#define __QBUF_H__

#include <stdlib.h>

struct qbuf {
    char* base;
    unsigned int len;
};

struct qbuf_ref {
    const char* base;
    unsigned int len;
};

static inline void qbuf_init(struct qbuf* q) {
    q->base = NULL;
    q->len = 0;
}

static inline void qbuf_destroy(struct qbuf* q) {
    q->len = 0;
    if (q->base) {
        free(q->base);
        q->base = NULL;
    }
}

static inline int qbuf_resize(struct qbuf* q, unsigned int newsize) {
    void* ptr = realloc(q->base, newsize);
    if (ptr) {
        q->base = (char*)ptr;
        q->len = newsize;
        return 0;
    }

    return -1;
}

#endif
