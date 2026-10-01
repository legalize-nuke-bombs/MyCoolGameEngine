#include "vector2.h"

bool vectors_equal(const struct vector2 vector1, const struct vector2 vector2) {
    return vector1.x == vector2.x && vector1.y == vector2.y;
}

const struct vector2 vector2_zero = { .x = 0.0, .y = 0.0 };
const struct vector2 vector2_one = { .x = 1.0, .y = 1.0 };
const struct vector2 vector2_10 = {.x = 10.0, .y = 10.0};
const struct vector2 vector2_100 = {.x = 100.0, .y = 100.0};
const struct vector2 vector2_1000 = {.x = 1000.0, .y = 1000.0};
