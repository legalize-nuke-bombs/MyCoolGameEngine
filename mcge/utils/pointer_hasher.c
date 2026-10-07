//
// Created by nikita on 07.10.2026.
//

#include "pointer_hasher.h"

#include <stdint.h>

int pointer_hasher_hash(const void *ptr) {
    uint64_t x = (uintptr_t)ptr;
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    return (int)x;
}
