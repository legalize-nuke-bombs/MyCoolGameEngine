#ifndef MYCOOLGAMEENGINE_VECTOR2MATH_H
#define MYCOOLGAMEENGINE_VECTOR2MATH_H

#include "Vector2.h"

double get_vector_length(const struct Vector2 *vector);
double get_vector_magnitude(const struct Vector2 *vector);

struct Vector2 sum_vector(const struct Vector2 *vector1, const struct Vector2 *vector2);
struct Vector2 subtract_vector(const struct Vector2 *vector1, const struct Vector2 *vector2);

double get_distance(const struct Vector2 *point1, const struct Vector2 *point2);
double get_distance_magnitude(const struct Vector2 *point1, const struct Vector2 *point2);

#endif
