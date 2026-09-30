//
// Created by Nikita on 30.09.2026.
//

#ifndef MYCOOLGAMEENGINE_MENU_H
#define MYCOOLGAMEENGINE_MENU_H

struct entity;
struct parser;

const char* menu_component_key(void);

struct component* menu_create(struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_MENU_H
