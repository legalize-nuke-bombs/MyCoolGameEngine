//
// Created by nikita on 28.09.2026.
//

#ifndef MYCOOLGAMEENGINE_BOX_RENDERER_ANIMATED_H
#define MYCOOLGAMEENGINE_BOX_RENDERER_ANIMATED_H

struct parser;
struct entity;

const char* box_renderer_animated_component_key(void);

struct component* box_renderer_animated_create(struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_BOX_RENDERER_ANIMATED_H
