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
unsigned long qbuf_size(const struct qbuf* q);
void* qbuf_data(const struct qbuf* q);
int qbuf_empty(const struct qbuf* q);
void qbuf_clear(struct qbuf* q);
int qbuf_reserve(struct qbuf* q, unsigned long expected_size);
int qbuf_resize(struct qbuf* q, unsigned long expected_size);
int qbuf_append(struct qbuf* q, const void* data, unsigned long size);
int qbuf_append_c(struct qbuf* q, char c);
int qbuf_assign(struct qbuf* q, const void* data, unsigned long size);
void qbuf_swap(struct qbuf* a, struct qbuf* b);

#ifdef __cplusplus
}
#endif

#endif
