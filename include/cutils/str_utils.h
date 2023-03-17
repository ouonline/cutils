#ifndef __CUTILS_STR_UTILS_H__
#define __CUTILS_STR_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------- */

long dec2long(const char* src);
long ndec2long(const char* src, unsigned int len);

long bin2long(const char* src);

long hex2long(const char* src);
long nhex2long(const char* src, unsigned int len);

/* ------------------------------------------------------------------------- */

void* memmem(const void* text, unsigned long tlen, const void* pattern, unsigned long plen);

/* ------------------------------------------------------------------------- */

#define SUNDAY_ALPHABET_LEN 256

struct sunday_context {
    int pos[SUNDAY_ALPHABET_LEN];
    const char* pattern;
    unsigned int plen;
};

void sunday_preprocess(struct sunday_context* ctx, const char* pattern,
                       unsigned int plen);
/* returns UINT_MAX if not found */
unsigned int sunday_match(const struct sunday_context* ctx, const char* text,
                          unsigned int tlen);

#ifdef __cplusplus
}
#endif

#endif
