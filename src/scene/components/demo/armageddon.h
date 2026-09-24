//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ARMAGEDDON_H
#define MYCOOLGAMEENGINE_ARMAGEDDON_H

struct parser;
struct entity;

const char* armageddon_component_key(void);

struct component* armageddon_create(struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_ARMAGEDDON_H
