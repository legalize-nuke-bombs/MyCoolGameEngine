#include "vector2_math.h"

#include <math.h>

double vector_get_length(const struct vector2 *vector) {
    return sqrt(vector_get_magnitude(vector));
}
double vector_get_magnitude(const struct vector2 *vector) {
    return vector->x * vector->x + vector->y * vector->y;
}

struct vector2 vector_sum(const struct vector2 *vector1, const struct vector2 *vector2) {
    struct vector2 result;
    result.x = vector1->x + vector2->x;
    result.y = vector1->y + vector2->y;
    return result;
}
struct vector2 vector_subtract(const struct vector2 *vector1, const struct vector2 *vector2) {
    struct vector2 result;
    result.x = vector1->x - vector2->x;
    result.y = vector1->y - vector2->y;
    return result;
}

double vector_distance(const struct vector2 *point1, const struct vector2 *point2) {
    return sqrt(vector_distance_magnitude(point1, point2));
}
double vector_distance_magnitude(const struct vector2 *point1, const struct vector2 *point2) {
    const struct vector2 subtracted = vector_subtract(point1, point2);
    return subtracted.x * subtracted.x + subtracted.y * subtracted.y;
}

struct vector2 vector_relu(const struct vector2 *vector) {
    const struct vector2 output = {
        .x = vector->x < 0 ? 0 : vector->x,
        .y = vector->y < 0 ? 0 : vector->y,
    };
    return output;
}