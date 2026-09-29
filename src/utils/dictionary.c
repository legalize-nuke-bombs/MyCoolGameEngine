#include "dictionary.h"
#include <stdlib.h>

#include "../logging/logger.h"

#define DICTIONARY_MAX_DIM 30

struct dictionary_entry {
    void *key;
    void *value;
    struct dictionary_entry *next;
    int hash;
};

struct dictionary {
    struct dictionary_entry **buckets;
    int dim;
    int count;
    int (*hash)(const void *key);
    bool (*equals)(const void *key1, const void *key2);
};

static int dictionary_capacity(const struct dictionary *dictionary) {
    return 1 << dictionary->dim;
}

struct dictionary* dictionary_create(int dim, int (*hash)(const void*), bool (*equals)(const void*, const void*)) {
    if (dim < 0 || dim > DICTIONARY_MAX_DIM) {
        logger_warn("Dictionary bad dim passed (%d)", dim);
        dim = 0;
    }
    struct dictionary *dictionary = malloc(sizeof(struct dictionary));
    dictionary->dim = dim;
    dictionary->buckets = calloc(dictionary_capacity(dictionary), sizeof(struct dictionary_entry*));
    dictionary->count = 0;
    dictionary->hash = hash;
    dictionary->equals = equals;
    return dictionary;
}

void dictionary_destroy(struct dictionary *dictionary) {
    dictionary_clear(dictionary);
    free(dictionary->buckets);
    free(dictionary);
}

int dictionary_count(const struct dictionary *dictionary) {
    return dictionary->count;
}

static int dictionary_bucket_index(const struct dictionary *dictionary, const int hash) {
    return hash & (dictionary_capacity(dictionary) - 1);
}

static struct dictionary_entry* dictionary_find(const struct dictionary *dictionary, const void *key, const int hash) {
    struct dictionary_entry *entry = dictionary->buckets[dictionary_bucket_index(dictionary, hash)];
    while (entry != NULL) {
        if (entry->hash == hash && dictionary->equals(entry->key, key)) {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

static void dictionary_grow(struct dictionary *dictionary) {
    const int old_capacity = dictionary_capacity(dictionary);
    struct dictionary_entry **old_buckets = dictionary->buckets;

    dictionary->dim++;
    dictionary->buckets = calloc(dictionary_capacity(dictionary), sizeof(struct dictionary_entry*));

    for (int i = 0; i < old_capacity; i++) {
        struct dictionary_entry *entry = old_buckets[i];
        while (entry != NULL) {
            struct dictionary_entry *next = entry->next;
            const int index = dictionary_bucket_index(dictionary, entry->hash);
            entry->next = dictionary->buckets[index];
            dictionary->buckets[index] = entry;
            entry = next;
        }
    }
    free(old_buckets);
}

struct dictionary_iterator dictionary_begin(const struct dictionary *dictionary) {
    const struct dictionary_iterator iterator = {
        .bucket = 0,
        .node = NULL
    };
    return iterator;
}
bool dictionary_next(const struct dictionary *dictionary, struct dictionary_iterator *iterator, struct dictionary_node *node) {
    const struct dictionary_entry *entry = iterator->node;
    while (entry == NULL) {
        if (iterator->bucket >= dictionary_capacity(dictionary)) {
            return false;
        }
        entry = dictionary->buckets[iterator->bucket++];
    }
    node->key = entry->key;
    node->value = entry->value;
    iterator->node = entry->next;
    return true;
}

bool dictionary_try_add(struct dictionary *dictionary, void *key, void *value) {
    const int hash = dictionary->hash(key);
    if (dictionary_find(dictionary, key, hash) != NULL) {
        return false;
    }

    if (dictionary->count + 1 > dictionary_capacity(dictionary) / 4 * 3 && dictionary->dim < DICTIONARY_MAX_DIM) {
        dictionary_grow(dictionary);
    }

    struct dictionary_entry *entry = malloc(sizeof(struct dictionary_entry));
    entry->key = key;
    entry->value = value;
    entry->hash = hash;
    const int index = dictionary_bucket_index(dictionary, hash);
    entry->next = dictionary->buckets[index];
    dictionary->buckets[index] = entry;
    dictionary->count++;
    return true;
}
void *dictionary_get(const struct dictionary *dictionary, void *key) {
    const struct dictionary_entry *entry = dictionary_find(dictionary, key, dictionary->hash(key));
    if (entry == NULL) {
        return NULL;
    }
    return entry->value;
}
bool dictionary_present(const struct dictionary *dictionary, void *key) {
    return dictionary_get(dictionary, key) != NULL;
}
bool dictionary_absent(const struct dictionary *dictionary, void *key) {
    return dictionary_get(dictionary, key) == NULL;
}

bool dictionary_remove(struct dictionary *dictionary, void *key) {
    const int hash = dictionary->hash(key);
    struct dictionary_entry **link = &dictionary->buckets[dictionary_bucket_index(dictionary, hash)];
    while (*link != NULL) {
        struct dictionary_entry *entry = *link;
        if (entry->hash == hash && dictionary->equals(entry->key, key)) {
            *link = entry->next;
            free(entry);
            dictionary->count--;
            return true;
        }
        link = &entry->next;
    }
    return false;
}

void dictionary_clear(struct dictionary *this) {
    for (int i = 0; i < dictionary_capacity(this); i++) {
        struct dictionary_entry *entry = this->buckets[i];
        while (entry != NULL) {
            struct dictionary_entry *next = entry->next;
            free(entry);
            entry = next;
        }
        this->buckets[i] = NULL;
    }
    this->count = 0;
}
