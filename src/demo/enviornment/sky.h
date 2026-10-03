//
// Created by Nikita on 01.10.2026.
//

#ifndef MYCOOLGAMEENGINE_SKY_H
#define MYCOOLGAMEENGINE_SKY_H

struct parser;
struct entity;

const char* sky_component_key(void);

struct component* sky_create(struct parser* parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_SKY_H
