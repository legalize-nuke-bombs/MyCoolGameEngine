//
// Created by Nikita on 02.10.2026.
//

#ifndef MYCOOLGAMEENGINE_SIMULATOR_H
#define MYCOOLGAMEENGINE_SIMULATOR_H

#include "../../../api.h"


struct component_vtable;
struct entity;

MCGE_API const char* simulator_component_key();

MCGE_API extern const struct component_vtable simulator_vtable;

#endif //MYCOOLGAMEENGINE_SIMULATOR_H
