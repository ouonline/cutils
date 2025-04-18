#include "cutils/rbtree.h"
#include "cutils/avltree.h"
#include "cutils/random/xoshiro256ss.h"
#include "cutils/time_utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define N 1000000

struct avl_test_data {
    uint64_t val;
    struct avl_node node;
};

struct rb_test_data {
    uint64_t val;
    struct rb_node node;
};

static uint64_t* g_ints;
static struct avl_test_data* g_avl_data;
static struct rb_test_data* g_rb_data;
static struct avl_root g_avl_tree = AVL_ROOT_INITIALIZER;
static struct rb_root g_rb_tree = RB_ROOT_INITIALIZER;

static void gen_rand_ints() {
    g_ints = malloc(sizeof(uint64_t) * N);

    struct xoshiro256ss rnd;
    uint64_t seed = time(NULL);
    printf("seed = %lu\n", seed);
    xoshiro256ss_init(&rnd, seed);
    for (uint64_t i = 0; i < N; ++i) {
        g_ints[i] = xoshiro256ss_next(&rnd);
    }
}

static void gen_avl_test_data() {
    g_avl_data = malloc(sizeof(struct avl_test_data) * N);
    for (uint64_t i = 0; i < N; ++i) {
        g_avl_data[i].val = g_ints[i];
    }
}

static void gen_rb_test_data() {
    g_rb_data = malloc(sizeof(struct rb_test_data) * N);
    for (uint64_t i = 0; i < N; ++i) {
        g_rb_data[i].val = g_ints[i];
    }
}

static int avl_cmp_func(struct avl_node* a, struct avl_node* b) {
    const struct avl_test_data* aa = avl_entry(a, struct avl_test_data, node);
    const struct avl_test_data* bb = avl_entry(b, struct avl_test_data, node);
    if (aa->val < bb->val) {
        return -1;
    }
    if (aa->val > bb->val) {
        return 1;
    }
    return 0;
}

static int rb_cmp_func(struct rb_node* a, struct rb_node* b) {
    const struct rb_test_data* aa = rb_entry(a, struct rb_test_data, node);
    const struct rb_test_data* bb = rb_entry(b, struct rb_test_data, node);
    if (aa->val < bb->val) {
        return -1;
    }
    if (aa->val > bb->val) {
        return 1;
    }
    return 0;
}

static void test_rb_insert() {
    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    for (uint64_t i = 0; i < N; ++i) {
        rb_insert(&g_rb_data[i].node, &g_rb_tree, rb_cmp_func);
    }
    gettimeofday(&end, NULL);
    printf("rbtree insert [%lu] nodes costs [%f] ms.\n", N,
           diff_time_usec(end, begin) / 1000.0);
}

static void test_avl_insert() {
    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    for (uint64_t i = 0; i < N; ++i) {
        avl_insert(&g_avl_data[i].node, &g_avl_tree, avl_cmp_func);
    }
    gettimeofday(&end, NULL);
    printf("avltree insert [%lu] nodes costs [%f] ms.\n", N,
           diff_time_usec(end, begin) / 1000.0);
}

static struct avl_node* avl_lookup(struct avl_root* root, uint64_t val) {
    struct avl_node* node = root->node;
    while (node) {
        struct avl_test_data* data = avl_entry(node, struct avl_test_data, node);
        if (val == data->val) {
            return node;
        }
        if (val < data->val) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return NULL;
}

static struct rb_node* rb_lookup(struct rb_root* root, uint64_t val) {
    struct rb_node* node = root->node;
    while (node) {
        struct rb_test_data* data = rb_entry(node, struct rb_test_data, node);
        if (val == data->val) {
            return node;
        }
        if (val < data->val) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return NULL;
}

static void test_rb_lookup() {
    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    for (uint64_t i = 0; i < N; ++i) {
        struct rb_node* node = rb_lookup(&g_rb_tree, g_ints[i]);
    }
    gettimeofday(&end, NULL);
    printf("rbtree lookup [%lu] nodes costs [%f] ms.\n", N,
           diff_time_usec(end, begin) / 1000.0);
}

static void test_avl_lookup() {
    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    for (uint64_t i = 0; i < N; ++i) {
        struct avl_node* node = avl_lookup(&g_avl_tree, g_ints[i]);
    }
    gettimeofday(&end, NULL);
    printf("avltree lookup [%lu] nodes costs [%f] ms.\n", N,
           diff_time_usec(end, begin) / 1000.0);
}

static void test_rb_delete() {
    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    for (uint64_t i = 0; i < N; ++i) {
        struct rb_node* node = rb_lookup(&g_rb_tree, g_ints[i]);
        rb_delete(node, &g_rb_tree);
    }
    gettimeofday(&end, NULL);
    printf("rbtree delete [%lu] nodes costs [%f] ms.\n", N,
           diff_time_usec(end, begin) / 1000.0);
}

static void test_avl_delete() {
    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    for (uint64_t i = 0; i < N; ++i) {
        struct avl_node* node = avl_lookup(&g_avl_tree, g_ints[i]);
        avl_delete(node, &g_avl_tree);
    }
    gettimeofday(&end, NULL);
    printf("avltree delete [%lu] nodes costs [%f] ms.\n", N,
           diff_time_usec(end, begin) / 1000.0);
}

int main(void) {
    gen_rand_ints();
    gen_rb_test_data();
    gen_avl_test_data();
    test_rb_insert();
    test_avl_insert();
    test_rb_lookup();
    test_avl_lookup();
    test_rb_delete();
    test_avl_delete();
    return 0;
}
