//
// Created by Nikita on 26.09.2026.
//

#ifndef MYCOOLGAMEENGINE_KEYBOARD_CONTROLLER_H
#define MYCOOLGAMEENGINE_KEYBOARD_CONTROLLER_H

struct entity;
struct component_vtable;

const char* keyboard_controller_component_key(void);

extern const struct component_vtable keyboard_controller_vtable;

#endif //MYCOOLGAMEENGINE_KEYBOARD_CONTROLLER_H
