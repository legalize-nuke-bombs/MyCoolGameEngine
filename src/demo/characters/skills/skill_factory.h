//
// Created by nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_SKILL_FACTORY_H
#define MYCOOLGAMEENGINE_SKILL_FACTORY_H

struct skill_factory;
struct parser;
struct entity;

struct factory* skill_factory_create();
struct skill* skill_produce(const struct skill_factory *this, const char* key, struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_SKILL_FACTORY_H
