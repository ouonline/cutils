#include "cutils/avltree.h"

static struct avl_node* avl_parent(const struct avl_node* node) {
    return (struct avl_node*)(node->parent_balance & ~3);
}

static void avl_set_parent(struct avl_node* node, struct avl_node* parent) {
    node->parent_balance = (node->parent_balance & 3) | (unsigned long)parent;
}

#define BST_NODE_TYPE struct avl_node
#define BST_ROOT_TYPE struct avl_root
#define BST_GET_PARENT avl_parent
#define BST_SET_PARENT avl_set_parent
#include "bst_common_template.h"

/* ------------------------------------------------------------------------- */

struct avl_node* avl_root(struct avl_node* node) {
    return bst_root(node);
}

struct avl_node* avl_first(struct avl_root* root) {
    return bst_first(root);
}

struct avl_node* avl_last(struct avl_root* root) {
    return bst_last(root);
}

struct avl_node* avl_next(struct avl_node* node) {
    return bst_next(node);
}

struct avl_node* avl_prev(struct avl_node* node) {
    return bst_prev(node);
}

void avl_destroy(struct avl_root* root, void (*del_func)(struct avl_node*)) {
    return bst_destroy(root, del_func);
}

static int avl_balance_factor(struct avl_node* node) {
    return (int)(node->parent_balance & 3) - 1;
}

static void avl_set_balance_factor(struct avl_node* node, int bf) {
    node->parent_balance = (node->parent_balance & ~3) | (bf + 1);
}

/* returns 1 if the subtree becomes shorter */
static int do_rotate(struct avl_node* node, struct avl_root* root, int node_bf) {
    if (node_bf == 2) {
        struct avl_node* r = node->right;
        int r_bf = avl_balance_factor(r);

        /* `r_bf` may be 0 if some node is removed on the left subtree of `node` */
        if (r_bf >= 0) {
            /*
             *     +---------+                     +---------+
             *     | node: 2 |                     | r: -1,0 |
             *     +---------+                     +---------+
             *     /         \                     /         \
             *  +---+   +--------+          +-----------+   +----+
             *  | l |   | r: 0,1 |    ==>   | node: 0,1 |   | rr |
             *  +---+   +--------+          +-----------+   +----+
             *          /        \             /     \
             *       +----+    +----+       +---+  +----+
             *       | rl |    | rr |       | l |  | rl |
             *       +----+    +----+       +---+  +----+
             */
            bst_rotate_left(node, root);
            if (r_bf == 0) {
                avl_set_balance_factor(node, 1);
                avl_set_balance_factor(r, -1);
                return 0;
            }

            avl_set_balance_factor(node, 0);
            avl_set_balance_factor(r, 0);
            return 1;
        }

        /*
         *      +---------+                  +------+                            +-------+
         *      | node: 2 |                  | node |                            | rl: 0 |
         *      +---------+                  +------+                            +-------+
         *     /           \                 /      \                           /         \
         *  +---+      +-------+          +---+    +----+            +------------+     +--------+
         *  | l |      | r: -1 |    ==>   | l |    | rl |      ==>   | node: -1,0 |     | r: 0,1 |
         *  +---+      +-------+          +---+    +----+            +------------+     +--------+
         *              /     \                     /  \                /      \         /      \
         *  +------------+   +----+          +-----+    +---+        +---+   +-----+  +-----+  +----+
         *  | rl: -1,0,1 |   | rr |          | rll |    | r |        | l |   | rll |  | rlr |  | rr |
         *  +------------+   +----+          +-----+    +---+        +---+   +-----+  +-----+  +----+
         *      /        \                                 \
         *   +-----+  +-----+                            +-----+
         *   | rll |  | rlr |                            | rlr |
         *   +-----+  +-----+                            +-----+
         */
        struct avl_node* rl = r->left;
        int rl_bf = avl_balance_factor(rl);
        bst_rotate_right(r, root);
        bst_rotate_left(node, root);
        if (rl_bf == 1) {
            avl_set_balance_factor(node, -1);
            avl_set_balance_factor(r, 0);
        } else if (rl_bf == -1) {
            avl_set_balance_factor(node, 0);
            avl_set_balance_factor(r, 1);
        } else /* rl_bf == 0 */ { /* deletion */
            avl_set_balance_factor(node, 0);
            avl_set_balance_factor(r, 0);
        }
        avl_set_balance_factor(rl, 0);
        return 1;
    }

    struct avl_node* l = node->left;
    int l_bf = avl_balance_factor(l);

    /* `l_bf` may be 0 if some node is removed on the right subtree of `node` */
    if (l_bf <= 0) {
        /*
         *         +----------+              +--------+
         *         | node: -2 |              | l: 0,1 |
         *         +----------+              +--------+
         *         /          \              /        \
         *    +---------+   +---+         +----+   +------------+
         *    | l: -1,0 |   | r |   ==>   | ll |   | node: -1,0 |
         *    +---------+   +---+         +----+   +------------+
         *     /       \                             /        \
         *  +----+   +----+                       +----+     +---+
         *  | ll |   | lr |                       | lr |     | r |
         *  +----+   +----+                       +----+     +---+
         */
        bst_rotate_right(node, root);
        if (l_bf == 0) {
            avl_set_balance_factor(node, -1);
            avl_set_balance_factor(l, 1);
            return 0;
        }

        avl_set_balance_factor(node, 0);
        avl_set_balance_factor(l, 0);
        return 1;
    }

    /*
     *           +----------+                    +------+                      +-------+
     *           | node: -2 |                    | node |                      | lr: 0 |
     *           +----------+                    +------+                      +-------+
     *           /          \                    /      \                       /     \
     *      +------+       +---+              +----+   +---+          +---------+     +-----------+
     *      | l: 1 |       | r |     ==>      | lr |   | r |    ==>   | l: -1,0 |     | node: 0,1 |
     *      +------+       +---+              +----+   +---+          +---------+     +-----------+
     *     /        \                        /      \                  /       \         /      \
     *  +----+    +------------+          +---+   +-----+          +----+   +-----+  +-----+   +---+
     *  | ll |    | lr: -1,0,1 |          | l |   | lrr |          | ll |   | lrl |  | lrr |   | r |
     *  +----+    +------------+          +---+   +-----+          +----+   +-----+  +-----+   +---+
     *               /      \            /     \
     *          +-----+    +-----+   +----+  +-----+
     *          | lrl |    | lrr |   | ll |  | lrl |
     *          +-----+    +-----+   +----+  +-----+
     */
    struct avl_node* lr = l->right;
    int lr_bf = avl_balance_factor(lr);
    bst_rotate_left(l, root);
    bst_rotate_right(node, root);
    if (lr_bf == -1) {
        avl_set_balance_factor(node, 1);
        avl_set_balance_factor(l, 0);
    } else if (lr_bf == 1) {
        avl_set_balance_factor(node, 0);
        avl_set_balance_factor(l, -1);
    } else /* lr_bf == 0 */ { /* deletion */
        avl_set_balance_factor(node, 0);
        avl_set_balance_factor(l, 0);
    }
    avl_set_balance_factor(lr, 0);
    return 1;
}

void avl_insert_rebalance(struct avl_node* node, struct avl_root* root) {
    struct avl_node* parent = avl_parent(node);
    while (parent) {
        int parent_bf = avl_balance_factor(parent);
        if (node == parent->left) {
            --parent_bf;
        } else {
            ++parent_bf;
        }

        if (parent_bf == 0) {
            avl_set_balance_factor(parent, 0);
            return;
        }

        if (parent_bf == 1 || parent_bf == -1) {
            avl_set_balance_factor(parent, parent_bf);
            node = parent;
            parent = avl_parent(parent);
            continue;
        }

        do_rotate(parent, root, parent_bf);
        return;
    }
}

struct avl_node* avl_insert(struct avl_node* node, struct avl_root* root,
                            int (*cmp_func)(struct avl_node*, struct avl_node*)) {
    struct avl_node *parent = NULL, **cursor = &root->node;

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

    avl_link_node(node, parent, cursor);
    avl_insert_rebalance(node, root);

    return node;
}

static void avl_delete_rebalance(struct avl_node* node, struct avl_root* root, int node_bf) {
    while (1) {
        /* height of subtree is not changed */
        if (node_bf == 1 || node_bf == -1) {
            avl_set_balance_factor(node, node_bf);
            return;
        }

        struct avl_node* parent = avl_parent(node);
        int is_left = (parent && (node == parent->left));

        if (node_bf == 0) {
            avl_set_balance_factor(node, node_bf);
        } else /* node_bf == 2 || node_bf == -2 */ {
            int shorter = do_rotate(node, root, node_bf);
            if (!shorter) {
                return;
            }
        }

        if (!parent) {
            return;
        }

        node = parent;
        node_bf = avl_balance_factor(parent) + (is_left ? 1 : -1);
    }
}

void avl_delete(struct avl_node* node, struct avl_root* root) {
    int parent_bf = 0;
    struct avl_node *parent, *child;

    if (node->left && node->right) {
        struct avl_node* old = node;
        struct avl_node* old_parent = avl_parent(node);

        /* finds predecessor of the deleted node */
        node = node->right;
        while (node->left) {
            node = node->left;
        }

        if (old_parent) {
            if (old == old_parent->left) {
                old_parent->left = node;
            } else {
                old_parent->right = node;
            }
        } else {
            root->node = node;
        }

        parent = avl_parent(node);
        if (parent == old) {
            parent = node;
            parent_bf = avl_balance_factor(old) - 1;
        } else {
            child = node->right;
            if (child) {
                avl_set_parent(child, parent);
            }
            parent->left = child;
            node->right = old->right;
            avl_set_parent(old->right, node);
            parent_bf = avl_balance_factor(parent) + 1;
        }

        node->parent_balance = old->parent_balance;
        node->left = old->left;
        avl_set_parent(old->left, node);
        goto rebalance;
    }

    if (node->left) {
        child = node->left;
    } else {
        child = node->right;
    }

    parent = avl_parent(node);

    if (child) {
        avl_set_parent(child, parent);
    }

    if (!parent) {
        root->node = child;
        return;
    }

    if (node == parent->left) {
        parent->left = child;
        parent_bf = avl_balance_factor(parent) + 1;
    } else {
        parent->right = child;
        parent_bf = avl_balance_factor(parent) - 1;
    }

rebalance:
    avl_delete_rebalance(parent, root, parent_bf);
}
