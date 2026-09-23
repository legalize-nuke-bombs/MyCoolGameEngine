#include "dictionary.h"
#include <stdlib.h>

struct dictionary {
    struct dictionary_node *nodes;
    int dim;
    int count;
    int (*hash)(const void *key);
    bool (*equals)(const void *key1, const void *key2);
};

struct dictionary* dictionary_create(int dim, int (*hash)(const void*), bool (*equals)(const void*, const void*)) {
    if (dim < 0) {
        dim = 0;
    }
    struct dictionary *dictionary = malloc(sizeof(struct dictionary));
    dictionary->dim = dim;
    dictionary->nodes = calloc(dictionary_capacity(dictionary),sizeof(struct dictionary_node));
    dictionary->count = 0;
    dictionary->hash = hash;
    dictionary->equals = equals;
    return dictionary;
}

void dictionary_destroy(struct dictionary *dictionary) {
    free(dictionary->nodes);
    free(dictionary);
}

int dictionary_capacity(const struct dictionary *dictionary) {
    return 1 << dictionary->dim;
}
int dictionary_count(const struct dictionary *dictionary) {
    return dictionary->count;
}

static int dictionary_node_index(const struct dictionary *dictionary, void *key) {
    return dictionary->hash(key) & (dictionary_capacity(dictionary) - 1);
}
struct dictionary_node dictionary_get_node(const struct dictionary *dictionary, int index) {
    return dictionary->nodes[index];
}

bool dictionary_try_add(struct dictionary *dictionary, void *key, void *value) {
    const int capacity = dictionary_capacity(dictionary);

    if (dictionary->count >= capacity) {
        return false;
    }

    const int start_index = dictionary_node_index(dictionary, key);
    int index = start_index;

    while (dictionary->nodes[index].key != NULL) {
        if (dictionary->equals(dictionary->nodes[index].key, key)) {
            return false;
        }

        if (++index >= capacity) {
            index = 0;
        }

        if (index == start_index) {
            return false;
        }
    }

    dictionary->nodes[index].key = key;
    dictionary->nodes[index].value = value;
    dictionary->count++;
    return true;
}
void *dictionary_get(const struct dictionary *dictionary, void *key) {
    const int capacity = dictionary_capacity(dictionary);

    const int start_index = dictionary_node_index(dictionary, key);
    int index = start_index;

    while (dictionary->nodes[index].key != NULL) {
        if (dictionary->equals(dictionary->nodes[index].key, key)) {
            return dictionary->nodes[index].value;
        }

        if (++index >= capacity) {
            index = 0;
        }

        if (index == start_index) {
            break;
        }
    }

    return NULL;
}
bool dictionary_present(const struct dictionary *dictionary, void *key) {
    return dictionary_get(dictionary, key) != NULL;
}
bool dictionary_absent(const struct dictionary *dictionary, void *key) {
    return dictionary_get(dictionary, key) == NULL;
}
