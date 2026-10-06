//
// Created by nikita on 01.10.2026.
//

#ifndef MYCOOLGAMEENGINE_RIGID_SURFACE_H
#define MYCOOLGAMEENGINE_RIGID_SURFACE_H

struct component_vtable;
struct entity;
struct chunks;

#include "../../../utils/rect.h"

const char* rigid_surface_component_key(void);

extern const struct component_vtable rigid_surface_vtable;

double rigid_surface_get_friction(const struct chunks *chunks, struct rect rect);

#endif //MYCOOLGAMEENGINE_RIGID_SURFACE_H
