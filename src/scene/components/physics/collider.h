//
// Created by nikita on 02.10.2026.
//

#ifndef MYCOOLGAMEENGINE_COLLIDER_H
#define MYCOOLGAMEENGINE_COLLIDER_H

struct collider;
struct parser;
struct entity;

const char* collider_component_key(void);

struct component* collider_create(struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_COLLIDER_H
