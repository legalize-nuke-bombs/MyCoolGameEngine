//
// Created by nikita on 26.09.2026.
//

#ifndef MYCOOLGAMEENGINE_BOX_LIGHT_H
#define MYCOOLGAMEENGINE_BOX_LIGHT_H

#include "../../../api.h"

struct box_light;
struct entity;
struct component_vtable;

MCGE_API const char* box_light_component_key(void);

MCGE_API extern const struct component_vtable box_light_vtable;

#endif //MYCOOLGAMEENGINE_BOX_LIGHT_H
