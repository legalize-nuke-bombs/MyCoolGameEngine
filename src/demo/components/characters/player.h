//
// Created by Nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_PLAYER_H
#define MYCOOLGAMEENGINE_PLAYER_H

struct entity;
struct parser;

const char* player_component_key(void);

struct component* player_create(struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_PLAYER_H
