#include "cutils/avltree.h"
#include "cutils/qbuf.h"
#include "cutils/list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#undef NDEBUG
#include <assert.h>

struct test_data {
    int val;
    struct avl_node node;
};

struct queue_data {
    struct test_data* data;
    struct list_node node;
};

static int avl_balance_factor(struct avl_node* node) {
    return (int)(node->parent_balance & 3) - 1;
}

/*
 * use python `binarytree` to display results. for example:
 *
 *     >>> import binarytree
 *     >>>
 *     >>> while True:
 *     >>>     try:
 *     >>>         nodes = input().split(",")
 *     >>>         tree = binarytree.build(nodes)
 *     >>>         print(tree)
 *     >>>     except EOFError:
 *     >>>         break
 *
 * then:
 *
 *    $ ./test_avltree | python3 print.py
 */
static void output_level_order(struct avl_root* root, struct qbuf* res_str) {
    if (avl_empty(root)) {
        return;
    }

    struct list_node print_queue;
    list_init(&print_queue);

    struct queue_data* qdata = malloc(sizeof(struct queue_data));
    qdata->data = avl_entry(root->node, struct test_data, node);
    list_add_prev(&qdata->node, &print_queue);

    while (!list_empty(&print_queue)) {
        struct list_node* node = list_first(&print_queue);
        list_del(node);

        struct queue_data* qdata = list_entry(node, struct queue_data, node);
        struct test_data* data = qdata->data;
        free(qdata);

        if (!data) {
            qbuf_append(res_str, ",", 1);
        } else {
            char tmp[32];
            int len = sprintf(tmp, "%d[%d],", data->val, avl_balance_factor(&data->node));
            qbuf_append(res_str, tmp, len);

            qdata = malloc(sizeof(struct queue_data));
            if (data->node.left) {
                qdata->data = avl_entry(data->node.left, struct test_data, node);
            } else {
                qdata->data = NULL;
            }
            list_add_prev(&qdata->node, &print_queue);

            qdata = malloc(sizeof(struct queue_data));
            if (data->node.right) {
                qdata->data = avl_entry(data->node.right, struct test_data, node);
            } else {
                qdata->data = NULL;
            }
            list_add_prev(&qdata->node, &print_queue);
        }
    }

    /* remove trailing ','s */
    const char* data = qbuf_data(res_str);
    unsigned long size = qbuf_size(res_str);
    while (size >= 1) {
        if (data[size - 1] != ',') {
            break;
        }
        --size;
    }
    qbuf_resize(res_str, size);
}

static int cmp_func(struct avl_node* a, struct avl_node* b) {
    const struct test_data* ta = avl_entry(a, struct test_data, node);
    const struct test_data* tb = avl_entry(b, struct test_data, node);
    return (ta->val - tb->val);
}

static void destroy_func(struct avl_node* node, void* unused) {
    struct test_data* data = avl_entry(node, struct test_data, node);
    free(data);
}

static struct test_data* lookup(struct avl_root* root, int val) {
    struct avl_node* node = root->node;
    while (node) {
        struct test_data* data = avl_entry(node, struct test_data, node);
        if (val == data->val) {
            return data;
        }
        if (val < data->val) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return NULL;
}

#define __insert_and_check(root, arr, expected_str)                                  \
    do {                                                                             \
        for (int i = 0; i < sizeof(arr) / sizeof(int); ++i) {                        \
            struct test_data* data = malloc(sizeof(struct test_data));               \
            data->val = arr[i];                                                      \
            struct avl_node* tmp = avl_insert(&data->node, &tree, cmp_func);         \
            assert(tmp == &data->node);                                              \
        }                                                                            \
        struct qbuf res_str;                                                         \
        qbuf_init(&res_str);                                                         \
        output_level_order(root, &res_str);                                          \
        printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));                  \
        fflush(stdout);                                                              \
        assert(memcmp(qbuf_data(&res_str), expected_str, qbuf_size(&res_str)) == 0); \
        qbuf_destroy(&res_str);                                                      \
    } while (0)

static void test_insert_rotate_left() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;

    int values1[] = {1, 2};
    __insert_and_check(&tree, values1, "1[1],,2[0]");

    int values2[] = {3};
    __insert_and_check(&tree, values2, "2[0],1[0],3[0]");

    avl_destroy(&tree, NULL, destroy_func);
}

static void test_insert_rotate_right() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;
    struct test_data* data;
    struct avl_node* tmp;

    int values1[] = {3, 2};
    __insert_and_check(&tree, values1, "3[-1],2[0]");

    int values2[] = {1};
    __insert_and_check(&tree, values2, "2[0],1[0],3[0]");

    avl_destroy(&tree, NULL, destroy_func);
}

static void test_insert_rotate_leftright() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;
    struct test_data* data;
    struct avl_node* tmp;

    int values1[] = {3, 1};
    __insert_and_check(&tree, values1, "3[-1],1[0]");

    int values2[] = {2};
    __insert_and_check(&tree, values2, "2[0],1[0],3[0]");

    avl_destroy(&tree, NULL, destroy_func);
}

static void test_insert_rotate_rightleft() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;
    struct test_data* data;
    struct avl_node* tmp;

    int values1[] = {1, 3};
    __insert_and_check(&tree, values1, "1[1],,3[0]");

    int values2[] = {2};
    __insert_and_check(&tree, values2, "2[0],1[0],3[0]");

    avl_destroy(&tree, NULL, destroy_func);
}

static void test_insert_norotate() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;

    int values1[] = {2, 1};
    __insert_and_check(&tree, values1, "2[-1],1[0]");

    int values2[] = {3};
    __insert_and_check(&tree, values2, "2[0],1[0],3[0]");

    avl_destroy(&tree, NULL, destroy_func);
}

#define __delete_and_check(root, val, expected_str)                                  \
    do {                                                                             \
        struct qbuf res_str;                                                         \
        qbuf_init(&res_str);                                                         \
        struct test_data* data = lookup(root, val);                                  \
        assert(data != NULL);                                                        \
        printf("***** delete [%d] *****\n", val);                                    \
        avl_delete(&data->node, root);                                               \
        output_level_order(root, &res_str);                                          \
        printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));                  \
        fflush(stdout);                                                              \
        assert(memcmp(expected_str, qbuf_data(&res_str), qbuf_size(&res_str)) == 0); \
        qbuf_destroy(&res_str);                                                      \
    } while (0)

static void test_delete_rotate_left() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;

    int values[] = {1, 2, 3, 4, 5, 6, 7, 8};
    __insert_and_check(&tree, values, "4[1],2[0],6[1],1[0],3[0],5[0],7[1],,,,,,,,8[0]");
    __delete_and_check(&tree, 5, "4[0],2[0],7[0],1[0],3[0],6[0],8[0]");

    avl_destroy(&tree, NULL, destroy_func);
}

static void test_delete_rotate_right() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;

    int values[] = {8, 7, 6, 5, 4, 3, 2, 1};
    __insert_and_check(&tree, values, "5[-1],3[-1],7[0],2[-1],4[0],6[0],8[0],1[0]");
    __delete_and_check(&tree, 4, "5[0],2[0],7[0],1[0],3[0],6[0],8[0]");

    avl_destroy(&tree, NULL, destroy_func);
}

static void test_delete_rotate_leftright() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;

    int values[] = {6, 3, 8, 4};
    __insert_and_check(&tree, values, "6[-1],3[1],8[0],,4[0]");
    __delete_and_check(&tree, 8, "4[0],3[0],6[0]");

    avl_destroy(&tree, NULL, destroy_func);
}

static void test_delete_rotate_rightleft() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;

    int values[] = {6, 3, 8, 7};
    __insert_and_check(&tree, values, "6[1],3[0],8[-1],,,7[0]");
    __delete_and_check(&tree, 3, "7[0],6[0],8[0]");

    avl_destroy(&tree, NULL, destroy_func);
}

static void test_delete_norotate() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;

    int values[] = {1, 2, 3, 4, 5, 6, 7, 8};
    __insert_and_check(&tree, values, "4[1],2[0],6[1],1[0],3[0],5[0],7[1],,,,,,,,8[0]");
    __delete_and_check(&tree, 8, "4[0],2[0],6[0],1[0],3[0],5[0],7[0]");

    avl_destroy(&tree, NULL, destroy_func);
}

static void test_delete_rotate_sibling() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;

    int values[] = {7, 3, 9, 1, 4};
    __insert_and_check(&tree, values, "7[-1],3[0],9[0],1[0],4[0]");
    __delete_and_check(&tree, 9, "3[1],1[0],7[-1],,,4[0]");

    avl_destroy(&tree, NULL, destroy_func);
}

static void test_delete_non_leaf() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;

    int values[] = {1, 2, 3, 4, 5, 6, 7, 8};
    __insert_and_check(&tree, values, "4[1],2[0],6[1],1[0],3[0],5[0],7[1],,,,,,,,8[0]");
    __delete_and_check(&tree, 4, "5[0],2[0],7[0],1[0],3[0],6[0],8[0]");

    avl_destroy(&tree, NULL, destroy_func);
}

static void test_delete_bugfix0() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;

    int values[] = {7, 3, 9, 1, 10, 5, 2, 4, 6};
    __insert_and_check(&tree, values, "7[-1],3[0],9[1],1[1],5[0],,10[0],,2[0],4[0],6[0]");
    __delete_and_check(&tree, 7, "3[1],1[1],9[-1],,2[0],5[0],10[0],,,4[0],6[0]");

    avl_destroy(&tree, NULL, destroy_func);
}

static void test_delete_all() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;

    int values[] = {8, 7, 6, 5, 10, 3, 1, 4, 2, 9};
    __insert_and_check(&tree, values, "7[-1],3[0],9[0],1[1],5[0],8[0],10[0],,2[0],4[0],6[0]");

    for (int i = 0; i < sizeof(values) / sizeof(int); ++i) {
        struct test_data* data = lookup(&tree, values[i]);
        assert(data != NULL);
        avl_delete(&data->node, &tree);
        free(data);
    }

    avl_destroy(&tree, NULL, destroy_func);
}

static void test_delete_shorter_subtree() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;

    int values[] = {12,4,19,2,7,16,22,3,5,9,13,17,20,18};
    __insert_and_check(&tree, values, "12[1],4[0],19[-1],2[1],7[0],16[1],22[-1],,3[0],5[0],9[0],13[0],17[1],20[0],,,,,,,,,,,18[0]");
    __delete_and_check(&tree, 13, "12[0],4[0],19[0],2[1],7[0],17[0],22[-1],,3[0],5[0],9[0],16[0],18[0],20[0]");

    avl_destroy(&tree, NULL, destroy_func);
}

static void test_delete_rl_another_tree() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;

    int values[] = {12,7,28,5,13,33,14};
    __insert_and_check(&tree, values, "12[1],7[-1],28[-1],5[0],,13[1],33[0],,,,14[0]");
    __delete_and_check(&tree, 7, "13[0],12[-1],28[0],5[0],,14[0],33[0]");

    avl_destroy(&tree, NULL, destroy_func);
}

static void test_delete_rotate_left2() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;

    int values[] = {20,13,35,8,15,32,39,1,9,14,16,29,33,40,34};
    __insert_and_check(&tree, values, "20[1],13[0],35[-1],8[0],15[0],32[1],39[1],1[0],9[0],14[0],16[0],29[0],33[1],,40[0],,,,,,,,,,,,34[0]");
    __delete_and_check(&tree, 39, "20[0],13[0],33[0],8[0],15[0],32[-1],35[0],1[0],9[0],14[0],16[0],29[0],,34[0],40[0]");

    avl_destroy(&tree, NULL, destroy_func);
}

#define TEST_ITEM(item) {#item, item}

static const struct {
    const char* name;
    void (*func)(void);
} g_test_suite[] = {
    TEST_ITEM(test_insert_rotate_left),
    TEST_ITEM(test_insert_rotate_right),
    TEST_ITEM(test_insert_rotate_leftright),
    TEST_ITEM(test_insert_rotate_rightleft),
    TEST_ITEM(test_insert_norotate),
    TEST_ITEM(test_delete_rotate_left),
    TEST_ITEM(test_delete_rotate_right),
    TEST_ITEM(test_delete_rotate_leftright),
    TEST_ITEM(test_delete_rotate_rightleft),
    TEST_ITEM(test_delete_norotate),
    TEST_ITEM(test_delete_rotate_sibling),
    TEST_ITEM(test_delete_non_leaf),
    TEST_ITEM(test_delete_bugfix0),
    TEST_ITEM(test_delete_all),
    TEST_ITEM(test_delete_shorter_subtree),
    TEST_ITEM(test_delete_rl_another_tree),
    TEST_ITEM(test_delete_rotate_left2),
    {NULL, NULL},
};

int main(void) {
    for (int i = 0; g_test_suite[i].name; ++i) {
        printf("----- %s -----\n", g_test_suite[i].name);
        g_test_suite[i].func();
    }
    return 0;
}
