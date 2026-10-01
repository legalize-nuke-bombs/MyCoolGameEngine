//
// Created by Nikita on 02.10.2026.
//

#ifndef MYCOOLGAMEENGINE_SIMULATOR_H
#define MYCOOLGAMEENGINE_SIMULATOR_H


struct parser;
struct entity;

const char* simulator_component_key();

struct component* simulator_create(struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_SIMULATOR_H
