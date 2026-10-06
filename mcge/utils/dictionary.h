//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_DICTIONARY_H
#define MYCOOLGAMEENGINE_DICTIONARY_H
#include <stdbool.h>
#include "../api.h"

struct dictionary_node {
    void *key;
    void *value;
};

struct dictionary_iterator {
    int position;
};

struct dictionary;

MCGE_API struct dictionary* dictionary_create(int dim, int (*hash)(const void*), bool (*equals)(const void*, const void*));
MCGE_API void dictionary_destroy(struct dictionary *dictionary);

MCGE_API int dictionary_count(const struct dictionary *dictionary);

// Adding and removing while iterating is allowed: removed entries are skipped, entries added during a pass may or may not be visited in it.
MCGE_API struct dictionary_iterator dictionary_begin(const struct dictionary *dictionary);
MCGE_API bool dictionary_next(const struct dictionary *dictionary, struct dictionary_iterator *iterator, struct dictionary_node *node);

MCGE_API bool dictionary_try_add(struct dictionary *dictionary, void *key, void *value);
MCGE_API void *dictionary_get(const struct dictionary *dictionary, void *key);
MCGE_API bool dictionary_present(const struct dictionary *dictionary, void *key);
MCGE_API bool dictionary_absent(const struct dictionary *dictionary, void *key);

MCGE_API bool dictionary_remove(struct dictionary *dictionary, void *key);

MCGE_API void dictionary_clear(struct dictionary *this);

#endif //MYCOOLGAMEENGINE_DICTIONARY_H
