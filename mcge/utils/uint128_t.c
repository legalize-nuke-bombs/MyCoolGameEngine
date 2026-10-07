//
// Created by nikita on 07.10.2026.
//

#include "uint128_t.h"

const uint128_t uint128_zero = {
    .h = 0,
    .l = 0
};

int uint128_cmp(const uint128_t a, const uint128_t b) {
    if (a.h > b.h) {
        return 1;
    }
    if (a.h < b.h) {
        return -1;
    }
    if (a.l > b.l) {
        return 1;
    }
    if (a.l < b.l) {
        return -1;
    }
    return 0;
}

int uint128_hash(const void *base) {
    const uint128_t *this = base;

    const uint32_t low32  = (uint32_t)(this->l ^ (this->l >> 32));
    const uint32_t high32 = (uint32_t)(this->h ^ (this->h >> 32));

    const uint32_t hash = low32 ^ (high32 + 0x9e3779b9 + (low32 << 6) + (low32 >> 2));

    return (int)hash;
}