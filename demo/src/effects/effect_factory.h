//
// Created by nikita on 09.10.2026.
//

#ifndef MYCOOLGAMEENGINE_EFFECT_FACTORY_H
#define MYCOOLGAMEENGINE_EFFECT_FACTORY_H

struct fields;
struct entity;
struct effect;

struct effect* effect_factory_produce(struct fields *fields, struct entity *self);

#endif //MYCOOLGAMEENGINE_EFFECT_FACTORY_H
