#ifndef __CUTILS_VECTOR_H__
#define __CUTILS_VECTOR_H__

#include "qbuf.h"

struct vector {
    struct qbuf buf;
    unsigned int sizeof_item;
};

void vector_init(struct vector*, unsigned int sizeof_item);

/* returns 0 if success, -1 otherwise */
int vector_push_back(struct vector*, const void* item);

/* returns 0 if success, -1 otherwise. deleted item will be copied to `item` if `item` is not null */
int vector_pop_back(struct vector*, void* item);

void vector_destroy(struct vector*, void* arg, void (*destructor)(void* item, void* arg));

static inline int vector_empty(struct vector* vec) {
    return (qbuf_size(&vec->buf) == 0);
}

static inline unsigned int vector_size(struct vector* vec) {
    return qbuf_size(&vec->buf) / vec->sizeof_item;
}

#endif
