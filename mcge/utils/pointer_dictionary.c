#include "pointer_dictionary.h"

#include "pointer_hasher.h"


static bool pointer_dictionary_key_equals(const void *key1, const void *key2) {
    return key1 == key2;
}

struct dictionary* pointer_dictionary_build(const int dim) {
    return dictionary_create(dim, pointer_hasher_hash, pointer_dictionary_key_equals);
}
