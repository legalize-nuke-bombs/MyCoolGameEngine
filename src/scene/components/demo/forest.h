//
// Created by nikita on 28.09.2026.
//

#ifndef MYCOOLGAMEENGINE_FOREST_H
#define MYCOOLGAMEENGINE_FOREST_H

struct parser;
struct entity;

const char* forest_component_key(void);

struct component* forest_create(struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_FOREST_H
