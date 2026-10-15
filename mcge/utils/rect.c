//
// Created by Nikita on 26.09.2026.
//

#include "rect.h"

#include <math.h>

#include "vector2_math.h"


const struct rect rect_0 = {
    .position.x = 0,
    .position.y = 0,
    .size.x = 0,
    .size.y = 0
};

bool rects_equal(const struct rect rect1, const struct rect rect2) {
    return vectors_equal(rect1.position, rect2.position) && vectors_equal(rect1.size, rect2.size);
}

bool rect_contains(const struct rect rect, const struct vector2 point) {
    const struct vector2 half_size = vector_multiply_scalar(rect.size, 0.5f);

    const struct vector2 min_bound = vector_sub(rect.position, half_size);
    const struct vector2 max_bound = vector_sum(rect.position, half_size);

    return (point.x >= min_bound.x && point.x <= max_bound.x) &&
           (point.y >= min_bound.y && point.y <= max_bound.y);
}
bool rects_intersection(const struct rect rect1, const struct rect rect2) {
    const double delta_x = fabs(rect1.position.x - rect2.position.x);
    const double delta_y = fabs(rect1.position.y - rect2.position.y);

    const double sum_half_width  = (rect1.size.x + rect2.size.x) / 2.0;
    const double sum_half_height = (rect1.size.y + rect2.size.y) / 2.0;

    return (delta_x < sum_half_width) && (delta_y < sum_half_height);
}
