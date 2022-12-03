#ifndef __CUTILS_ROBIN_HOOD_HASH_H__
#define __CUTILS_ROBIN_HOOD_HASH_H__

struct robin_hood_hash_meta {
    unsigned int data_num;
    unsigned int lpsl;
    unsigned int max_data_num;
    unsigned int slot_num;
    float max_load_factor; /* for rehash */
};

struct robin_hood_hash_node {
    unsigned int psl;
    void* data;
};

struct robin_hood_hash_operations {
    /* gets key from the given data */
    const void* (*getkey)(const void* data);
    /* returns non-zero means equal */
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

/* returns pointer to data, or NULL if not found */
void* robin_hood_hash_lookup(struct robin_hood_hash*, const void* key);

/* iterates each data in hash table. */
void robin_hood_hash_foreach(struct robin_hood_hash*, void* arg_for_callback,
                             void (*f)(void* data, void* arg));

/*
  returns the newly inserted data or the existed data, or NULL if `max_load_factor` is reached
  and rehash fails.
*/
void* robin_hood_hash_insert(struct robin_hood_hash*, void* data);

/* returns the removed data, or NULL if not found. */
void* robin_hood_hash_remove(struct robin_hood_hash*, const void* key);

void robin_hood_hash_destroy(struct robin_hood_hash*);

static inline unsigned int robin_hood_hash_size(struct robin_hood_hash* h) {
    return h->meta.data_num;
}

#endif
