//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RECT_H
#define MYCOOLGAMEENGINE_RECT_H


#include <stdbool.h>

#include "vector2.h"
#include "../api.h"

struct rect {
    struct vector2 position;
    struct vector2 size;
};

MCGE_API bool rects_equal(struct rect rect1, struct rect rect2);

MCGE_API bool rects_intersection(struct rect rect1, struct rect rect2);

MCGE_API extern const struct rect rect_0;

#endif //MYCOOLGAMEENGINE_RECT_H
