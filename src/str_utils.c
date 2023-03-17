#include <ctype.h>
#include <string.h>
#include <limits.h>
#include "cutils/str_utils.h"

static int hex_value[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 0 - 15 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 16 - 31 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 32 - 47 */
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, /* 48 - 63 */
    0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 64 - 79 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 80 - 95 */
    0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 96 - 111 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 112 - 127 */
};

long ndec2long(const char* src, unsigned int len) {
    unsigned int i = 0;
    for (; i < len; ++i) {
        if (!isspace(src[i])) {
            break;
        }
    }
    if (i == len) {
        return 0;
    }

    int is_negative = 0;
    if (src[i] == '-') {
        is_negative = 1;
        ++i;
    } else if (src[i] == '+') {
        ++i;
    } else if (!isdigit(src[i])) {
        return 0;
    }

    long result = 0;

    for (; i < len; ++i) {
        if (!isdigit(src[i])) {
            break;
        }
        result = result * 10 + (src[i] - '0');
    }

    return (is_negative) ? -result : result;
}

long dec2long(const char* src) {
    return ndec2long(src, strlen(src));
}

long nhex2long(const char* src, unsigned int len) {
    long result = 0;

    unsigned int i = 0;
    for (; i < len; ++i) {
        if (!isspace(src[i])) {
            break;
        }
    }
    for (i = 0; i < len; ++i) {
        if (!isxdigit(src[i])) {
            return result;
        }
        result = (result << 4) + hex_value[(int)(src[i])];
    }

    return result;
}

long hex2long(const char* src) {
    return nhex2long(src, strlen(src));
}

long bin2long(const char* src) {
    long result = 0;

    unsigned int i;
    for (i = 0; src[i]; ++i) {
        if (!isspace(src[i])) {
            break;
        }
    }
    for (; src[i]; ++i) {
        if (src[i] != '0' && src[i] != '1') {
            return result;
        }
        result = (result << 1) + (src[i] - '0');
    }

    return result;
}

/* ------------------------------------------------------------------------- */

/* https://opensource.apple.com/source/Libc/Libc-825.40.1/string/FreeBSD/memmem.c.auto.html */
void* memmem(const void* l, unsigned long l_len, const void* s, unsigned long s_len) {
    register char *cur, *last;
    const char *cl = (const char *)l;
    const char *cs = (const char *)s;

    /* we need something to compare */
    if (l_len == 0 || s_len == 0) {
        return NULL;
    }

    /* "s" must be smaller or equal to "l" */
    if (l_len < s_len) {
        return NULL;
    }

    /* special case where s_len == 1 */
    if (s_len == 1) {
        return memchr(l, (int)*cs, l_len);
    }

    /* the last position where its possible to find "s" in "l" */
    last = (char *)cl + l_len - s_len;

    for (cur = (char*)cl; cur <= last; cur++) {
        if (cur[0] == cs[0] && memcmp(cur, cs, s_len) == 0) {
            return cur;
        }
    }

    return NULL;
}

/* ------------------------------------------------------------------------- */

#ifdef SUNDAY_DEBUG
#include <stdio.h>
static void debug_compare(const char *text, int tlen,
                          const char *pattern, int plen,
                          int i, int j) {
    int c, k;

    if (tlen > 10) {
        printf("     ");
        for (c = 1; c < tlen / 10; ++c) {
            printf("                   %d", c);
        }
        if (tlen % 10 != 0) {
            printf("                   %d", c);
        }
        printf("\n");
    }
    printf("pos");
    for (c = 0; c <= tlen / 10; ++c) {
        for (k = 0; k < 10; ++k) {
            if (c * 10 + k < tlen) {
                printf(" %d", k);
            }
        }
    }
    printf("\n");

    printf("T  ");
    for (c = 0; c < tlen; ++c) {
        printf(" %c", text[c]);
    }
    printf("\n");

    printf("P  ");
    for (c = 0; c < i - j; ++c) {
        printf("  ");
    }
    for (c = 0; c < plen; ++c) {
        printf(" %c", pattern[c]);
    }
    printf("\n");

    printf("   ");
    for (c = 0; c < i; ++c) {
        printf("  ");
    }
    printf(" *\n");

    printf("---------------------------------\n");
}

static int my_memcmp(const char* text, int tlen,
                     const char* pattern, int plen, int idx) {
    int i;

    for (i = plen - 1; i >= 0; --i) {
        debug_compare(text, tlen, pattern, plen, idx + i, i);
        if (text[idx + i] != pattern[i]) {
            return text[idx + i] - pattern[i];
        }
    }

    return 0;
}
#else
#define my_memcmp(text, tlen, pattern, plen, idx)   \
    memcmp(text + idx, pattern, plen)
#endif

void sunday_preprocess(struct sunday_context* ctx, const char* pattern,
                       unsigned int plen) {
    unsigned int i;

    for (i = 0; i < SUNDAY_ALPHABET_LEN; ++i) {
        ctx->pos[i] = plen + 1;
    }

    for (i = 0; i < plen; ++i) {
        ctx->pos[(unsigned char)pattern[i]] = plen - i;
    }

    ctx->pattern = pattern;
    ctx->plen = plen;
}

unsigned int sunday_match(const struct sunday_context* ctx, const char* text,
                          unsigned int tlen) {
    int x = 0;

    while (1) {
        if (x + ctx->plen > tlen) {
            break;
        }

        if (my_memcmp(text, tlen, ctx->pattern, ctx->plen, x) == 0) {
            return x;
        }

        if (x + ctx->plen == tlen) {
            break;
        }

        x += ctx->pos[(unsigned char)text[x + ctx->plen]];
    }

    return UINT_MAX;
}
