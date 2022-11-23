#ifndef __CUTILS_BITMAP_H__
#define __CUTILS_BITMAP_H__

#include "utils.h"

/* `offset` is counted from right to left, starting from 0. */
static inline void bitmap_set_r2l(unsigned char* bitarray, unsigned long bytes,
                                  unsigned long offset) {
    unsigned int rest = offset & 0x7;
    bytes -= (offset >> 3); // end position
    bitarray[bytes - 1] |= (1u << rest);
}

/* `offset` is counted from right to left, starting from 0. */
static inline void bitmap_clear_r2l(unsigned char* bitarray, unsigned long bytes,
                                    unsigned long offset) {
    unsigned int rest = offset & 0x7;
    bytes -= (offset >> 3); // end position
    bitarray[bytes - 1] &= ~(1u << rest);
}

/* `offset` is counted from right to left, starting from 0. */
static inline int bitmap_test_r2l(unsigned char* bitarray, unsigned long bytes,
                                  unsigned long offset) {
    unsigned int rest = offset & 0x7;
    bytes -= (offset >> 3); // end position
    return (bitarray[bytes - 1] & (1u << rest));
}


/* `offset` is counted from left to right, starting from 0. */
static inline void bitmap_set_l2r(unsigned char* bitarray, unsigned long offset) {
    unsigned int rest = offset & 0x7;

    offset -= rest;
    if (offset) {
        bitarray += (offset >> 3);
    }

    (*bitarray) |= (1u << (7 - rest));
}

/* `offset` is counted from left to right, starting from 0. */
static inline void bitmap_clear_l2r(unsigned char* bitarray, unsigned long offset) {
    unsigned int rest = offset & 0x7;

    offset -= rest;
    if (offset) {
        bitarray += (offset >> 3);
    }

    (*bitarray) &= ~(1u << (7 - rest));
}

/* `offset` is counted from left to right, starting from 0. */
static inline int bitmap_test_l2r(unsigned char* bitarray, unsigned long offset) {
    unsigned int rest = offset & 0x7;

    offset -= rest;
    if (offset) {
        bitarray += (offset >> 3);
    }

    return ((*bitarray) & (1u << (7 - rest)));
}

#endif
