#ifndef __CUTILS_RBTREE_H__
#define __CUTILS_RBTREE_H__

#include "utils.h"

struct
#ifdef _MSC_VER
__declspec(align(8))
#else
__attribute__((aligned(sizeof(long))))
#endif
rb_node {
    /*
      Last bit of the parent pointer is used to record this node's color,
      0 for red and 1 for black.
    */
    unsigned long parent_color;
    union {
        struct {
            struct rb_node* left;
            struct rb_node* right;
        };
        struct rb_node* child[2];
    };
};

struct rb_root {
    struct rb_node* node;
};

#define RB_ROOT_INITIALIZER {NULL}

static inline void rb_init(struct rb_root* root) {
    root->node = NULL;
}

void rb_destroy(struct rb_root* root, void (*del_func)(struct rb_node*));

#define rb_empty(root) (!((root)->node))

#define rb_entry(ptr, type, member) container_of(ptr, type, member)

static inline void rb_link_node(struct rb_node* node, struct rb_node* parent,
                                struct rb_node** link) {
    node->parent_color = (unsigned long)parent;
    node->left = node->right = NULL;
    *link = node;
}

void rb_insert_rebalance(struct rb_node* node, struct rb_root* root);

/* returns the newly inserted node if ok, or the existing one if fails */
struct rb_node* rb_insert(struct rb_node* node, struct rb_root* root,
                          int (*cmp_func)(struct rb_node*, struct rb_node*));

void rb_delete(struct rb_node* node, struct rb_root* root);

struct rb_node* rb_root(struct rb_node* node);
struct rb_node* rb_first(struct rb_root* root);
struct rb_node* rb_last(struct rb_root* root);
struct rb_node* rb_next(struct rb_node* node);
struct rb_node* rb_prev(struct rb_node* node);

#endif
