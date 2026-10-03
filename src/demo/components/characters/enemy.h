//
// Created by nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_ENEMY_H
#define MYCOOLGAMEENGINE_ENEMY_H

struct entity;
struct parser;

const char* enemy_component_key(void);

struct component* enemy_create(struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_ENEMY_H
