#ifndef MYCOOLGAMEENGINE_VECTOR2_H
#define MYCOOLGAMEENGINE_VECTOR2_H
#include <stdbool.h>

struct vector2 {
    double x;
    double y;
};

bool vectors_equal(struct vector2 vector1, struct vector2 vector2);

extern const struct vector2 vector2_zero;
extern const struct vector2 vector2_one;
extern const struct vector2 vector2_10;
extern const struct vector2 vector2_100;

#endif
