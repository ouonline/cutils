#ifndef __CUTILS_BST_COMMON_TEMPLATE_H__
#define __CUTILS_BST_COMMON_TEMPLATE_H__

static int bst_direction(BST_NODE_TYPE* node, BST_NODE_TYPE* parent) {
    return (node == parent->right);
}

/*
 *     px             px              px          px
 *     |              |               |           |
 *     x              y               x           y
 *    / \     =>     / \     or      / \    =>   / \
 *  xl   y          x   yr          y   xr      yl  x
 *      / \        / \             / \             / \
 *    yl   yr    xl   yl         yl   yr          yr  xr
 */
static void bst_rotate(BST_NODE_TYPE* x, BST_ROOT_TYPE* root, int direction) {
    int opposite = 1 - direction;
    BST_NODE_TYPE* y = x->child[opposite];
    BST_NODE_TYPE* px = BST_GET_PARENT(x);

    x->child[opposite] = y->child[direction];
    BST_SET_PARENT(y, px);
    if (y->child[direction]) {
        BST_SET_PARENT(y->child[direction], x);
    }

    if (px) {
        px->child[bst_direction(x, px)] = y;
    } else {
        root->node = y;
    }

    y->child[direction] = x;
    BST_SET_PARENT(x, y);
}

static BST_NODE_TYPE* bst_root(BST_NODE_TYPE* node) {
    BST_NODE_TYPE* parent;
    while ((parent = BST_GET_PARENT(node))) {
        node = parent;
    }
    return node;
}

static BST_NODE_TYPE* bst_first(BST_ROOT_TYPE* root) {
    BST_NODE_TYPE* n = root->node;
    if (n) {
        while (n->left) {
            n = n->left;
        }
    }
    return n;
}

static BST_NODE_TYPE* bst_last(BST_ROOT_TYPE* root) {
    BST_NODE_TYPE* n = root->node;
    if (n) {
        while (n->right) {
            n = n->right;
        }
    }
    return n;
}

static BST_NODE_TYPE* bst_next(BST_NODE_TYPE* node) {
    BST_NODE_TYPE* parent;

    if (node->right) {
        node = node->right;
        while (node->left) {
            node = node->left;
        }
        return node;
    }

    parent = BST_GET_PARENT(node);
    while (parent && node == parent->right) {
        node = parent;
        parent = BST_GET_PARENT(node);
    }
    return parent;
}

static BST_NODE_TYPE* bst_prev(BST_NODE_TYPE* node) {
    BST_NODE_TYPE* parent;

    if (node->left) {
        node = node->left;
        while (node->right) {
            node = node->right;
        }
        return node;
    }

    parent = BST_GET_PARENT(node);
    while (parent && node == parent->left) {
        node = parent;
        parent = BST_GET_PARENT(node);
    }
    return parent;
}

static void bst_null_del(BST_NODE_TYPE* nil) {
    (void)nil;
}

static void bst_destroy(BST_ROOT_TYPE* root, void (*del_func)(BST_NODE_TYPE*)) {
    BST_NODE_TYPE* node = root->node;

    if (!del_func) {
        del_func = bst_null_del;
    }

    while (node) {
        if (node->left) {
            node = node->left;
        } else if (node->right) {
            node = node->right;
        } else {
            BST_NODE_TYPE* tmp = node;
            node = BST_GET_PARENT(node);
            if (node) {
                if (tmp == node->left) {
                    node->left = NULL;
                } else {
                    node->right = NULL;
                }
            }
            del_func(tmp);
        }
    }

    root->node = NULL;
}

#endif
