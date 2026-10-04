//
// Created by Nikita on 04.10.2026.
//

#ifndef MYCOOLGAMEENGINE_FEAR_BALL_H
#define MYCOOLGAMEENGINE_FEAR_BALL_H

struct entity;
struct parser;

const char* fear_ball_component_key(void);

struct component* fear_ball_create(struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_FEAR_BALL_H
