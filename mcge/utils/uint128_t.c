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