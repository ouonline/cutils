#ifndef __UTILS_HASH_H__
#define __UTILS_HASH_H__

#ifdef __cplusplus
extern "C" {
#endif

unsigned long hash11(const char* str, unsigned int len);
unsigned long hash31(const char* str, unsigned int len);
unsigned long djb_hash(const char* str, unsigned int len);
unsigned long sdbm_hash(const char* str, unsigned int len);
unsigned long dek_hash(const char* str, unsigned int len);
unsigned long bp_hash(const char* str, unsigned int len);
unsigned long pjw_hash(const char* str, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif
