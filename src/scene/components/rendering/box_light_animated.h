//
// Created by nikita on 28.09.2026.
//

#ifndef MYCOOLGAMEENGINE_BOX_LIGHT_ANIMATED_H
#define MYCOOLGAMEENGINE_BOX_LIGHT_ANIMATED_H

struct box_light_animated;
struct entity;
struct parser;

const char* box_light_animated_component_key(void);

struct component* box_light_animated_create(struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_BOX_LIGHT_ANIMATED_H
