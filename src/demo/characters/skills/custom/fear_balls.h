//
// Created by Nikita on 04.10.2026.
//

#ifndef MYCOOLGAMEENGINE_FEAR_BALLS_H
#define MYCOOLGAMEENGINE_FEAR_BALLS_H

struct parser;
struct entity;

#define SKILL_FEAR_BALLS_KEY "fear_balls"

struct skill* fear_balls_parse(struct parser *parser, struct entity *self);

#endif //MYCOOLGAMEENGINE_FEAR_BALLS_H
