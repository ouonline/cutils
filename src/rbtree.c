#include "cutils/rbtree.h"

static struct rb_node* rb_parent(const struct rb_node* node) {
    return (struct rb_node*)(node->parent_color & ~1);
}

static void rb_set_parent(struct rb_node* node, struct rb_node* parent) {
    node->parent_color = (unsigned long)parent + (node->parent_color & 1);
}

#define BST_NODE_TYPE struct rb_node
#define BST_ROOT_TYPE struct rb_root
#define BST_GET_PARENT rb_parent
#define BST_SET_PARENT rb_set_parent
#include "bst_common_template.h"

struct rb_node* rb_root(struct rb_node* node) {
    return bst_root(node);
}

struct rb_node* rb_first(struct rb_root* root) {
    return bst_first(root);
}

struct rb_node* rb_last(struct rb_root* root) {
    return bst_last(root);
}

struct rb_node* rb_next(struct rb_node* node) {
    return bst_next(node);
}

struct rb_node* rb_prev(struct rb_node* node) {
    return bst_prev(node);
}

void rb_destroy(struct rb_root* root, void* arg,
                void (*del_func)(struct rb_node*, void*)) {
    bst_destroy(root, arg, del_func);
}

/* ------------------------------------------------------------------------- */

#define RB_RED    0
#define RB_BLACK  1

static int rb_color(const struct rb_node* node) {
    return node->parent_color & 1;
}

static void rb_set_color(struct rb_node* node, int color) {
    node->parent_color = (node->parent_color & ~1) | color;
}

#define rb_is_red(node)             (!rb_color(node))
#define rb_is_black(node)           rb_color(node)
#define rb_set_red(node)            ((node)->parent_color &= ~1)
#define rb_set_black(node)          ((node)->parent_color |= 1)

void rb_insert_rebalance(struct rb_node* node, struct rb_root* root) {
    struct rb_node* parent;

    while ((parent = rb_parent(node)) && rb_is_red(parent)) {
        struct rb_node* gparent = rb_parent(parent);
        int direction = bst_direction(parent, gparent);
        int opposite = 1 - direction;
        struct rb_node* uncle = gparent->child[opposite];

        if (uncle && rb_is_red(uncle)) {
            rb_set_black(uncle);
            rb_set_black(parent);
            rb_set_red(gparent);
            node = gparent;
            continue;
        }

        if (node == parent->child[opposite]) {
            bst_rotate(parent, root, direction);
            swap_value(node, parent);
        }

        bst_rotate(gparent, root, opposite);
        rb_set_red(gparent);
        rb_set_black(parent);
    }

    rb_set_black(root->node);
}

struct rb_node* rb_insert(struct rb_node* node, struct rb_root* root,
                          int (*cmp_func)(struct rb_node*, struct rb_node*)) {
    struct rb_node *parent = NULL, **cursor = &root->node;

    while (*cursor) {
        int diff;
        parent = *cursor;
        diff = cmp_func(node, parent);
        if (diff < 0) {
            cursor = &parent->left;
        } else if (diff > 0) {
            cursor = &parent->right;
        } else {
            return parent;
        }
    }

    rb_link_node(node, parent, cursor);
    rb_insert_rebalance(node, root);

    return node;
}

static void rb_delete_rebalance(struct rb_node* node,
                                struct rb_node* parent,
                                struct rb_root* root) {
    while ((!node || rb_is_black(node)) && (node != root->node)) {
        int direction = bst_direction(node, parent);
        int opposite = 1 - direction;
        struct rb_node* sibling = parent->child[opposite];

        if (rb_is_red(sibling)) {
            bst_rotate(parent, root, direction);
            rb_set_red(parent);
            rb_set_black(sibling);
            sibling = parent->child[opposite];
        }

        if ((!sibling->left || rb_is_black(sibling->left)) &&
            (!sibling->right || rb_is_black(sibling->right))) {
            rb_set_red(sibling);
            node = parent;
            parent = rb_parent(node);
        } else {
            if (!sibling->child[opposite] || rb_is_black(sibling->child[opposite])) {
                rb_set_black(sibling->child[direction]);
                rb_set_red(sibling);
                bst_rotate(sibling, root, opposite);
                sibling = parent->child[opposite];
            }
            rb_set_color(sibling, rb_color(parent));
            rb_set_black(parent);
            rb_set_black(sibling->child[opposite]);
            bst_rotate(parent, root, direction);
            node = root->node;
            break;
        }
    }

    if (node) {
        rb_set_black(node);
    }
}

void rb_delete(struct rb_node* node, struct rb_root* root) {
    int color;
    struct rb_node *parent, *child = NULL;

    if (node->left && node->right) {
        struct rb_node* old = node;
        struct rb_node* old_parent = rb_parent(node);

        node = node->right;
        while (node->left) {
            node = node->left;
        }

        if (old_parent) {
            int direction = bst_direction(old, old_parent);
            old_parent->child[direction] = node;
        } else {
            root->node = node;
        }

        child = node->right;
        parent = rb_parent(node);
        color = rb_color(node);

        if (parent == old) {
            parent = node;
        } else {
            if (child) {
                rb_set_parent(child, parent);
            }
            parent->left = child;
            node->right = old->right;
            rb_set_parent(old->right, node);
        }

        node->parent_color = old->parent_color;
        node->left = old->left;
        rb_set_parent(old->left, node);
        goto rebalance;
    }

    if (node->left) {
        child = node->left;
    } else {
        child = node->right;
    }

    parent = rb_parent(node);
    color = rb_color(node);

    if (child) {
        rb_set_parent(child, parent);
    }

    if (parent) {
        int direction = bst_direction(node, parent);
        parent->child[direction] = child;
    } else {
        root->node = child;
    }

rebalance:
    if (color == RB_BLACK) {
        rb_delete_rebalance(child, parent, root);
    }
}
