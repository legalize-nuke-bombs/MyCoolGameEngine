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

struct rect transform_get_rect(const struct transform *this);
void transform_set_rect(struct transform *this, struct rect rect);

#endif //MYCOOLGAMEENGINE_TRANSFORM_H
