//
// Created by nikita on 09.10.2026.
//

#ifndef MYCOOLGAMEENGINE_EFFECT_INTERNAL_H
#define MYCOOLGAMEENGINE_EFFECT_INTERNAL_H

#include <stdbool.h>
#include <stddef.h>

#include "effect.h"
#include <mcge/mcge.h>

struct fields;

struct effect_vtable {
    const char* key;
    size_t size;
    void (*on_create)(struct effect *this, struct fields *fields);
    void (*on_destroy)(struct effect *this);
    void (*on_update)(struct effect *this, double dt);
    void (*on_refresh)(struct effect *this, const struct effect *incoming);
};

struct effect {
    const struct effect_vtable* vtable;

    double duration;
    double timer;

    bool alive;

    struct entity *self;
};

void effect_base_create(struct effect *this, const struct effect_vtable *vtable, double duration, struct entity *self);

struct entity* effect_self(const struct effect *this);

#endif //MYCOOLGAMEENGINE_EFFECT_INTERNAL_H
