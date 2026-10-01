//
// Created by Nikita on 26.09.2026.
//

#ifndef MYCOOLGAMEENGINE_KEYBOARD_CONTROLLER_H
#define MYCOOLGAMEENGINE_KEYBOARD_CONTROLLER_H

struct entity;
struct parser;

const char* keyboard_controller_component_key(void);

struct component* keyboard_controller_create(struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_KEYBOARD_CONTROLLER_H
