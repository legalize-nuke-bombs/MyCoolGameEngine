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

struct rect rect_sdl(const struct rect *obj, const struct rect *viewport) {
    const struct vector2 half_viewport_size = vector_multiply_scalar(&viewport->size, 0.5);
    const struct rect result = {
        .position = {
            .x = half_viewport_size.x + (obj->position.x - viewport->position.x) - (obj->size.x / 2.0f),
            .y = half_viewport_size.y - (obj->position.y - viewport->position.y) - (obj->size.y / 2.0f)
        },
        .size = obj->size
    };
    return result;
}