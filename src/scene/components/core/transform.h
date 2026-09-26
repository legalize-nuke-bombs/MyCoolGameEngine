//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_TRANSFORM_H
#define MYCOOLGAMEENGINE_TRANSFORM_H

#include "../component.h"
#include "../../../utils/vector2.h"

struct transform;
struct parser;

const char* transform_component_key(void);

struct component* transform_create(struct parser *parser, struct entity *parent);

struct vector2 transform_get_position(const struct transform *this);
void transform_set_position(struct transform *this, struct vector2 position);
struct vector2 transform_get_scale(const struct transform *this);
void transform_set_scale(struct transform *this, const struct vector2 *scale);

#endif //MYCOOLGAMEENGINE_TRANSFORM_H
