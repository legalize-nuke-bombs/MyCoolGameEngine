//
// Created by nikita on 01.10.2026.
//

#ifndef MYCOOLGAMEENGINE_RIGID_BODY_H
#define MYCOOLGAMEENGINE_RIGID_BODY_H

struct parser;
struct entity;
struct rigid_body;

const char* rigid_body_component_key(void);

struct component* rigid_body_create(struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_RIGID_BODY_H
