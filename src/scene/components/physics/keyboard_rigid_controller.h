#ifndef MYCOOLGAMEENGINE_KEYBOARD_RIGID_CONTROLLER_H
#define MYCOOLGAMEENGINE_KEYBOARD_RIGID_CONTROLLER_H

struct entity;
struct parser;

const char* keyboard_rigid_controller_component_key(void);

struct component* keyboard_rigid_controller_create(struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_KEYBOARD_RIGID_CONTROLLER_H
