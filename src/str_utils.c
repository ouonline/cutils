#include <stdio.h>
#include <ctype.h>
#include <string.h>

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
