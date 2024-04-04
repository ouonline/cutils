#include "cutils/vector.h"
#include <string.h> /* memcpy */

void vector_init(struct vector* vec, unsigned int sizeof_item) {
    qbuf_init(&vec->buf);
    vec->sizeof_item = sizeof_item;
}

int vector_push_back(struct vector* vec, const void* item) {
    return qbuf_append(&vec->buf, item, vec->sizeof_item);
}

int vector_pop_back(struct vector* vec, void* item) {
    if (qbuf_empty(&vec->buf)) {
        return -1;
    }

    unsigned long new_size = qbuf_size(&vec->buf) - vec->sizeof_item;
    if (item) {
        memcpy(item, (char*)qbuf_data(&vec->buf) + new_size, vec->sizeof_item);
    }

    qbuf_resize(&vec->buf, new_size);
    return 0;
}

void* vector_front(struct vector* vec) {
    if (qbuf_empty(&vec->buf)) {
        return NULL;
    }
    return qbuf_data(&vec->buf);
}

void* vector_back(struct vector* vec) {
    if (qbuf_empty(&vec->buf)) {
        return NULL;
    }

    unsigned long offset = qbuf_size(&vec->buf) - vec->sizeof_item;
    return (char*)qbuf_data(&vec->buf) + offset;
}

typedef void (*delete_func_t)(void*, void*);

struct deleter_arg {
    delete_func_t f;
    void* arg_for_callback;
};

static int deleter(void* item, void* arg) {
    struct deleter_arg* darg = (struct deleter_arg*)arg;
    darg->f(item, darg->arg_for_callback);
    return 0;
}

int vector_foreach(struct vector* vec, void* arg_for_callback, int (*f)(void*, void*)) {
    unsigned long offset;
    for (offset = 0; offset < qbuf_size(&vec->buf); offset += vec->sizeof_item) {
        int ret = f((char*)qbuf_data(&vec->buf) + offset, arg_for_callback);
        if (ret != 0) {
            return ret;
        }
    }
    return 0;
}

void vector_destroy(struct vector* vec, void* arg_for_callback, void (*cb)(void*, void*)) {
    if (vec) {
        if (cb) {
            struct deleter_arg arg = {.f = cb, .arg_for_callback = arg_for_callback};
            vector_foreach(vec, &arg, deleter);
        }
        qbuf_destroy(&vec->buf);
    }
}

void* vector_at(struct vector* vec, unsigned int idx) {
    return (char*)qbuf_data(&vec->buf) + vec->sizeof_item * idx;
}

int vector_remove(struct vector* vec, unsigned int idx, void* arg_for_callback,
                  void (*destroy)(void*, void*)) {
    if (vector_empty(vec)) {
        return 0;
    }

    unsigned int sz = vector_size(vec);
    if (idx >= sz) {
        return 0;
    }

    if (destroy) {
        destroy(vector_at(vec, idx), arg_for_callback);
    }

    if (idx < sz - 1) {
        void* dst = (char*)qbuf_data(&vec->buf) + vec->sizeof_item * idx;
        const void* src = (char*)qbuf_data(&vec->buf) + vec->sizeof_item * (idx + 1);
        memmove(dst, src, (sz - idx - 1) * vec->sizeof_item);
    }

    qbuf_resize(&vec->buf, qbuf_size(&vec->buf) - vec->sizeof_item);
    return 1;
}

int vector_reserve(struct vector* vec, unsigned int newsize) {
    return qbuf_reserve(&vec->buf, vec->sizeof_item * newsize);
}

int vector_resize(struct vector* vec, unsigned int newsize, void* arg_for_callback,
                  void (*destroy)(void* item, void* arg)) {
    unsigned int oldsize = vector_size(vec);
    if (newsize == oldsize) {
        return 0;
    }
    if (newsize < oldsize) {
        if (destroy) {
            unsigned int i;
            for (i = newsize; i < oldsize; ++i) {
                destroy(vector_at(vec, i), arg_for_callback);
            }
        }
    }

    return qbuf_resize(&vec->buf, newsize * vec->sizeof_item);
}
