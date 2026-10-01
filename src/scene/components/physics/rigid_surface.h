//
// Created by nikita on 01.10.2026.
//

#ifndef MYCOOLGAMEENGINE_RIGID_SURFACE_H
#define MYCOOLGAMEENGINE_RIGID_SURFACE_H

struct parser;
struct entity;
struct chunks;

#include "../../../utils/rect.h"

const char* rigid_surface_component_key(void);

struct component* rigid_surface_create(struct parser *parser, struct entity *parent);

double rigid_surface_get_friction(const struct chunks *chunks, struct rect rect);

#endif //MYCOOLGAMEENGINE_RIGID_SURFACE_H
