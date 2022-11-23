#ifndef __CUTILS_HLIST_H__
#define __CUTILS_HLIST_H__

/* hash list from linux kernel */

#include "utils.h"

#define hlist_entry(ptr, type, member) container_of(ptr, type, member)

struct hlist_node {
    struct hlist_node *next, **pprev;
};

struct hlist_head {
    struct hlist_node* first;
};

static inline void hlist_init_head(struct hlist_head* head) {
    head->first = NULL;
}

static inline void hlist_init(struct hlist_node* node) {
    node->next = NULL;
    node->pprev = NULL;
}

static inline void hlist_add_head(struct hlist_node* node,
                                  struct hlist_head* head) {
    struct hlist_node* first = head->first;

    node->next = first;
    if (first) {
        first->pprev = &(node->next);
    }
    head->first = node;
    node->pprev = &(head->first);
}

static inline int hlist_unhashed(struct hlist_node* node) {
    return !(node->pprev);
}

static inline int hlist_empty(struct hlist_head* head) {
    return !(head->first);
}

static inline void __hlist_del(struct hlist_node* node) {
    struct hlist_node* next = node->next;
    struct hlist_node** pprev = node->pprev;

    *pprev = next;
    if (next) {
        next->pprev = pprev;
    }
}

static inline void hlist_del(struct hlist_node* node) {
    __hlist_del(node);
    hlist_init(node);
}

#define hlist_for_each(p, head) \
    for ((p) = (head)->first; (p); (p) = (p)->next)

#define hlist_for_each_safe(p, n, head) \
    for ((p) = (head)->first; (p) && ((n) = (p)->next, 1); (p) = (n))

#define hlist_for_each_from(p, pos, head) \
    for ((p) = (pos); (p) && ((n) = (p)->next, 1); (p) = (n))

#define hlist_for_each_from_safe(p, n, pos, head) \
    for ((p) = (pos); (p) && ((n) = (p)->next, 1); (p) = (n))

#endif
