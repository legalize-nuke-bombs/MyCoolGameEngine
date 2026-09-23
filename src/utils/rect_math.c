//
// Created by nikita on 23.09.2026.
//

#include "rect_math.h"
#include "vector2_math.h"


struct rect rect_sum(const struct rect *rect1, const struct rect *rect2) {
    const struct rect result = {
        .position = vector_sum(&rect1->position, &rect2->position),
        .size = vector_sum(&rect1->size, &rect2->size)
    };
    return result;
}
struct rect rect_sub(const struct rect *rect1, const struct rect *rect2) {
    const struct rect result = {
        .position = vector_sub(&rect1->position, &rect2->position),
        .size = vector_sub(&rect1->size, &rect2->size)
    };
    return result;
}

struct rect rect_sdl(const struct rect *this) {
    const struct vector2 half_size = {
        .x = this->size.x / 2.0f,
        .y = this->size.y / 2.0f
    };

    const struct rect result = {
        .position = vector_sub(&this->position, &half_size),
        .size = this->size
    };

    return result;
}