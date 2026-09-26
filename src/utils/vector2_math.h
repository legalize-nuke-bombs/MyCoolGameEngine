#ifndef MYCOOLGAMEENGINE_VECTOR2MATH_H
#define MYCOOLGAMEENGINE_VECTOR2MATH_H

#include "vector2.h"

double vector_get_length(struct vector2 vector);
double vector_get_sqr_length(struct vector2 vector);

struct vector2 vector_sum(struct vector2 vector1, struct vector2 vector2);
struct vector2 vector_sub(struct vector2 vector1, struct vector2 vector2);
struct vector2 vector_multiply_vector(struct vector2 vector1, struct vector2 vector2);
struct vector2 vector_multiply_scalar(struct vector2 vector1, double scalar);

struct vector2 vector_normalize(struct vector2 this);

double vector_distance(const struct vector2 *point1, const struct vector2 *point2);
double vector_sqr_distance(const struct vector2 *point1, const struct vector2 *point2);

struct vector2 vector_relu(const struct vector2 *vector);

#endif
