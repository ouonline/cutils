#ifndef __CUTILS_OFFLEN_H__
#define __CUTILS_OFFLEN_H__

#ifdef __cplusplus
extern "C" {
#endif

struct offlen {
    unsigned long off;
    unsigned long len;
};

#ifdef __cplusplus
typedef struct offlen OffLen;
#endif

static inline void offlen_reset(struct offlen* item) {
    item->off = 0;
    item->len = 0;
}

#ifdef __cplusplus
}
#endif

#endif
