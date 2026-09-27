//
// Created by Nikita on 26.09.2026.
//

#include "rect.h"

#include <tgmath.h>


const struct rect rect_0 = {
    .position.x = 0,
    .position.y = 0,
    .size.x = 0,
    .size.y = 0
};

bool rects_intersection(const struct rect rect1, const struct rect rect2) {
    const long double delta_x = fabsl(rect1.position.x - rect2.position.x);
    const long double delta_y = fabsl(rect1.position.y - rect2.position.y);

    const double sum_half_width  = (rect1.size.x + rect2.size.x) / 2.0;
    const double sum_half_height = (rect1.size.y + rect2.size.y) / 2.0;

    return (delta_x < sum_half_width) && (delta_y < sum_half_height);
}