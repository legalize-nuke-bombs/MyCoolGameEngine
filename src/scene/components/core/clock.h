//
// Created by Nikita on 01.10.2026.
//

#ifndef MYCOOLGAMEENGINE_CLOCK_H
#define MYCOOLGAMEENGINE_CLOCK_H

struct parser;
struct entity;

const char* clock_component_key(void);

struct component* clock_create(struct parser* parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_CLOCK_H
