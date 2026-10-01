//
// Created by Nikita on 01.10.2026.
//

#ifndef MYCOOLGAMEENGINE_CLOCK_H
#define MYCOOLGAMEENGINE_CLOCK_H

struct parser;
struct entity;
struct clock;

const char* clock_component_key(void);

struct component* clock_create(struct parser* parser, struct entity *parent);

double clock_get_cycle_progress(const struct clock *this);

#endif //MYCOOLGAMEENGINE_CLOCK_H
