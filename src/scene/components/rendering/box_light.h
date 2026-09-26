//
// Created by nikita on 26.09.2026.
//

#ifndef MYCOOLGAMEENGINE_BOX_LIGHT_H
#define MYCOOLGAMEENGINE_BOX_LIGHT_H

struct box_light;
struct entity;
struct parser;

const char* box_light_component_key(void);

struct component* box_light_create(struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_BOX_LIGHT_H
