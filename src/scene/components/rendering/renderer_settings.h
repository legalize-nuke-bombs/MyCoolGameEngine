//
// Created by Nikita on 26.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_SETTINGS_H
#define MYCOOLGAMEENGINE_RENDERER_SETTINGS_H

struct entity;
struct parser;

const char* renderer_settings_component_key(void);

struct component* renderer_settings_create(struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_RENDERER_SETTINGS_H
