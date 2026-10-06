//
// Created by nikita on 28.09.2026.
//

#ifndef MYCOOLGAMEENGINE_BOX_RENDERER_ANIMATED_H
#define MYCOOLGAMEENGINE_BOX_RENDERER_ANIMATED_H

#include "../../../api.h"

struct component_vtable;
struct entity;

MCGE_API const char* box_renderer_animated_component_key(void);

MCGE_API extern const struct component_vtable box_renderer_animated_vtable;

#endif //MYCOOLGAMEENGINE_BOX_RENDERER_ANIMATED_H
