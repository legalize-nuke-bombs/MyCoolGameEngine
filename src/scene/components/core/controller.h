//
// Created by Nikita on 26.09.2026.
//

#ifndef MYCOOLGAMEENGINE_CONTROLLER_H
#define MYCOOLGAMEENGINE_CONTROLLER_H

#include "../../../utils/vector2.h"

struct entity;
struct parser;
struct controller;

const char* controller_component_key(void);

struct component* controller_create(struct parser *parser, struct entity *parent);

void controller_move(const struct controller* this, struct vector2 direction, double dt);

#endif //MYCOOLGAMEENGINE_CONTROLLER_H
