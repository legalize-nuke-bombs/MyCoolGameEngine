#ifndef MYCOOLGAMEENGINE_ENTITY_FACTORY_H
#define MYCOOLGAMEENGINE_ENTITY_FACTORY_H

struct entity;
struct fields;

struct entity* entity_factory_produce(struct fields *fields);

#endif //MYCOOLGAMEENGINE_ENTITY_FACTORY_H
