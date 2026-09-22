#include "dictionary.h"
#include <stdlib.h>

void dictionary_init(struct dictionary *dictionary, int dim, int (*hash)(const void*), bool (*equals)(const void*, const void*)) {
    if (dim < 0) {
        dim = 0;
    }
    dictionary->_dim = dim;
    dictionary->_nodes = calloc(dictionary_capacity(dictionary),sizeof(struct dictionary_node));
    dictionary->_count = 0;
    dictionary->hash = hash;
    dictionary->equals = equals;
}

void dictionary_destroy(const struct dictionary *dictionary) {
    free(dictionary->_nodes);
}

int dictionary_capacity(const struct dictionary *dictionary) {
    return 1 << dictionary->_dim;
}
int dictionary_count(const struct dictionary *dictionary) {
    return dictionary->_count;
}

static int dictionary_node_index(const struct dictionary *dictionary, void *key) {
    return dictionary->hash(key) & (dictionary_capacity(dictionary) - 1);
}

bool dictionary_try_add(struct dictionary *dictionary, void *key, void *value) {
    const int capacity = dictionary_capacity(dictionary);

    if (dictionary->_count >= capacity) {
        return false;
    }

    const int start_index = dictionary_node_index(dictionary, key);
    int index = start_index;

    while (dictionary->_nodes[index].key != NULL) {
        if (dictionary->equals(dictionary->_nodes[index].key, key)) {
            return false;
        }

        if (++index >= capacity) {
            index = 0;
        }

        if (index == start_index) {
            return false;
        }
    }

    dictionary->_nodes[index].key = key;
    dictionary->_nodes[index].value = value;
    dictionary->_count++;
    return true;
}

void *dictionary_get(const struct dictionary *dictionary, void *key) {
    const int capacity = dictionary_capacity(dictionary);

    const int start_index = dictionary_node_index(dictionary, key);
    int index = start_index;

    while (dictionary->_nodes[index].key != NULL) {
        if (dictionary->equals(dictionary->_nodes[index].key, key)) {
            return dictionary->_nodes[index].value;
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
