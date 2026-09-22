//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_DICTIONARY_H
#define MYCOOLGAMEENGINE_DICTIONARY_H
#include <stdbool.h>

struct dictionary_node {
    void *key;
    void *value;
};

struct dictionary {
    struct dictionary_node *_nodes;
    int _dim;
    int _count;
    int (*hash)(void *key);
    bool (*equals)(void *key1, void *key2);
};

void dictionary_init(struct dictionary *dictionary, int dim, int (*hash)(void*), bool (*equals)(void*, void*));
void dictionary_destroy(const struct dictionary *dictionary);

int dictionary_capacity(const struct dictionary *dictionary);
int dictionary_count(const struct dictionary *dictionary);

bool dictionary_try_add(struct dictionary *dictionary, void *key, void *value);
void *dictionary_get(const struct dictionary *dictionary, void *key);

#endif //MYCOOLGAMEENGINE_DICTIONARY_H
