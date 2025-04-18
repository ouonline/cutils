#include "cutils/robin_hood_hash.h"
#include "cutils/time_utils.h"
#include "cutils/hash_func.h"
#include "test_utils.h"
#include <stdio.h>
#include <sys/time.h>

#undef NDEBUG
#include <assert.h>

#define N 1999999

static const void* default_getkey(const void* data) {
    return data;
}

static int default_equal(const void* a, const void* b) {
    return strcmp(a, b);
}

static unsigned long default_hash(const void* key) {
    return bkd_hash(key, strlen((const char*)key));
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
        robin_hood_hash_insert(h, data[i], data[i]);
    }
    gettimeofday(&end, NULL);
    printf("insert ends...\ninserting [%u]([%u] valid) strings costs [%lf] ms.\n",
           N, robin_hood_hash_size(h), (double)diff_time_usec(end, begin) / 1000);
}

static void test_lookup(struct robin_hood_hash* h, char** data) {
    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    for (unsigned int i = N; i > 0; --i) {
        assert(robin_hood_hash_lookup(h, data[i - 1]) != NULL);
    }
    gettimeofday(&end, NULL);
    printf("lookup [%u] costs %lf ms.\n", N, (double)diff_time_usec(end, begin) / 1000);
}

static void test_remove(struct robin_hood_hash* h, char** data) {
    for (unsigned int i = N; i > 0; --i) {
        if (robin_hood_hash_lookup(h, data[i - 1]) == NULL) {
            continue; /* duplicate items */
        }
        assert(robin_hood_hash_remove(h, data[i - 1]) != NULL);
    }
    printf("test remove ok\n");
}

static void test_rehash(char** data) {
    printf("--- norehash insert -----\n");
    struct robin_hood_hash norehash;
    robin_hood_hash_init(&norehash, N * 2, ROBIN_HOOD_HASH_DEFAULT_MAX_LOAD_FACTOR, &g_ops);
    test_insert(&norehash, data);
    printf("lpsl = %u\n", norehash.meta.lpsl);

    printf("--- rehash insert -----\n");
    struct robin_hood_hash h;
    robin_hood_hash_init(&h, 10, ROBIN_HOOD_HASH_DEFAULT_MAX_LOAD_FACTOR, &g_ops);
    test_insert(&h, data);
    printf("lpsl = %u\n", h.meta.lpsl);

    printf("--- rehash lookup -----\n");
    test_lookup(&h, data);
    printf("lpsl = %u\n", h.meta.lpsl);

    printf("--- rehash lookup again -----\n");
    test_lookup(&h, data);
    printf("lpsl = %u\n", h.meta.lpsl);

    printf("test rehash ok\n");
}

int main(void) {
    printf("begin generating test data...\n");
    char** data = gen_random_str(N);

    struct robin_hood_hash h;
    robin_hood_hash_init(&h, N * 2, ROBIN_HOOD_HASH_DEFAULT_MAX_LOAD_FACTOR, &g_ops);

    printf("----------- test insert -------------\n");
    test_insert(&h, data);
    printf("----------- test lookup -------------\n");
    test_lookup(&h, data);
    printf("----------- test remove -------------\n");
    test_remove(&h, data);

    printf("----------- test rehash -------------\n");
    test_rehash(data);

    robin_hood_hash_destroy(&h, NULL, NULL);

    return 0;
}
