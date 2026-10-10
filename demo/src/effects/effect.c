//
// Created by nikita on 09.10.2026.
//

#include <stdlib.h>

#include "effect_internal.h"

void effect_base_create(struct effect *this, const struct effect_vtable *vtable, const double duration, struct entity *self) {
    this->vtable = vtable;
    this->duration = duration;
    this->timer = 0;
    this->alive = true;
    this->self = self;
}

struct entity* effect_self(const struct effect *this) {
    return this->self;
}

void effect_destroy(struct effect *this) {
    if (this->vtable->on_destroy) {
        this->vtable->on_destroy(this);
    }
    free(this);
}

void effect_update(struct effect *this, const double dt) {
    this->timer += dt;
    if (this->vtable->on_update) {
        this->vtable->on_update(this, dt);
    }
    if (this->timer >= this->duration) {
        effect_mark_destroyed(this);
    }
}

const char* effect_key(const struct effect *this) {
    return this->vtable->key;
}

void effect_mark_destroyed(struct effect *this) {
    this->alive = false;
}
bool effect_is_alive(const struct effect *this) {
    return this->alive;
}

bool effect_try_refresh(struct effect *this, const struct effect *incoming) {
    if (this->vtable->on_refresh == NULL) {
        return false;
    }
    this->vtable->on_refresh(this, incoming);
    return true;
}

double effect_get_time_left(const struct effect *this) {
    if (this->timer >= this->duration) {
        return 0;
    }
    return this->duration - this->timer;
}
double effect_get_time_full(const struct effect *this) {
    return this->duration;
}

bool effect_try_get_color(const struct effect *this, struct color *color) {
    if (this->vtable->get_color == NULL) {
        return false;
    }
    *color = this->vtable->get_color();
    return true;
}