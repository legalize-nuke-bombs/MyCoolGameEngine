#ifndef MYCOOLGAMEENGINE_ENTITY_FACTORY_H
#define MYCOOLGAMEENGINE_ENTITY_FACTORY_H

#include "../api.h"

struct entity;
struct fields;

MCGE_API struct entity* entity_factory_produce(struct fields *fields);

#endif //MYCOOLGAMEENGINE_ENTITY_FACTORY_H
