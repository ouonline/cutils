#ifndef __CUTILS_ROBIN_HOOD_HASH_H__
#define __CUTILS_ROBIN_HOOD_HASH_H__

struct robin_hood_hash_meta {
    unsigned int value_num;
    unsigned int lpsl;
    unsigned int max_value_num;
    unsigned int slot_num;
    float max_load_factor; /* for rehash */
};

struct robin_hood_hash_node {
    unsigned int psl;
    void* value;
};

struct robin_hood_hash_operations {
    /* gets key from the given value */
    const void* (*getkey)(const void* value);
    /* returns 0 if equal */
    int (*equal)(const void* akey, const void* bkey);
    /* calc the hash value of the given key */
    unsigned long (*hash)(const void* key);
};

struct robin_hood_hash {
    struct robin_hood_hash_meta meta;
    const struct robin_hood_hash_operations* ops;
    struct robin_hood_hash_node* table;
};

#define ROBIN_HOOD_HASH_DEFAULT_MAX_LOAD_FACTOR 0.9

/* returns 0 if success. */
int robin_hood_hash_init(struct robin_hood_hash*, unsigned int init_slot_num, float max_load_factor,
                         const struct robin_hood_hash_operations* ops);

/* returns pointer to value, or NULL if not found */
void* robin_hood_hash_lookup(struct robin_hood_hash*, const void* key);

/* iterates each value in hash table. */
void robin_hood_hash_foreach(struct robin_hood_hash*, void* arg_for_callback,
                             void (*f)(void* value, void* arg));


struct robin_hood_hash_insertion_res {
    /* 1 if inserted, 0 otherwise */
    int inserted;
    /*
      pointer to the newly inserted value or the existed value, or NULL if `max_load_factor`
      is reached and rehash failed.
    */
    void** pvalue;
};

/* NOTE parameter `key` is not required to be part of `value` and is not inserted */
struct robin_hood_hash_insertion_res robin_hood_hash_insert(struct robin_hood_hash*,
                                                            const void* key, void* value);

/* returns the removed value, or NULL if not found. */
void* robin_hood_hash_remove(struct robin_hood_hash*, const void* key);

void robin_hood_hash_destroy(struct robin_hood_hash*);

static inline unsigned int robin_hood_hash_size(struct robin_hood_hash* h) {
    return h->meta.value_num;
}

#endif
