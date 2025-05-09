#include "cutils/avltree.h"

static struct avl_node* avl_parent(const struct avl_node* node) {
    return (struct avl_node*)(node->parent_balance & ~3);
}

static void avl_set_parent(struct avl_node* node, struct avl_node* parent) {
    node->parent_balance = (unsigned long)parent + (node->parent_balance & 3);
}

#define BST_NODE_TYPE struct avl_node
#define BST_ROOT_TYPE struct avl_root
#define BST_GET_PARENT avl_parent
#define BST_SET_PARENT avl_set_parent
#include "bst_common_template.h"

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

void avl_destroy(struct avl_root* root, void* arg,
                 void (*del_func)(struct avl_node*, void*)) {
    bst_destroy(root, arg, del_func);
}

/* ------------------------------------------------------------------------- */

static int avl_balance_factor(struct avl_node* node) {
    return (int)(node->parent_balance & 3) - 1;
}

static void avl_set_balance_factor(struct avl_node* node, int bf) {
    node->parent_balance = (node->parent_balance & ~3) | (bf + 1);
}

static int signs_are_opposite(int x, int y) {
    return (x * y < 0);
}

/* returns 1 if the subtree becomes shorter */
static int do_rotate(struct avl_node* node, struct avl_root* root, int node_bf) {
    int direction = (node_bf > 0);
    int opposite = 1 - direction;
    struct avl_node* child = node->child[direction];
    int child_bf = avl_balance_factor(child);
    int child_subtree_is_leaning_opposite = signs_are_opposite(child_bf, node_bf);

    if (!child_subtree_is_leaning_opposite) {
        /*
         *     +---------+                        +-------------+
         *     | node: 2 |                        | child: -1,0 |
         *     +---------+                        +-------------+
         *     /         \                          /         \
         *  +---+   +------------+          +-----------+   +----+
         *  | l |   | child: 0,1 |    ==>   | node: 0,1 |   | rr |
         *  +---+   +------------+          +-----------+   +----+
         *            /        \               /     \
         *         +----+    +----+         +---+  +----+
         *         | rl |    | rr |         | l |  | rl |
         *         +----+    +----+         +---+  +----+
         *
         * or:
         *
         *          +----------+              +------------+
         *          | node: -2 |              | child: 0,1 |
         *          +----------+              +------------+
         *          /          \                /        \
         *  +-------------+   +---+          +----+   +------------+
         *  | child: -1,0 |   | r |   ==>    | ll |   | node: -1,0 |
         *  +-------------+   +---+          +----+   +------------+
         *     /       \                                /        \
         *  +----+   +----+                          +----+     +---+
         *  | ll |   | lr |                          | lr |     | r |
         *  +----+   +----+                          +----+     +---+
         */
        bst_rotate(node, root, opposite);

        /* `child_bf` may be 0 if some node is removed on the subtree of `node`'s direction. */
        if (child_bf == 0) {
            /*
              if the subtree of `child` is balanced before rotating, it will become
              leaning after rotation.
            */
            int lean_bf = node_bf / 2;
            avl_set_balance_factor(node, lean_bf);
            avl_set_balance_factor(child, -lean_bf);
            return 0;
        }

        avl_set_balance_factor(node, 0);
        avl_set_balance_factor(child, 0);
        return 1;
    }

    /*
     *        +---------+                     +------+                          +-----------+
     *        | node: 2 |                     | node |                          | gchild: 0 |
     *        +---------+                     +------+                          +-----------+
     *         /       \                      /      \                           /         \
     *      +---+   +-----------+          +---+  +--------+            +------------+   +------------+
     *      | l |   | child: -1 |    ==>   | l |  | gchild |      ==>   | node: -1,0 |   | child: 0,1 |
     *      +---+   +-----------+          +---+  +--------+            +------------+   +------------+
     *               /         \                   /      \               /       \         /       \
     *  +----------------+   +----+           +-----+  +-------+       +---+   +-----+   +-----+  +----+
     *  | gchild: -1,0,1 |   | rr |           | rll |  | child |       | l |   | rll |   | rlr |  | rr |
     *  +----------------+   +----+           +-----+  +-------+       +---+   +-----+   +-----+  +----+
     *      /        \                                  /     \
     *   +-----+  +-----+                           +-----+  +----+
     *   | rll |  | rlr |                           | rlr |  | rr |
     *   +-----+  +-----+                           +-----+  +----+
     *
     * or:
     *
     *         +----------+                           +------+                      +-----------+
     *         | node: -2 |                           | node |                      | gchild: 0 |
     *         +----------+                           +------+                      +-----------+
     *          /        \                            /      \                        /       \
     *   +----------+   +---+                  +--------+   +---+        +-------------+   +-----------+
     *   | child: 1 |   | r |       ==>        | gchild |   | r |  ==>   | child: -1,0 |   | node: 0,1 |
     *   +----------+   +---+                  +--------+   +---+        +-------------+   +-----------+
     *     /      \                             /      \                    /       \        /       \
     *  +----+  +----------------+        +-------+   +-----+           +----+  +-----+   +-----+  +---+
     *  | ll |  | gchild: -1,0,1 |        | child |   | lrr |           | ll |  | lrl |   | lrr |  | r |
     *  +----+  +----------------+        +-------+   +-----+           +----+  +-----+   +-----+  +---+
     *               /      \              /     \
     *          +-----+    +-----+     +----+  +-----+
     *          | lrl |    | lrr |     | ll |  | lrl |
     *          +-----+    +-----+     +----+  +-----+
     */
    struct avl_node* gchild = child->child[opposite];
    int gchild_bf = avl_balance_factor(gchild);
    int gchild_subtree_is_leaning_opposite = signs_are_opposite(gchild_bf, node_bf);

    bst_rotate(child, root, direction);
    bst_rotate(node, root, opposite);

    avl_set_balance_factor(node, gchild_subtree_is_leaning_opposite ? 0 : -gchild_bf);
    avl_set_balance_factor(child, gchild_subtree_is_leaning_opposite ? -gchild_bf : 0);
    avl_set_balance_factor(gchild, 0);

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
            int direction = bst_direction(old, old_parent);
            old_parent->child[direction] = node;
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

    int direction = bst_direction(node, parent);
    parent->child[direction] = child;
    parent_bf = avl_balance_factor(parent) + (direction ? -1 : 1);

rebalance:
    avl_delete_rebalance(parent, root, parent_bf);
}
