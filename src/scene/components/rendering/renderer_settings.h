//
// Created by Nikita on 26.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_SETTINGS_H
#define MYCOOLGAMEENGINE_RENDERER_SETTINGS_H

struct entity;
struct component_vtable;

const char* renderer_settings_component_key(void);

extern const struct component_vtable renderer_settings_vtable;

#endif //MYCOOLGAMEENGINE_RENDERER_SETTINGS_H
