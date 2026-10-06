#include "pointer_dictionary.h"

#include <stdint.h>

static int pointer_dictionary_key_hash(const void *key) {
    uint64_t x = (uint64_t)(uintptr_t)key;
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    return (int)x;
}

static bool pointer_dictionary_key_equals(const void *key1, const void *key2) {
    return key1 == key2;
}

struct dictionary* pointer_dictionary_build(const int dim) {
    return dictionary_create(dim, pointer_dictionary_key_hash, pointer_dictionary_key_equals);
}
