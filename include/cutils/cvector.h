#ifndef __CUTILS_CVECTOR_H__
#define __CUTILS_CVECTOR_H__

#include "qbuf.h"

struct cvector {
    struct qbuf buf;
    unsigned int sizeof_item;
};

static inline void cvector_init(struct cvector* vec, unsigned int sizeof_item) {
    qbuf_init(&vec->buf);
    vec->sizeof_item = sizeof_item;
}

/** elements should be destroyed first */
static inline void cvector_destroy(struct cvector* vec) {
    qbuf_destroy(&vec->buf);
}

static inline void cvector_clear(struct cvector* vec) {
    qbuf_clear(&vec->buf);
}

/** returns 0 if success */
#define cvector_push_back(_vec_, _type_, _element_)                     \
    ({                                                                  \
        int _old_size_ = qbuf_size(&(_vec_)->buf);                      \
        int _ret_ = qbuf_resize(&(_vec_)->buf, _old_size_ + (_vec_)->sizeof_item); \
        if (_ret_ == 0) {                                               \
            char* _data_ = (char*)qbuf_data(&(_vec_)->buf);             \
            *(_type_*)(_data_ + _old_size_) = (_element_);              \
        }                                                               \
        _ret_;                                                          \
    })

static inline void* cvector_at(struct cvector* vec, unsigned int idx) {
    return (char*)qbuf_data(&vec->buf) + vec->sizeof_item * idx;
}

/** returns 0 if success */
static inline int cvector_reserve(struct cvector* vec, unsigned int newsize) {
    return qbuf_reserve(&vec->buf, vec->sizeof_item * newsize);
}

static inline int cvector_empty(struct cvector* vec) {
    return (qbuf_size(&vec->buf) == 0);
}

static inline void* cvector_data(struct cvector* vec) {
    return qbuf_data(&vec->buf);
}

static inline unsigned int cvector_size(struct cvector* vec) {
    return qbuf_size(&vec->buf) / vec->sizeof_item;
}

#endif
