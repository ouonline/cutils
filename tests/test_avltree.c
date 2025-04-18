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
            int len = sprintf(tmp, "%d,", data->val);
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

static void destroy_func(struct avl_node* node) {
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

static void test_insert_rotate_left() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;
    struct test_data* data;
    struct avl_node* tmp;

    struct qbuf res_str;
    qbuf_init(&res_str);

    int values[] = {1, 2};
    for (int i = 0; i < sizeof(values) / sizeof(int); ++i) {
        data = malloc(sizeof(struct test_data));
        data->val = values[i];
        tmp = avl_insert(&data->node, &tree, cmp_func);
        assert(tmp == &data->node);
    }

    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp(qbuf_data(&res_str), "1,,2", qbuf_size(&res_str)) == 0);

    data = malloc(sizeof(struct test_data));
    data->val = 3;
    tmp = avl_insert(&data->node, &tree, cmp_func);
    assert(tmp == &data->node);

    qbuf_clear(&res_str);
    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp(qbuf_data(&res_str), "2,1,3", qbuf_size(&res_str)) == 0);

    qbuf_destroy(&res_str);
    avl_destroy(&tree, destroy_func);
}

static void test_insert_rotate_right() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;
    struct test_data* data;
    struct avl_node* tmp;

    struct qbuf res_str;
    qbuf_init(&res_str);

    int values[] = {3, 2};
    for (int i = 0; i < sizeof(values) / sizeof(int); ++i) {
        data = malloc(sizeof(struct test_data));
        data->val = values[i];
        tmp = avl_insert(&data->node, &tree, cmp_func);
        assert(tmp == &data->node);
    }

    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp(qbuf_data(&res_str), "3,2", qbuf_size(&res_str)) == 0);

    data = malloc(sizeof(struct test_data));
    data->val = 1;
    tmp = avl_insert(&data->node, &tree, cmp_func);
    assert(tmp == &data->node);

    qbuf_clear(&res_str);
    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp(qbuf_data(&res_str), "2,1,3", qbuf_size(&res_str)) == 0);

    qbuf_destroy(&res_str);
    avl_destroy(&tree, destroy_func);
}

static void test_insert_rotate_leftright() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;
    struct test_data* data;
    struct avl_node* tmp;

    struct qbuf res_str;
    qbuf_init(&res_str);

    int values[] = {3, 1};
    for (int i = 0; i < sizeof(values) / sizeof(int); ++i) {
        data = malloc(sizeof(struct test_data));
        data->val = values[i];
        tmp = avl_insert(&data->node, &tree, cmp_func);
        assert(tmp == &data->node);
    }

    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp(qbuf_data(&res_str), "3,1", qbuf_size(&res_str)) == 0);

    data = malloc(sizeof(struct test_data));
    data->val = 2;
    tmp = avl_insert(&data->node, &tree, cmp_func);
    assert(tmp == &data->node);

    qbuf_clear(&res_str);
    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp(qbuf_data(&res_str), "2,1,3", qbuf_size(&res_str)) == 0);

    qbuf_destroy(&res_str);
    avl_destroy(&tree, destroy_func);
}

static void test_insert_rotate_rightleft() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;
    struct test_data* data;
    struct avl_node* tmp;

    struct qbuf res_str;
    qbuf_init(&res_str);

    int values[] = {1, 3};
    for (int i = 0; i < sizeof(values) / sizeof(int); ++i) {
        data = malloc(sizeof(struct test_data));
        data->val = values[i];
        tmp = avl_insert(&data->node, &tree, cmp_func);
        assert(tmp == &data->node);
    }

    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp(qbuf_data(&res_str), "1,,3", qbuf_size(&res_str)) == 0);

    data = malloc(sizeof(struct test_data));
    data->val = 2;
    tmp = avl_insert(&data->node, &tree, cmp_func);
    assert(tmp == &data->node);

    qbuf_clear(&res_str);
    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp(qbuf_data(&res_str), "2,1,3", qbuf_size(&res_str)) == 0);

    qbuf_destroy(&res_str);
    avl_destroy(&tree, destroy_func);
}

static void test_insert_norotate() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;
    struct test_data* data;
    struct avl_node* tmp;

    struct qbuf res_str;
    qbuf_init(&res_str);

    int values[] = {2, 1};
    for (int i = 0; i < sizeof(values) / sizeof(int); ++i) {
        data = malloc(sizeof(struct test_data));
        data->val = values[i];
        tmp = avl_insert(&data->node, &tree, cmp_func);
        assert(tmp == &data->node);
    }

    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp(qbuf_data(&res_str), "2,1", qbuf_size(&res_str)) == 0);

    data = malloc(sizeof(struct test_data));
    data->val = 3;
    tmp = avl_insert(&data->node, &tree, cmp_func);
    assert(tmp == &data->node);

    qbuf_clear(&res_str);
    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp(qbuf_data(&res_str), "2,1,3", qbuf_size(&res_str)) == 0);

    qbuf_destroy(&res_str);
    avl_destroy(&tree, destroy_func);
}

static void test_delete_rorate_left() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;
    struct test_data* data;

    struct qbuf res_str;
    qbuf_init(&res_str);

#define N 8
    for (int i = 1; i <= N; ++i) {
        data = malloc(sizeof(struct test_data));
        data->val = i;
        avl_insert(&data->node, &tree, cmp_func);
    }
#undef N

    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp("4,2,6,1,3,5,7,,,,,,,,8", qbuf_data(&res_str), qbuf_size(&res_str)) == 0);

    data = lookup(&tree, 5);
    assert(data != NULL);

    avl_delete(&data->node, &tree);
    qbuf_clear(&res_str);
    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp("4,2,7,1,3,6,8", qbuf_data(&res_str), qbuf_size(&res_str)) == 0);

    qbuf_destroy(&res_str);
    avl_destroy(&tree, destroy_func);
}

static void test_delete_rorate_right() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;
    struct test_data* data;

    struct qbuf res_str;
    qbuf_init(&res_str);

#define N 8
    for (int i = N; i >= 1; --i) {
        data = malloc(sizeof(struct test_data));
        data->val = i;
        avl_insert(&data->node, &tree, cmp_func);
    }
#undef N

    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp("5,3,7,2,4,6,8,1", qbuf_data(&res_str), qbuf_size(&res_str)) == 0);

    data = lookup(&tree, 4);
    assert(data != NULL);

    avl_delete(&data->node, &tree);
    qbuf_clear(&res_str);
    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp("5,2,7,1,3,6,8", qbuf_data(&res_str), qbuf_size(&res_str)) == 0);

    qbuf_destroy(&res_str);
    avl_destroy(&tree, destroy_func);
}

static void test_delete_rotate_leftright() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;
    struct test_data* data;

    struct qbuf res_str;
    qbuf_init(&res_str);

    int values[] = {6, 3, 8, 4};
    for (int i = 0; i < sizeof(values) / sizeof(int); ++i) {
        data = malloc(sizeof(struct test_data));
        data->val = values[i];
        struct avl_node* tmp = avl_insert(&data->node, &tree, cmp_func);
        assert(tmp == &data->node);
    }

    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp("6,3,8,,4", qbuf_data(&res_str), qbuf_size(&res_str)) == 0);

    data = lookup(&tree, 8);
    assert(data != NULL);

    avl_delete(&data->node, &tree);
    qbuf_clear(&res_str);
    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp("4,3,6", qbuf_data(&res_str), qbuf_size(&res_str)) == 0);

    qbuf_destroy(&res_str);
    avl_destroy(&tree, destroy_func);
}

static void test_delete_rotate_rightleft() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;
    struct test_data* data;

    struct qbuf res_str;
    qbuf_init(&res_str);

    int values[] = {6, 3, 8, 7};
    for (int i = 0; i < sizeof(values) / sizeof(int); ++i) {
        data = malloc(sizeof(struct test_data));
        data->val = values[i];
        struct avl_node* tmp = avl_insert(&data->node, &tree, cmp_func);
        assert(tmp == &data->node);
    }

    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp("6,3,8,,,7", qbuf_data(&res_str), qbuf_size(&res_str)) == 0);

    data = lookup(&tree, 3);
    assert(data != NULL);

    avl_delete(&data->node, &tree);
    qbuf_clear(&res_str);
    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp("7,6,8", qbuf_data(&res_str), qbuf_size(&res_str)) == 0);

    qbuf_destroy(&res_str);
    avl_destroy(&tree, destroy_func);
}

static void test_delete_nororate() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;
    struct test_data* data;

    struct qbuf res_str;
    qbuf_init(&res_str);

#define N 8
    for (int i = 1; i <= N; ++i) {
        data = malloc(sizeof(struct test_data));
        data->val = i;
        avl_insert(&data->node, &tree, cmp_func);
    }

    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp("4,2,6,1,3,5,7,,,,,,,,8", qbuf_data(&res_str), qbuf_size(&res_str)) == 0);

    data = lookup(&tree, N);
    assert(data != NULL);
#undef N

    avl_delete(&data->node, &tree);

    qbuf_clear(&res_str);
    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp("4,2,6,1,3,5,7", qbuf_data(&res_str), qbuf_size(&res_str)) == 0);

    qbuf_destroy(&res_str);
    avl_destroy(&tree, destroy_func);
}

static void test_delete_rotate_sibling() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;
    struct test_data* data;

    struct qbuf res_str;
    qbuf_init(&res_str);

    int values[] = {7, 3, 9, 1, 4};
    for (int i = 0; i < sizeof(values) / sizeof(int); ++i) {
        data = malloc(sizeof(struct test_data));
        data->val = values[i];
        struct avl_node* tmp = avl_insert(&data->node, &tree, cmp_func);
        assert(tmp == &data->node);
    }

    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp("7,3,9,1,4", qbuf_data(&res_str), qbuf_size(&res_str)) == 0);

    data = lookup(&tree, 9);
    assert(data != NULL);

    avl_delete(&data->node, &tree);
    qbuf_clear(&res_str);
    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp("3,1,7,,,4", qbuf_data(&res_str), qbuf_size(&res_str)) == 0);

    qbuf_destroy(&res_str);
    avl_destroy(&tree, destroy_func);
}

static void test_delete_non_leaf() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;
    struct test_data* data;

    struct qbuf res_str;
    qbuf_init(&res_str);

#define N 8
    for (int i = 1; i <= N; ++i) {
        data = malloc(sizeof(struct test_data));
        data->val = i;
        avl_insert(&data->node, &tree, cmp_func);
    }

    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp("4,2,6,1,3,5,7,,,,,,,,8", qbuf_data(&res_str), qbuf_size(&res_str)) == 0);
#undef N

    data = lookup(&tree, 4);
    assert(data != NULL);

    avl_delete(&data->node, &tree);

    qbuf_clear(&res_str);
    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp("5,2,7,1,3,6,8", qbuf_data(&res_str), qbuf_size(&res_str)) == 0);

    qbuf_destroy(&res_str);
    avl_destroy(&tree, destroy_func);
}

static void test_delete_bugfix0() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;
    struct test_data* data;

    struct qbuf res_str;
    qbuf_init(&res_str);

    int values[] = {7, 3, 9, 1, 10, 5, 2, 4, 6};
    for (int i = 0; i < sizeof(values) / sizeof(int); ++i) {
        data = malloc(sizeof(struct test_data));
        data->val = values[i];
        struct avl_node* tmp = avl_insert(&data->node, &tree, cmp_func);
        assert(tmp == &data->node);
    }

    qbuf_clear(&res_str);
    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp("7,3,9,1,5,,10,,2,4,6", qbuf_data(&res_str), qbuf_size(&res_str)) == 0);

    data = lookup(&tree, 7);
    assert(data != NULL);
    avl_delete(&data->node, &tree);
    free(data);

    printf("***** delete [7] *****\n");
    qbuf_clear(&res_str);
    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp("3,1,9,,2,5,10,,,4,6", qbuf_data(&res_str), qbuf_size(&res_str)) == 0);

    qbuf_destroy(&res_str);
    avl_destroy(&tree, destroy_func);
}

static void test_delete_bugfix1() {
    struct avl_root tree = AVL_ROOT_INITIALIZER;
    struct test_data* data;

    struct qbuf res_str;
    qbuf_init(&res_str);

    int values[] = {8, 7, 6, 5, 10, 3, 1, 4, 2, 9};
    for (int i = 0; i < sizeof(values) / sizeof(int); ++i) {
        data = malloc(sizeof(struct test_data));
        data->val = values[i];
        struct avl_node* tmp = avl_insert(&data->node, &tree, cmp_func);
        assert(tmp == &data->node);
    }

    qbuf_clear(&res_str);
    output_level_order(&tree, &res_str);
    printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    assert(memcmp("7,3,9,1,5,8,10,,2,4,6", qbuf_data(&res_str), qbuf_size(&res_str)) == 0);

    for (int i = 0; i < sizeof(values) / sizeof(int); ++i) {
        data = lookup(&tree, values[i]);
        assert(data != NULL);
        avl_delete(&data->node, &tree);
        free(data);

        printf("***** delete [%d] *****\n", values[i]);
        qbuf_clear(&res_str);
        output_level_order(&tree, &res_str);
        printf("%.*s\n", qbuf_size(&res_str), qbuf_data(&res_str));
    }

    qbuf_destroy(&res_str);
    avl_destroy(&tree, destroy_func);
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
    TEST_ITEM(test_delete_rorate_left),
    TEST_ITEM(test_delete_rorate_right),
    TEST_ITEM(test_delete_rotate_leftright),
    TEST_ITEM(test_delete_rotate_rightleft),
    TEST_ITEM(test_delete_nororate),
    TEST_ITEM(test_delete_rotate_sibling),
    TEST_ITEM(test_delete_non_leaf),
    TEST_ITEM(test_delete_bugfix0),
    TEST_ITEM(test_delete_bugfix1),
    {NULL, NULL},
};

int main(void) {
    for (int i = 0; g_test_suite[i].name; ++i) {
        printf("----- %s -----\n", g_test_suite[i].name);
        g_test_suite[i].func();
    }
    return 0;
}
