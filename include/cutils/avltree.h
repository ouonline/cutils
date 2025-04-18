#ifndef __CUTILS_AVLTREE_H__
#define __CUTILS_AVLTREE_H__

#include "utils.h"

struct
#ifdef _MSC_VER
__declspec(align(8))
#else
__attribute__((aligned(sizeof(long))))
#endif
avl_node {
    /*
      Last 2 bits of the parent pointer is used to record this node's balance factor,
      which is in range [-1, 1] and mapped to [0, 2] by adding 1.
    */
    unsigned long parent_balance;
    struct avl_node *left, *right;
};

struct avl_root {
    struct avl_node* node;
};

#define AVL_ROOT_INITIALIZER {NULL}

static inline void avl_init(struct avl_root* root) {
    root->node = NULL;
}

#define avl_empty(root) (!(root)->node)

#define avl_entry(ptr, type, member) container_of(ptr, type, member)

static inline void avl_link_node(struct avl_node* node, struct avl_node* parent,
                                 struct avl_node** link) {
    node->parent_balance = (unsigned long)parent | 1;
    node->left = node->right = NULL;
    *link = node;
}

void avl_insert_rebalance(struct avl_node*, struct avl_root*);

/* returns the newly inserted node if ok, or the existing one if fails. */
struct avl_node* avl_insert(struct avl_node*, struct avl_root*,
                            int (*cmp_func)(struct avl_node*, struct avl_node*));

void avl_delete(struct avl_node*, struct avl_root*);

struct avl_node* avl_root(struct avl_node*);
struct avl_node* avl_first(struct avl_root*);
struct avl_node* avl_last(struct avl_root*);
struct avl_node* avl_next(struct avl_node*);
struct avl_node* avl_prev(struct avl_node*);
void avl_destroy(struct avl_root*, void (*del_func)(struct avl_node*));

#endif
