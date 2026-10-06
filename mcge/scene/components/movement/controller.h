//
// Created by Nikita on 26.09.2026.
//

#ifndef MYCOOLGAMEENGINE_CONTROLLER_H
#define MYCOOLGAMEENGINE_CONTROLLER_H

#include "../../../utils/vector2.h"
#include "../../../api.h"

struct entity;
struct component_vtable;
struct controller;

MCGE_API const char* controller_component_key(void);

MCGE_API extern const struct component_vtable controller_vtable;

MCGE_API void controller_move(const struct controller* this, struct vector2 direction, double dt);

#endif //MYCOOLGAMEENGINE_CONTROLLER_H
