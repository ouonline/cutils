#ifndef __QBUF_H__
#define __QBUF_H__

struct qbuf {
    char* base;
    unsigned int len;
};

struct qbuf_ref {
    const char* base;
    unsigned int len;
};

#endif
