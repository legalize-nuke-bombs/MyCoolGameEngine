//
// Created by nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_COMPONENT_FACTORY_H
#define MYCOOLGAMEENGINE_COMPONENT_FACTORY_H

struct parser;
struct entity;
struct component;

void component_factory_create(void);
void component_factory_destroy(void);

void component_factory_register(const char *key, struct component* (*create)(struct parser *parser, struct entity *parent));
struct component* component_factory_produce(const char *key, struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_COMPONENT_FACTORY_H
