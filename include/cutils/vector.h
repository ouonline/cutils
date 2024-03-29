#ifndef __CUTILS_VECTOR_H__
#define __CUTILS_VECTOR_H__

#include "qbuf.h"

struct vector {
    struct qbuf buf;
    unsigned int sizeof_item;
};

void vector_init(struct vector*, unsigned int sizeof_item);
void vector_destroy(struct vector*, void* arg_for_callback, void (*)(void* item, void* arg));

/* returns 0 if success, -1 otherwise. NOTE that contents of `item` will be copied. */
int vector_push_back(struct vector*, const void* item);

/* returns 0 if success, -1 otherwise. deleted item will be copied to `item` if `item` is not null */
int vector_pop_back(struct vector*, void* item);

void* vector_front(struct vector*);
void* vector_back(struct vector*);
void* vector_at(struct vector*, unsigned int idx);

/* returns the number of removed item */
int vector_remove(struct vector*, unsigned int idx, void* arg_for_callback,
                  void (*destroy)(void* item, void* arg));

/* returns 0 if success */
int vector_reserve(struct vector*, unsigned int newsize);

/* returns 0 if success */
int vector_resize(struct vector*, unsigned int newsize, void* arg_for_callback,
                  void (*destroy)(void* item, void* arg));

/* iteration terminates if `callback` returns a non-zero value, which will be the returned value of vector_foreach(). */
int vector_foreach(struct vector*, void* arg_for_callback, int (*callback)(void* item, void* arg));

static inline int vector_empty(struct vector* vec) {
    return (qbuf_size(&vec->buf) == 0);
}

static inline unsigned int vector_size(struct vector* vec) {
    return qbuf_size(&vec->buf) / vec->sizeof_item;
}

#endif
