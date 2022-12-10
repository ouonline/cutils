#ifndef __CUTILS_HASH_FUNC_H__
#define __CUTILS_HASH_FUNC_H__

static inline unsigned long bkd_hash(const char* str, unsigned int len) {
    unsigned int i;
    const unsigned int seed = 131;
    unsigned long value = 0;

    for (i = 0; i < len; ++i) {
        value = value * seed + str[i];
    }

    return value;
}

#endif
