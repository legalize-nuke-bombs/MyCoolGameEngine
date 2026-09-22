//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_TRANSFORM_H
#define MYCOOLGAMEENGINE_TRANSFORM_H

#include "../component.h"
#include "../../../utils/vector2.h"

struct transform {
    struct component base;
    struct vector2 position;
    struct vector2 scale;
};

const char* transform_component_key();

void transform_init(struct transform *this, const struct vector2* position, const struct vector2* scale);

struct component* transform_as_component(struct transform *this);

#endif //MYCOOLGAMEENGINE_TRANSFORM_H
