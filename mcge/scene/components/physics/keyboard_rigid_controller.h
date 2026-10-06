#ifndef MYCOOLGAMEENGINE_KEYBOARD_RIGID_CONTROLLER_H
#define MYCOOLGAMEENGINE_KEYBOARD_RIGID_CONTROLLER_H

struct entity;
struct component_vtable;

const char* keyboard_rigid_controller_component_key(void);

extern const struct component_vtable keyboard_rigid_controller_vtable;

#endif //MYCOOLGAMEENGINE_KEYBOARD_RIGID_CONTROLLER_H
