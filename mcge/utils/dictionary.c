#include "dictionary.h"
#include <stdlib.h>

#include "../logging/logger.h"

#define DICTIONARY_MAX_DIM 30
#define DICTIONARY_NONE (-1)

struct dictionary_entry {
    void *key;
    void *value;
    int hash;
    int next_free;
};

struct dictionary {
    struct dictionary_entry *entries;
    int *index;
    int entries_capacity;
    int used;
    int count;
    int free_head;

    int dim;

    int (*hash)(const void *key);
    bool (*equals)(const void *key1, const void *key2);
};

static int dictionary_index_capacity(const struct dictionary *dictionary) {
    return 1 << dictionary->dim;
}
static int dictionary_mask(const struct dictionary *dictionary) {
    return dictionary_index_capacity(dictionary) - 1;
}
static void dictionary_index_reset(const struct dictionary *dictionary) {
    for (int i = 0; i < dictionary_index_capacity(dictionary); i++) {
        dictionary->index[i] = DICTIONARY_NONE;
    }
}

struct dictionary* dictionary_create(int dim, int (*hash)(const void*), bool (*equals)(const void*, const void*)) {
    if (dim < 0 || dim > DICTIONARY_MAX_DIM) {
        logger_warn("Dictionary bad dim passed (%d)", dim);
        dim = 0;
    }
    struct dictionary *dictionary = malloc(sizeof(struct dictionary));
    dictionary->entries = NULL;
    dictionary->index = NULL;
    dictionary->entries_capacity = 0;
    dictionary->used = 0;
    dictionary->count = 0;
    dictionary->free_head = DICTIONARY_NONE;
    dictionary->dim = dim > 0 ? dim : 1;
    dictionary->hash = hash;
    dictionary->equals = equals;
    return dictionary;
}

void dictionary_destroy(struct dictionary *dictionary) {
    free(dictionary->entries);
    free(dictionary);
}

int dictionary_count(const struct dictionary *dictionary) {
    return dictionary->count;
}

static int dictionary_find_slot(const struct dictionary *dictionary, const void *key, const int hash) {
    if (dictionary->index == NULL) {
        return DICTIONARY_NONE;
    }
    const int mask = dictionary_mask(dictionary);
    for (int i = hash & mask; dictionary->index[i] != DICTIONARY_NONE; i = (i + 1) & mask) {
        const struct dictionary_entry *entry = &dictionary->entries[dictionary->index[i]];
        if (entry->hash == hash && dictionary->equals(entry->key, key)) {
            return i;
        }
    }
    return DICTIONARY_NONE;
}

static void dictionary_index_place(const struct dictionary *dictionary, const int position) {
    const int mask = dictionary_mask(dictionary);
    int i = dictionary->entries[position].hash & mask;
    while (dictionary->index[i] != DICTIONARY_NONE) {
        i = (i + 1) & mask;
    }
    dictionary->index[i] = position;
}

static void dictionary_allocate_block(struct dictionary *dictionary, const int dim) {
    const int entries_capacity = (1 << dim) / 2;
    struct dictionary_entry *entries = malloc(sizeof(struct dictionary_entry) * entries_capacity + sizeof(int) * (1 << dim));
    for (int position = 0; position < dictionary->used; position++) {
        entries[position] = dictionary->entries[position];
    }
    free(dictionary->entries);
    dictionary->entries = entries;
    dictionary->index = (int *)(entries + entries_capacity);
    dictionary->entries_capacity = entries_capacity;
    dictionary->dim = dim;
    dictionary_index_reset(dictionary);
    for (int position = 0; position < dictionary->used; position++) {
        if (dictionary->entries[position].key != NULL) {
            dictionary_index_place(dictionary, position);
        }
    }
}

static int dictionary_take_position(struct dictionary *dictionary) {
    if (dictionary->free_head != DICTIONARY_NONE) {
        const int position = dictionary->free_head;
        dictionary->free_head = dictionary->entries[position].next_free;
        return position;
    }
    if (dictionary->entries == NULL) {
        dictionary_allocate_block(dictionary, dictionary->dim);
    }
    else if (dictionary->used == dictionary->entries_capacity) {
        if (dictionary->dim >= DICTIONARY_MAX_DIM) {
            return DICTIONARY_NONE;
        }
        dictionary_allocate_block(dictionary, dictionary->dim + 1);
    }
    return dictionary->used++;
}

struct dictionary_iterator dictionary_begin(const struct dictionary *dictionary) {
    const struct dictionary_iterator iterator = {
        .position = 0
    };
    return iterator;
}
bool dictionary_next(const struct dictionary *dictionary, struct dictionary_iterator *iterator, struct dictionary_node *node) {
    while (iterator->position < dictionary->used) {
        const struct dictionary_entry *entry = &dictionary->entries[iterator->position++];
        if (entry->key != NULL) {
            node->key = entry->key;
            node->value = entry->value;
            return true;
        }
    }
    return false;
}

bool dictionary_try_add(struct dictionary *dictionary, void *key, void *value) {
    const int hash = dictionary->hash(key);
    if (dictionary_find_slot(dictionary, key, hash) != DICTIONARY_NONE) {
        return false;
    }

    const int position = dictionary_take_position(dictionary);
    if (position == DICTIONARY_NONE) {
        logger_error("Dictionary is full (count %d)", dictionary->count);
        return false;
    }

    struct dictionary_entry *entry = &dictionary->entries[position];
    entry->key = key;
    entry->value = value;
    entry->hash = hash;
    entry->next_free = DICTIONARY_NONE;
    dictionary_index_place(dictionary, position);
    dictionary->count++;
    return true;
}
void *dictionary_get(const struct dictionary *dictionary, void *key) {
    const int slot = dictionary_find_slot(dictionary, key, dictionary->hash(key));
    if (slot == DICTIONARY_NONE) {
        return NULL;
    }
    return dictionary->entries[dictionary->index[slot]].value;
}
bool dictionary_present(const struct dictionary *dictionary, void *key) {
    return dictionary_get(dictionary, key) != NULL;
}
bool dictionary_absent(const struct dictionary *dictionary, void *key) {
    return dictionary_get(dictionary, key) == NULL;
}

bool dictionary_remove(struct dictionary *dictionary, void *key) {
    int hole = dictionary_find_slot(dictionary, key, dictionary->hash(key));
    if (hole == DICTIONARY_NONE) {
        return false;
    }

    const int position = dictionary->index[hole];
    struct dictionary_entry *entry = &dictionary->entries[position];
    entry->key = NULL;
    entry->value = NULL;
    entry->next_free = dictionary->free_head;
    dictionary->free_head = position;

    const int mask = dictionary_mask(dictionary);
    dictionary->index[hole] = DICTIONARY_NONE;
    for (int i = (hole + 1) & mask; dictionary->index[i] != DICTIONARY_NONE; i = (i + 1) & mask) {
        const int home = dictionary->entries[dictionary->index[i]].hash & mask;
        if (((i - home) & mask) >= ((i - hole) & mask)) {
            dictionary->index[hole] = dictionary->index[i];
            dictionary->index[i] = DICTIONARY_NONE;
            hole = i;
        }
    }

    dictionary->count--;
    if (dictionary->count == 0) {
        dictionary->used = 0;
        dictionary->free_head = DICTIONARY_NONE;
    }
    return true;
}

void dictionary_clear(struct dictionary *this) {
    if (this->index != NULL) {
        dictionary_index_reset(this);
    }
    this->used = 0;
    this->count = 0;
    this->free_head = DICTIONARY_NONE;
}
