//
// Created by Nikita on 30.09.2026.
//

#ifndef MYCOOLGAMEENGINE_SCENE_SWITCHER_ON_KEY_PRESSED_H
#define MYCOOLGAMEENGINE_SCENE_SWITCHER_ON_KEY_PRESSED_H

#include "mcge/api.h"

struct entity;
struct component_vtable;

MCGE_API const char* scene_switcher_on_key_pressed_component_key(void);

MCGE_API extern const struct component_vtable scene_switcher_on_key_pressed_vtable;

#endif //MYCOOLGAMEENGINE_SCENE_SWITCHER_ON_KEY_PRESSED_H
