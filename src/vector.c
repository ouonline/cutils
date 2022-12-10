#include "cutils/vector.h"
#include <stddef.h> /* NULL */

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
    return qbuf_data(&vec->buf) + offset;
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

void vector_destroy(struct vector* vec, void* arg_for_callback, void (*cb)(void*, void*)) {
    if (vec && cb) {
        struct deleter_arg arg = { .f = cb, .arg_for_callback = arg_for_callback };
        vector_foreach(vec, &arg, deleter);
        qbuf_destroy(&vec->buf);
        vec->sizeof_item = 0;
    }
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

void* vector_at(struct vector* vec, unsigned int idx) {
    unsigned int offset = qbuf_size(&vec->buf) + vec->sizeof_item * idx;
    return qbuf_data(&vec->buf) + offset;
}
