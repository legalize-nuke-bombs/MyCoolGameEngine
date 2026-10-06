//
// Created by nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_COMPONENT_FACTORY_H
#define MYCOOLGAMEENGINE_COMPONENT_FACTORY_H

struct fields;
struct entity;
struct component;
struct component_vtable;

void component_factory_create(void);
void component_factory_destroy(void);

void component_factory_register(const struct component_vtable *vtable);
struct component* component_factory_produce(const char *key, struct fields *fields, struct entity *parent);

#endif //MYCOOLGAMEENGINE_COMPONENT_FACTORY_H
