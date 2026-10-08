//
// Created by nikita on 08.10.2026.
//

#ifndef MYCOOLGAMEENGINE_MOVEMENT_H
#define MYCOOLGAMEENGINE_MOVEMENT_H

#include <mcge/mcge.h>

struct movement;

const char* movement_component_key();

extern const struct component_vtable movement_vtable;

void movement_try_move(struct movement *this, struct vector2 direction);

#endif //MYCOOLGAMEENGINE_MOVEMENT_H
