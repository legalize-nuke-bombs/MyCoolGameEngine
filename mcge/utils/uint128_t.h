//
// Created by nikita on 07.10.2026.
//

#ifndef MYCOOLGAMEENGINE_UINT128_T_H
#define MYCOOLGAMEENGINE_UINT128_T_H
#include <stdint.h>

struct uint128_t {
    uint64_t h;
    uint64_t l;
};

typedef struct uint128_t uint128_t;

int uint128_cmp(uint128_t a, uint128_t b);

#endif //MYCOOLGAMEENGINE_UINT128_T_H
