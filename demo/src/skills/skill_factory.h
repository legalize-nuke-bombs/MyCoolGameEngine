//
// Created by nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_SKILL_FACTORY_H
#define MYCOOLGAMEENGINE_SKILL_FACTORY_H

struct fields;
struct entity;
struct skill;

struct skill* skill_factory_produce(struct fields *fields, struct entity *self);

#endif //MYCOOLGAMEENGINE_SKILL_FACTORY_H
