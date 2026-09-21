#include "vector2_math.h"

#include <math.h>

double get_vector_length(const struct vector2 *vector) {
    return sqrt(get_vector_magnitude(vector));
}
double get_vector_magnitude(const struct vector2 *vector) {
    return vector->x * vector->x + vector->y * vector->y;
}

struct vector2 sum_vector(const struct vector2 *vector1, const struct vector2 *vector2) {
    struct vector2 result;
    result.x = vector1->x + vector2->x;
    result.y = vector2->y + vector2->y;
    return result;
}
struct vector2 subtract_vector(const struct vector2 *vector1, const struct vector2 *vector2) {
    struct vector2 result;
    result.x = vector1->x - vector2->x;
    result.y = vector2->y - vector2->y;
    return result;
}

double get_distance(const struct vector2 *point1, const struct vector2 *point2) {
    return sqrt(get_distance_magnitude(point1, point2));
}
double get_distance_magnitude(const struct vector2 *point1, const struct vector2 *point2) {
    const struct vector2 subtracted = subtract_vector(point1, point2);
    return subtracted.x * subtracted.x + subtracted.y * subtracted.y;
}