//
// Created by nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_COMPONENT_FACTORY_H
#define MYCOOLGAMEENGINE_COMPONENT_FACTORY_H

#include "../../api.h"

struct fields;
struct entity;
struct component;
struct component_vtable;

MCGE_API void component_factory_create(void);
MCGE_API void component_factory_destroy(void);

MCGE_API void component_factory_register(const struct component_vtable *vtable);
MCGE_API struct component* component_factory_produce(const char *key, struct fields *fields, struct entity *parent);

#endif //MYCOOLGAMEENGINE_COMPONENT_FACTORY_H
