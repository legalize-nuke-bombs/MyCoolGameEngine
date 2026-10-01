//
// Created by nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_IDLE_H
#define MYCOOLGAMEENGINE_IDLE_H

struct entity;
struct parser;

const char* idle_component_key(void);

struct component* idle_create(struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_IDLE_H
