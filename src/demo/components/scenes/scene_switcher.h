//
// Created by Nikita on 30.09.2026.
//

#ifndef MYCOOLGAMEENGINE_SCENE_SWITCHER_H
#define MYCOOLGAMEENGINE_SCENE_SWITCHER_H

struct entity;
struct parser;

const char* scene_switcher_component_key(void);

struct component* scene_switcher_create(struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_SCENE_SWITCHER_H
