//
// Created by Nikita on 30.09.2026.
//

#ifndef MYCOOLGAMEENGINE_SCENE_SWITCHER_H
#define MYCOOLGAMEENGINE_SCENE_SWITCHER_H

struct entity;
struct component_vtable;

const char* scene_switcher_component_key(void);

extern const struct component_vtable scene_switcher_vtable;

#endif //MYCOOLGAMEENGINE_SCENE_SWITCHER_H
