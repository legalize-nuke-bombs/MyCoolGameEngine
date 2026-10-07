//
// Created by nikita on 07.10.2026.
//

#include "entity_dictionary.h"

#include <stdbool.h>

#include "mcge/scene/entity.h"


static bool entity_dictionary_key_equals(const void *p1, const void *p2) {
    const uint128_t *id1 = p1;
    const uint128_t *id2 = p2;
    return uint128_cmp(*id1, *id2) == 0;
}


struct dictionary* entity_dictionary_build(const int dim) {
    return dictionary_create(dim, uint128_hash, entity_dictionary_key_equals);
}