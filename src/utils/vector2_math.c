#include "vector2_math.h"

#include <math.h>

double vector_mod(const struct vector2 vector) {
    return sqrt(vector_sql_mod(vector));
}
double vector_sql_mod(struct vector2 vector) {
    return vector.x * vector.x + vector.y * vector.y;
}

struct vector2 vector_sum(const struct vector2 vector1, const struct vector2 vector2) {
    struct vector2 result;
    result.x = vector1.x + vector2.x;
    result.y = vector1.y + vector2.y;
    return result;
}
struct vector2 vector_sub(const struct vector2 vector1, const struct vector2 vector2) {
    struct vector2 result;
    result.x = vector1.x - vector2.x;
    result.y = vector1.y - vector2.y;
    return result;
}
struct vector2 vector_multiply_vector(const struct vector2 vector1, const struct vector2 vector2) {
    struct vector2 result;
    result.x = vector1.x * vector2.x;
    result.y = vector1.y * vector2.y;
    return result;
}
struct vector2 vector_multiply_scalar(const struct vector2 vector1, double scalar) {
    const struct vector2 result = {
        .x = vector1.x * scalar,
        .y = vector1.y * scalar
    };
    return result;
}
double vector_dot(const struct vector2 vector1, const struct vector2 vector2) {
    return vector1.x * vector2.x + vector1.y * vector2.y;
}

struct vector2 vector_normalize(const struct vector2 this) {
    const double length = vector_mod(this);
    if (length < 0.00001) {
        return vector2_zero;
    }
    return vector_multiply_scalar(this, 1 / length);
}

double vector_distance(const struct vector2 *point1, const struct vector2 *point2) {
    return sqrt(vector_sqr_distance(point1, point2));
}
double vector_sqr_distance(const struct vector2 *point1, const struct vector2 *point2) {
    const struct vector2 subtracted = vector_sub(*point1, *point2);
    return subtracted.x * subtracted.x + subtracted.y * subtracted.y;
}

struct vector2 vector_relu(const struct vector2 *vector) {
    const struct vector2 output = {
        .x = vector->x < 0 ? 0 : vector->x,
        .y = vector->y < 0 ? 0 : vector->y,
    };
    return output;
}