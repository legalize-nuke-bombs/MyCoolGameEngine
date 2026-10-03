//
// Created by nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_COMPONENT_FACTORY_H
#define MYCOOLGAMEENGINE_COMPONENT_FACTORY_H

struct parser;
struct entity;
struct component_factory;

struct factory* component_factory_create();

struct component* component_factory_produce(const struct component_factory *this, const char *key, struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_COMPONENT_FACTORY_H
