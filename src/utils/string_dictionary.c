//
// Created by nikita on 22.09.2026.
//

#include "string_dictionary.h"

#include <string.h>

#include "string_hasher.h"

static int string_dictionary_key_hash(const void *key) {
    const char* string = key;
    return string_hasher_djb2(string);
}

static bool string_dictionary_key_equals(const void *key1, const void *key2) {
    const char* string1 = key1;
    const char* string2 = key2;
    return strcmp(string1, string2) == 0;
}

struct dictionary* string_dictionary_build(const int dim) {
    return dictionary_create(dim, string_dictionary_key_hash, string_dictionary_key_equals);
}
