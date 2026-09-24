//
// Created by Nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_SUICIDAL_H
#define MYCOOLGAMEENGINE_SUICIDAL_H

struct parser;
struct entity;

const char* suicidal_component_key(void);

struct component* suicidal_create(struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_SUICIDAL_H
