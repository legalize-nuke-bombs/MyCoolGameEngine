//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RECT_MATH_H
#define MYCOOLGAMEENGINE_RECT_MATH_H

#include "rect.h"

struct rect rect_sum(const struct rect *rect1, const struct rect *rect2);
struct rect rect_sub(const struct rect *rect1, const struct rect *rect2);

struct rect rect_sdl(const struct rect *obj, const struct rect *viewport);

#endif //MYCOOLGAMEENGINE_RECT_MATH_H
