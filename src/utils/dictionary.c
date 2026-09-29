#include "dictionary.h"
#include <stdlib.h>
#include <string.h>

#include "../logging/logger.h"

#define DICTIONARY_MAX_DIM 30

struct dictionary_slot {
    void *key;
    void *value;
    int hash;
};

struct dictionary {
    struct dictionary_slot *slots;
    int dim;
    int count;
    int (*hash)(const void *key);
    bool (*equals)(const void *key1, const void *key2);
};

static int dictionary_capacity(const struct dictionary *dictionary) {
    return 1 << dictionary->dim;
}
static int dictionary_mask(const struct dictionary *dictionary) {
    return dictionary_capacity(dictionary) - 1;
}

struct dictionary* dictionary_create(int dim, int (*hash)(const void*), bool (*equals)(const void*, const void*)) {
    if (dim < 0 || dim > DICTIONARY_MAX_DIM) {
        logger_warn("Dictionary bad dim passed (%d)", dim);
        dim = 0;
    }
    struct dictionary *dictionary = malloc(sizeof(struct dictionary));
    dictionary->dim = dim;
    dictionary->slots = calloc(dictionary_capacity(dictionary), sizeof(struct dictionary_slot));
    dictionary->count = 0;
    dictionary->hash = hash;
    dictionary->equals = equals;
    return dictionary;
}

void dictionary_destroy(struct dictionary *dictionary) {
    free(dictionary->slots);
    free(dictionary);
}

int dictionary_count(const struct dictionary *dictionary) {
    return dictionary->count;
}

static int dictionary_find_index(const struct dictionary *dictionary, const void *key, const int hash) {
    const int mask = dictionary_mask(dictionary);
    for (int i = hash & mask; dictionary->slots[i].key != NULL; i = (i + 1) & mask) {
        if (dictionary->slots[i].hash == hash && dictionary->equals(dictionary->slots[i].key, key)) {
            return i;
        }
    }
    return -1;
}

static void dictionary_place(struct dictionary *dictionary, const struct dictionary_slot slot) {
    const int mask = dictionary_mask(dictionary);
    int i = slot.hash & mask;
    while (dictionary->slots[i].key != NULL) {
        i = (i + 1) & mask;
    }
    dictionary->slots[i] = slot;
}

static void dictionary_grow(struct dictionary *dictionary) {
    const int old_capacity = dictionary_capacity(dictionary);
    struct dictionary_slot *old_slots = dictionary->slots;

    dictionary->dim++;
    dictionary->slots = calloc(dictionary_capacity(dictionary), sizeof(struct dictionary_slot));

    for (int i = 0; i < old_capacity; i++) {
        if (old_slots[i].key != NULL) {
            dictionary_place(dictionary, old_slots[i]);
        }
    }
    free(old_slots);
}

struct dictionary_iterator dictionary_begin(const struct dictionary *dictionary) {
    const struct dictionary_iterator iterator = {
        .bucket = 0,
        .node = NULL
    };
    return iterator;
}
bool dictionary_next(const struct dictionary *dictionary, struct dictionary_iterator *iterator, struct dictionary_node *node) {
    while (iterator->bucket < dictionary_capacity(dictionary)) {
        const struct dictionary_slot *slot = &dictionary->slots[iterator->bucket++];
        if (slot->key != NULL) {
            node->key = slot->key;
            node->value = slot->value;
            return true;
        }
    }
    return false;
}

bool dictionary_try_add(struct dictionary *dictionary, void *key, void *value) {
    const int hash = dictionary->hash(key);
    if (dictionary_find_index(dictionary, key, hash) >= 0) {
        return false;
    }

    if (2 * (dictionary->count + 1) > dictionary_capacity(dictionary) && dictionary->dim < DICTIONARY_MAX_DIM) {
        dictionary_grow(dictionary);
    }
    if (dictionary->count >= dictionary_capacity(dictionary)) {
        logger_error("Dictionary is full (count %d)", dictionary->count);
        return false;
    }

    const struct dictionary_slot slot = {
        .key = key,
        .value = value,
        .hash = hash
    };
    dictionary_place(dictionary, slot);
    dictionary->count++;
    return true;
}
void *dictionary_get(const struct dictionary *dictionary, void *key) {
    const int index = dictionary_find_index(dictionary, key, dictionary->hash(key));
    if (index < 0) {
        return NULL;
    }
    return dictionary->slots[index].value;
}
bool dictionary_present(const struct dictionary *dictionary, void *key) {
    return dictionary_get(dictionary, key) != NULL;
}
bool dictionary_absent(const struct dictionary *dictionary, void *key) {
    return dictionary_get(dictionary, key) == NULL;
}

bool dictionary_remove(struct dictionary *dictionary, void *key) {
    int hole = dictionary_find_index(dictionary, key, dictionary->hash(key));
    if (hole < 0) {
        return false;
    }

    const int mask = dictionary_mask(dictionary);
    dictionary->slots[hole].key = NULL;
    dictionary->slots[hole].value = NULL;
    for (int i = (hole + 1) & mask; dictionary->slots[i].key != NULL; i = (i + 1) & mask) {
        const int home = dictionary->slots[i].hash & mask;
        if (((i - home) & mask) >= ((i - hole) & mask)) {
            dictionary->slots[hole] = dictionary->slots[i];
            dictionary->slots[i].key = NULL;
            dictionary->slots[i].value = NULL;
            hole = i;
        }
    }
    dictionary->count--;
    return true;
}

void dictionary_clear(struct dictionary *this) {
    memset(this->slots, 0, dictionary_capacity(this) * sizeof(struct dictionary_slot));
    this->count = 0;
}
