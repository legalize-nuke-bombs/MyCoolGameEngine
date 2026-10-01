//
// Created by nikita on 01.10.2026.
//

#ifndef MYCOOLGAMEENGINE_RIGID_BODY_H
#define MYCOOLGAMEENGINE_RIGID_BODY_H

struct parser;
struct entity;
struct rigid_body;
struct vector2;

const char* rigid_body_component_key(void);

struct component* rigid_body_create(struct parser *parser, struct entity *parent);

void rigid_body_push_off(struct rigid_body *this, struct vector2 push_force, double dt);

#endif //MYCOOLGAMEENGINE_RIGID_BODY_H
