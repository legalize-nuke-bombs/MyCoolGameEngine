#ifndef MYCOOLGAMEENGINE_VECTOR2_H
#define MYCOOLGAMEENGINE_VECTOR2_H
#include <stdbool.h>
#include "../api.h"

struct vector2 {
    double x;
    double y;
};

MCGE_API bool vectors_equal(struct vector2 vector1, struct vector2 vector2);

MCGE_API extern const struct vector2 vector2_zero;
MCGE_API extern const struct vector2 vector2_one;
MCGE_API extern const struct vector2 vector2_10;
MCGE_API extern const struct vector2 vector2_100;
MCGE_API extern const struct vector2 vector2_1000;
MCGE_API extern const struct vector2 vector2_10000;
MCGE_API extern const struct vector2 vector2_100000;

#endif
