//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_TRANSFORM_H
#define MYCOOLGAMEENGINE_TRANSFORM_H

#include "component.h"
#include "../../utils/vector2.h"

struct transform;

struct transform* transform_create(struct entity *parent, const struct vector2* position, const struct vector2* scale);

struct vector2 transform_get_position(const struct transform *this);
void transform_set_position(struct transform *this, const struct vector2 *position);
struct vector2 transform_get_scale(const struct transform *this);
void transform_set_scale(struct transform *this, const struct vector2 *scale);

struct component* transform_as_component(struct transform *this);

#endif //MYCOOLGAMEENGINE_TRANSFORM_H
