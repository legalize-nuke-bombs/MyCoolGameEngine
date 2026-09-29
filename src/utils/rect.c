//
// Created by Nikita on 26.09.2026.
//

#include "rect.h"

#include <math.h>


const struct rect rect_0 = {
    .position.x = 0,
    .position.y = 0,
    .size.x = 0,
    .size.y = 0
};

bool rects_equal(const struct rect rect1, const struct rect rect2) {
    return vectors_equal(rect1.position, rect2.position) && vectors_equal(rect1.size, rect2.size);
}

bool rects_intersection(const struct rect rect1, const struct rect rect2) {
    const double delta_x = fabs(rect1.position.x - rect2.position.x);
    const double delta_y = fabs(rect1.position.y - rect2.position.y);

    const double sum_half_width  = (rect1.size.x + rect2.size.x) / 2.0;
    const double sum_half_height = (rect1.size.y + rect2.size.y) / 2.0;

    return (delta_x < sum_half_width) && (delta_y < sum_half_height);
}