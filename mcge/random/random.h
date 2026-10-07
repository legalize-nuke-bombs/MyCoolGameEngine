//
// Created by nikita on 28.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RANDOM_H
#define MYCOOLGAMEENGINE_RANDOM_H

#include "../api.h"
#include "mcge/utils/uint128_t.h"

struct msystem;

MCGE_API extern const struct msystem random_msystem;

MCGE_API int random_next_int(int l, int r);
MCGE_API uint128_t random_next_uint128();
MCGE_API double random_next_double(double l, double r);

#endif //MYCOOLGAMEENGINE_RANDOM_H
