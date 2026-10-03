//
// Created by nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_SKILLED_H
#define MYCOOLGAMEENGINE_SKILLED_H

struct entity;
struct parser;

const char* skilled_component_key(void);

struct component* skilled_create(struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_SKILLED_H
