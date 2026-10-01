//
// Created by Nikita on 01.10.2026.
//

#ifndef MYCOOLGAMEENGINE_GAME_CLOSER_H
#define MYCOOLGAMEENGINE_GAME_CLOSER_H

struct entity;
struct parser;

const char* game_closer_component_key(void);

struct component* game_closer_create(struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_GAME_CLOSER_H
