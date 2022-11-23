#include "cutils/robin_hood_hash.h"
#include "cutils/time_utils.h"
#include "test_utils.h"
#include <stdio.h>
#include <sys/time.h>

#include "xxhash.h"

#undef NDEBUG
#include <assert.h>

#define N 1999999

static const void* default_getkey(const void* data) {
    return data;
}

static int default_equal(const void* a, const void* b) {
    return (strcmp(a, b) == 0);
}

static unsigned long default_hash(const void* key) {
    return XXH64(key, strlen((const char*)key), 5);
}

static const struct robin_hood_hash_operations g_ops = {
    .getkey = default_getkey,
    .equal = default_equal,
    .hash = default_hash,
};

static void test_insert(struct robin_hood_hash* h, char** data) {
    printf("insert begins...\n");
    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    for (unsigned int i = 0; i < N; ++i) {
        void* ret = robin_hood_hash_insert(h, data[i]);
    }
    gettimeofday(&end, NULL);
    printf("insert ends...\ninserting [%u]([%u] valid) strings costs [%lf] ms.\n",
           N, robin_hood_hash_size(h), (double)diff_time_usec(end, &begin) / 1000);
}

static void test_lookup(struct robin_hood_hash* h, char** data) {
    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    for (unsigned int i = N; i > 0; --i) {
        assert(robin_hood_hash_lookup(h, data[i - 1]) != NULL);
    }
    gettimeofday(&end, NULL);
    printf("find [%u] costs %lf ms.\n", N, diff_time_usec(end, &begin) / 1000);
}

int main(void) {
    printf("begin generating test data...\n");
    char** data = gen_random_str(N);

    struct robin_hood_hash h;
    robin_hood_hash_init(&h, N, ROBIN_HOOD_HASH_DEFAULT_MAX_LOAD_FACTOR, &g_ops);

    test_insert(&h, data);
    test_lookup(&h, data);

    robin_hood_hash_destroy(&h);
    return 0;
}
