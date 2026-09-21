#include "Vector2Math.h"

#include <math.h>

double get_vector_length(const struct Vector2 *vector) {
    return sqrt(get_vector_magnitude(vector));
}
double get_vector_magnitude(const struct Vector2 *vector) {
    return vector->x * vector->x + vector->y * vector->y;
}

struct Vector2 sum_vector(const struct Vector2 *vector1, const struct Vector2 *vector2) {
    struct Vector2 result;
    result.x = vector1->x + vector2->x;
    result.y = vector2->y + vector2->y;
    return result;
}
struct Vector2 subtract_vector(const struct Vector2 *vector1, const struct Vector2 *vector2) {
    struct Vector2 result;
    result.x = vector1->x - vector2->x;
    result.y = vector2->y - vector2->y;
    return result;
}

double get_distance(const struct Vector2 *point1, const struct Vector2 *point2) {
    return sqrt(get_distance_magnitude(point1, point2));
}
double get_distance_magnitude(const struct Vector2 *point1, const struct Vector2 *point2) {
    const struct Vector2 subtracted = subtract_vector(point1, point2);
    return subtracted.x * subtracted.x + subtracted.y * subtracted.y;
}