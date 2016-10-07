#ifndef __HASH_H__
#define __HASH_H__

#include "list.h"
#include <pthread.h>

struct hash_head {
    struct hlist_head head;
    pthread_rwlock_t lock;
};

unsigned long simple_hash11(const char* str, unsigned int len);
unsigned long simple_hash31(const char* str, unsigned int len);
unsigned long djb_hash(const char* str, unsigned int len);
unsigned long sdbm_hash(const char* str, unsigned int len);
unsigned long dek_hash(const char* str, unsigned int len);
unsigned long bp_hash(const char* str, unsigned int len);
unsigned long pjw_hash(const char* str, unsigned int len);

#endif
