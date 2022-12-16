#include <stdio.h>
#include "cutils/robin_hood_hash.h"
#include <stdlib.h>
#include <limits.h>

static void mark_slot_empty(struct robin_hood_hash_node* node) {
    node->psl = UINT_MAX;
}

static int slot_is_empty(const struct robin_hood_hash_node* node) {
    return (node->psl == UINT_MAX);
}

int robin_hood_hash_init(struct robin_hood_hash* h, unsigned int slot_num, float max_load_factor,
                         const struct robin_hood_hash_operations* ops) {
    unsigned int max_value_num = slot_num * max_load_factor;

    h->table = (struct robin_hood_hash_node*)malloc(slot_num * sizeof(struct robin_hood_hash_node));
    if (!h->table) {
        return 1;
    }

    h->ops = ops;
    h->meta.value_num = 0;
    h->meta.lpsl = 0;
    h->meta.max_value_num = max_value_num;
    h->meta.slot_num = slot_num;
    h->meta.max_load_factor = max_load_factor;

    for (unsigned int i = 0; i < slot_num; ++i) {
        mark_slot_empty(&h->table[i]);
    }

    return 0;
}

static unsigned int do_lookup(const struct robin_hood_hash* h, const void* key) {
    unsigned int slot = h->ops->hash(key) % h->meta.slot_num;

    for (unsigned int psl = 0; psl <= h->meta.lpsl; ++psl) {
        if (slot_is_empty(&h->table[slot]) || psl > h->table[slot].psl) {
            return UINT_MAX;
        }

        const void* key_in = h->ops->getkey(h->table[slot].value);
        if (h->ops->equal(key, key_in) == 0) {
            return slot;
        }

        slot = (slot + 1) % h->meta.slot_num;
    }

    return UINT_MAX;
}

void* robin_hood_hash_lookup(struct robin_hood_hash* h, const void* key) {
    unsigned int slot = do_lookup(h, key);
    return (slot == UINT_MAX) ? NULL : h->table[slot].value;
}

void robin_hood_hash_foreach(struct robin_hood_hash* h, void* arg_for_callback,
                             void (*f)(void* value, void* arg)) {
    for (unsigned int i = 0; i < h->meta.slot_num; ++i) {
        if (slot_is_empty(&h->table[i])) {
            continue;
        }

        f(h->table[i].value, arg_for_callback);
    }
}

static void swap_node(struct robin_hood_hash_node* a, struct robin_hood_hash_node* b) {
    struct robin_hood_hash_node tmp = *a;
    *a = *b;
    *b = tmp;
}

static void rehash_callback(void* value, void* arg) {
    struct robin_hood_hash* new_hash = (struct robin_hood_hash*)arg;
    const void* key = new_hash->ops->getkey(value);
    robin_hood_hash_insert(new_hash, key, value);
}

/* TODO optimize */
static int rehash(struct robin_hood_hash* h) {
    unsigned int new_slot_num = h->meta.slot_num * 2;

    struct robin_hood_hash new_hash;
    int err = robin_hood_hash_init(&new_hash, new_slot_num, h->meta.max_load_factor, h->ops);
    if (err) {
        return err;
    }

    robin_hood_hash_foreach(h, &new_hash, rehash_callback);

    struct robin_hood_hash tmp = *h;
    *h = new_hash;
    robin_hood_hash_destroy(&tmp, NULL, NULL);
    return 0;
}

struct robin_hood_hash_insertion_res robin_hood_hash_insert(struct robin_hood_hash* h,
                                                            const void* key, void* value) {
    struct robin_hood_hash_insertion_res res;

    struct robin_hood_hash_meta* meta = &h->meta;
    if (meta->value_num >= meta->max_value_num) {
        int err = rehash(h);
        if (err) {
            res.inserted = 0;
            res.pvalue = NULL;
            return res;
        }
    }

    struct robin_hood_hash_node* table = h->table;
    unsigned int slot = h->ops->hash(key) % meta->slot_num;

    struct robin_hood_hash_node tmp_node;
    tmp_node.psl = 0;
    tmp_node.value = value;

    while (1) {
        if (slot_is_empty(&table[slot])) {
            if (tmp_node.psl > meta->lpsl) {
                meta->lpsl = tmp_node.psl;
            }

            table[slot] = tmp_node;
            ++meta->value_num;

            res.inserted = 1;
            if (tmp_node.value == value) {
                res.pvalue = &table[slot].value;
            }
            return res;
        }

        if (tmp_node.value == value) {
            const void* key_existed = h->ops->getkey(table[slot].value);
            if (h->ops->equal(key, key_existed) == 0) {
                res.inserted = 0;
                res.pvalue = &table[slot].value;
                return res;
            }
        }

        if (tmp_node.psl > table[slot].psl) {
            if (tmp_node.psl > meta->lpsl) {
                meta->lpsl = tmp_node.psl;
            }
            swap_node(&tmp_node, &table[slot]);

            if (table[slot].value == value) {
                res.pvalue = &table[slot].value;
            }
        }

        ++tmp_node.psl;
        slot = (slot + 1) % meta->slot_num;
    }

    /* unreachable */
    res.inserted = 0;
    res.pvalue = NULL;
    return res;
}

void* robin_hood_hash_remove(struct robin_hood_hash* h, const void* key) {
    unsigned int slot = do_lookup(h, key);
    if (slot == UINT_MAX) {
        return NULL;
    }

    void* ret = h->table[slot].value;
    --h->meta.value_num;

    unsigned int next_slot = (slot + 1) % h->meta.slot_num;
    while (!slot_is_empty(&h->table[next_slot]) && h->table[next_slot].psl > 0) {
        h->table[slot] = h->table[next_slot];
        --h->table[slot].psl;
        slot = next_slot;
        next_slot = (next_slot + 1) % h->meta.slot_num;
    }

    mark_slot_empty(&h->table[slot]);
    return ret;
}

void robin_hood_hash_destroy(struct robin_hood_hash* h, void* arg_for_callback,
                             void (*destroy)(void* value, void* arg)) {
    if (h->table) {
        if (destroy) {
            robin_hood_hash_foreach(h, arg_for_callback, destroy);
        }
        free(h->table);
    }
}
