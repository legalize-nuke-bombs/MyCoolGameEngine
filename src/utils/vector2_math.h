#ifndef MYCOOLGAMEENGINE_VECTOR2MATH_H
#define MYCOOLGAMEENGINE_VECTOR2MATH_H

#include "vector2.h"

double get_vector_length(const struct vector2 *vector);
double get_vector_magnitude(const struct vector2 *vector);

struct vector2 sum_vector(const struct vector2 *vector1, const struct vector2 *vector2);
struct vector2 subtract_vector(const struct vector2 *vector1, const struct vector2 *vector2);

double get_distance(const struct vector2 *point1, const struct vector2 *point2);
double get_distance_magnitude(const struct vector2 *point1, const struct vector2 *point2);

#endif
