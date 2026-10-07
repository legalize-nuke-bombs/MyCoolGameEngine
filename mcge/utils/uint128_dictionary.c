#include "uint128_dictionary.h"

#include "uint128_t.h"


static bool uint128_dictionary_key_equals(const void *key1, const void *key2) {
    const uint128_t *number1 = key1;
    const uint128_t *number2 = key2;
    return uint128_cmp(*number1, *number2) == 0;
}

struct dictionary* uint128_dictionary_build(const int dim) {
    return dictionary_create(dim, uint128_hash, uint128_dictionary_key_equals);
}
