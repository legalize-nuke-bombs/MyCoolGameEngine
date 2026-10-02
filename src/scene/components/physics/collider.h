//
// Created by nikita on 02.10.2026.
//

#ifndef MYCOOLGAMEENGINE_COLLIDER_H
#define MYCOOLGAMEENGINE_COLLIDER_H

struct collider;
struct parser;
struct entity;
struct rect;

const char* collider_component_key(void);

struct component* collider_create(struct parser *parser, struct entity *parent);

struct entity* collider_try_get_obstacle(const struct collider *this, struct rect rect);

#endif //MYCOOLGAMEENGINE_COLLIDER_H
