//
// Created by nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_SKILL_INTERNAL_H
#define MYCOOLGAMEENGINE_SKILL_INTERNAL_H

#include <stdbool.h>
#include <stddef.h>

#include "skill.h"
#include <mcge/mcge.h>

struct fields;

struct skill_vtable {
    const char* key;
    size_t size;
    void (*on_create)(struct skill *this, struct fields *fields);
    void (*on_destroy)(struct skill *this);
    void (*on_enable)(struct skill *this);
    bool (*on_invoke)(struct skill *this);
};

struct skill {
    const struct skill_vtable* vtable;

    double manacost;

    double cool_timer;
    double cooldown;

    double execution_time;
    bool interruptable;

    struct entity *self;
    uint128_t hands_id;
    uint128_t mana_id;
};

void skill_base_create(struct skill *this, const struct skill_vtable *vtable, double manacost, double cooldown, double execution_time, bool interruptable, struct entity *self);

struct entity* skill_self(const struct skill *this);

#endif //MYCOOLGAMEENGINE_SKILL_INTERNAL_H
