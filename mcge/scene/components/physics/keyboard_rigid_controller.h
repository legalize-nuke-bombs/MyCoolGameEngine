#ifndef MYCOOLGAMEENGINE_KEYBOARD_RIGID_CONTROLLER_H
#define MYCOOLGAMEENGINE_KEYBOARD_RIGID_CONTROLLER_H

#include "../../../api.h"

struct entity;
struct component_vtable;

MCGE_API const char* keyboard_rigid_controller_component_key(void);

MCGE_API extern const struct component_vtable keyboard_rigid_controller_vtable;

#endif //MYCOOLGAMEENGINE_KEYBOARD_RIGID_CONTROLLER_H
