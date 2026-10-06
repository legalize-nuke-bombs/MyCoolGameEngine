//
// Created by nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_IDLE_H
#define MYCOOLGAMEENGINE_IDLE_H

#include "../../../api.h"

struct entity;
struct component_vtable;

MCGE_API const char* idle_component_key(void);

MCGE_API extern const struct component_vtable idle_vtable;

#endif //MYCOOLGAMEENGINE_IDLE_H
