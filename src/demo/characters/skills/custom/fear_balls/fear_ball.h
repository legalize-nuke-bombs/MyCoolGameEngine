//
// Created by Nikita on 04.10.2026.
//

#ifndef MYCOOLGAMEENGINE_FEAR_BALL_H
#define MYCOOLGAMEENGINE_FEAR_BALL_H

#include "../../../../../utils/vector2.h"

struct entity;
struct parser;
struct fear_ball;

const char* fear_ball_component_key(void);

struct component* fear_ball_create(struct parser *parser, struct entity *parent);

void fear_ball_set_direction(struct fear_ball* this, struct vector2 direction);

#endif //MYCOOLGAMEENGINE_FEAR_BALL_H
