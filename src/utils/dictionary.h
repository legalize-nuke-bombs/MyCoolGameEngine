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

struct dictionary;

struct dictionary* dictionary_create(int dim, int (*hash)(const void*), bool (*equals)(const void*, const void*));
void dictionary_destroy(struct dictionary *dictionary);

int dictionary_capacity(const struct dictionary *dictionary);
int dictionary_count(const struct dictionary *dictionary);

struct dictionary_node dictionary_get_node(const struct dictionary *dictionary, int index);

bool dictionary_try_add(struct dictionary *dictionary, void *key, void *value);
void *dictionary_get(const struct dictionary *dictionary, void *key);
bool dictionary_present(const struct dictionary *dictionary, void *key);
bool dictionary_absent(const struct dictionary *dictionary, void *key);

void dictionary_clear(struct dictionary *this);

#endif //MYCOOLGAMEENGINE_DICTIONARY_H
