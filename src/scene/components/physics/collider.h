//
// Created by nikita on 02.10.2026.
//

#ifndef MYCOOLGAMEENGINE_COLLIDER_H
#define MYCOOLGAMEENGINE_COLLIDER_H
#include <stdbool.h>

struct collider;
struct component_vtable;
struct entity;
struct rect;

const char* collider_component_key(void);

extern const struct component_vtable collider_vtable;

struct entity* collider_try_get_obstacle(const struct collider *this, struct rect rect);

const struct rigid_material* collider_get_rigid_material(const struct collider *this);
bool collider_is_trigger(const struct collider *this);
struct action* collider_on_trigger_enter(struct collider *this);
struct action* collider_on_trigger_exit(struct collider *this);

#endif //MYCOOLGAMEENGINE_COLLIDER_H
