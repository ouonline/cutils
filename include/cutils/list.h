#ifndef __CUTILS_LIST_H__
#define __CUTILS_LIST_H__

/* doubly-linked list operations from linux kernel */

#include "utils.h"

#define list_entry(ptr, type, member) container_of(ptr, type, member)

struct list_node {
    struct list_node *prev, *next;
};

#define DEF_LIST_NODE(p) struct list_node p = {&p, &p}

static inline void list_init(struct list_node* list) {
    list->prev = list;
    list->next = list;
}

static inline void list_add_prev(struct list_node* node,
                                 struct list_node* next) {
    node->next = next;
    node->prev = next->prev;
    next->prev = node;
    node->prev->next = node;
}

static inline void list_add_next(struct list_node* node,
                                 struct list_node* prev) {
    node->prev = prev;
    node->next = prev->next;
    prev->next = node;
    node->next->prev = node;
}

static inline void __list_del(struct list_node* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

static inline void list_del(struct list_node* node) {
    __list_del(node);
    list_init(node);
}

static inline struct list_node* list_first(struct list_node* head) {
    return head->next;
}

static inline struct list_node* list_last(struct list_node* head) {
    return head->prev;
}

static inline struct list_node* list_next(struct list_node* node) {
    return node->next;
}

static inline struct list_node* list_prev(struct list_node* node) {
    return node->prev;
}

static inline int list_empty(struct list_node* head) {
    return (head->next == head);
}

/* move one list to another */
static inline void list_move_prev(struct list_node* src,
                                  struct list_node* dst) {
    if (!list_empty(src)) {
        src->next->prev = dst->prev;
        src->prev->next = dst;
        dst->prev->next = src->next;
        dst->prev = src->prev;
        list_init(src);
    }
}

static inline void list_move_next(struct list_node* src,
                                  struct list_node* dst) {
    if (!list_empty(src)) {
        src->next->prev = dst;
        src->prev->next = dst->next;
        dst->next->prev = src->prev;
        dst->next = src->next;
        list_init(src);
    }
}

#define list_for_each(p, head) \
    for ((p) = (head)->next; (p) != (head); (p) = (p)->next)

#define list_for_each_safe(p, n, head) \
    for ((p) = (head)->next, (n) = (p)->next; \
         (p) != (head); \
         (p) = (n), (n) = (p)->next)

#define list_for_each_reverse(p, head) \
    for ((p) = (head)->prev; (p) != (head); (p) = (p)->prev)

#define list_for_each_reverse_safe(p, n, head) \
    for ((p) = (head)->prev, (n) = (p)->prev; \
         (p) != (head); \
         (p) = (n), (n) = (p)->prev)

#define list_for_each_from(p, pos, head) \
    for ((p) = (pos); (p) != (head); (p) = (p)->next)

#define list_for_each_from_safe(p, n, pos, head) \
    for ((p) = (pos), (n) = (p)->next; \
         (p) != (head); \
         (p) = (n), (n) = (p)->next)

#define list_for_each_reverse_from(p, pos, head) \
    for ((p) = (pos); (p) != (head); (p) = (p)->prev)

#define list_for_each_reverse_from_safe(p, n, pos, head) \
    for ((p) = (pos), (n) = (p)->prev; \
         (p) != (head); \
         (p) = (n), (n) = (p)->prev)

#endif
