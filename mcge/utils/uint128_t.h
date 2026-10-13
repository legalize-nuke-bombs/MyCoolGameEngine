//
// Created by nikita on 07.10.2026.
//

#ifndef MYCOOLGAMEENGINE_UINT128_T_H
#define MYCOOLGAMEENGINE_UINT128_T_H

#include <stdbool.h>
#include <stdint.h>
#include "../api.h"

struct uint128_t {
    uint64_t h;
    uint64_t l;
};

typedef struct uint128_t uint128_t;

MCGE_API extern const uint128_t uint128_zero;

MCGE_API int uint128_cmp(uint128_t a, uint128_t b);

MCGE_API int uint128_hash(const void *base);

MCGE_API bool uint128_from_string(const char *str, uint128_t *output);
MCGE_API void uint128_to_string(uint128_t this, char* output);

#endif //MYCOOLGAMEENGINE_UINT128_T_H
