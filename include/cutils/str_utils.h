#ifndef __CUTILS_STR_UTILS_H__
#define __CUTILS_STR_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

long dec2long(const char* src);
long ndec2long(const char* src, unsigned int len);

long bin2long(const char* src);

long hex2long(const char* src);
long nhex2long(const char* src, unsigned int len);

void* memmem(const void* text, unsigned long tlen, const void* pattern, unsigned long plen);

#ifdef __cplusplus
}
#endif

#endif
