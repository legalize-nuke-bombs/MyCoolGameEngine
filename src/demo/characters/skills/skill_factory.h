//
// Created by nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_SKILL_FACTORY_H
#define MYCOOLGAMEENGINE_SKILL_FACTORY_H

struct parser;
struct entity;
struct skill;

struct skill* skill_factory_produce(const char* key, struct parser *parser, struct entity *self);

#endif //MYCOOLGAMEENGINE_SKILL_FACTORY_H
