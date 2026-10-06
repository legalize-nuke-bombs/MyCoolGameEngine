//
// Created by Nikita on 26.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_SETTINGS_H
#define MYCOOLGAMEENGINE_RENDERER_SETTINGS_H

#include "../../../api.h"

struct entity;
struct component_vtable;

MCGE_API const char* renderer_settings_component_key(void);

MCGE_API extern const struct component_vtable renderer_settings_vtable;

#endif //MYCOOLGAMEENGINE_RENDERER_SETTINGS_H
