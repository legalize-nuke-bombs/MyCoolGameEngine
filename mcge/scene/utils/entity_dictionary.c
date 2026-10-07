//
// Created by nikita on 07.10.2026.
//

#include "entity_dictionary.h"

#include <stdbool.h>
#include <stdint.h>

#include "mcge/scene/entity.h"


static int entity_dictionary_key_hash(const void *key) {
    const uint128_t *id = key;

    const uint32_t low32  = (uint32_t)(id->l ^ (id->l >> 32));
    const uint32_t high32 = (uint32_t)(id->h ^ (id->h >> 32));

    const uint32_t hash = low32 ^ (high32 + 0x9e3779b9 + (low32 << 6) + (low32 >> 2));

    return (int)hash;
}

static bool entity_dictionary_key_equals(const void *p1, const void *p2) {
    const uint128_t *id1 = p1;
    const uint128_t *id2 = p2;
    return uint128_cmp(*id1, *id2) == 0;
}


struct dictionary* entity_dictionary_build(const int dim) {
    return dictionary_create(dim, entity_dictionary_key_hash, entity_dictionary_key_equals);
}