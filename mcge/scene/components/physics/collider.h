//
// Created by nikita on 02.10.2026.
//

#ifndef MYCOOLGAMEENGINE_COLLIDER_H
#define MYCOOLGAMEENGINE_COLLIDER_H
#include <stdbool.h>
#include "../../../api.h"

struct collider;
struct component_vtable;
struct entity;
struct rect;

MCGE_API const char* collider_component_key(void);

MCGE_API extern const struct component_vtable collider_vtable;

MCGE_API struct entity* collider_try_get_obstacle(const struct collider *this, struct rect rect);

MCGE_API const struct rigid_material* collider_get_rigid_material(const struct collider *this);
MCGE_API struct action* collider_on_enter(struct collider *this);
MCGE_API struct action* collider_on_exit(struct collider *this);

#endif //MYCOOLGAMEENGINE_COLLIDER_H
