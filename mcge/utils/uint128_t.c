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

bool uint128_from_string(const char *str, uint128_t *output) {
    // TODO Maybe honest long arithmetic here will be more crossplatform and clean
    __uint128_t result = 0;

    while (*str) {
        if (*str >= '0' && *str <= '9') {
            result = result * 10 + (*str - '0');
        }
        else {
            return false;
        }
        str++;
    }

    output->l = (uint64_t)result;
    output->h = (uint64_t)(result >> 64);
    return true;
}
void uint128_to_string(const uint128_t this, char *output) {
    __uint128_t num = ((__uint128_t)this.h << 64) | this.l;
    if (num == 0) {
        output[0] = '0';
        output[1] = '\0';
        return;
    }

    char temp[40];
    int i = 0;
    while (num > 0) {
        temp[i++] = (char)(num % 10 + '0');
        num /= 10;
    }

    int j = 0;
    while (i > 0) {
        output[j++] = temp[--i];
    }
    output[j] = '\0';
}
