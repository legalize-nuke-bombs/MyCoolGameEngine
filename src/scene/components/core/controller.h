//
// Created by Nikita on 26.09.2026.
//

#ifndef MYCOOLGAMEENGINE_CONTROLLER_H
#define MYCOOLGAMEENGINE_CONTROLLER_H

struct entity;
struct parser;

const char* controller_component_key(void);

struct component* controller_create(struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_CONTROLLER_H
