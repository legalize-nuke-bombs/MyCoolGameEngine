//
// Created by nikita on 03.10.2026.
//

#include <stdlib.h>

#include "skill_internal.h"
#include "../../../../logging/logger.h"

struct skill {
    const struct skill_vtable* vtable;
    double cool_timer;
    double cooldown;
};

void skill_base_create(struct skill *this, const struct skill_vtable *vtable, double cooldown) {
    this->vtable = vtable;
    this->cooldown = cooldown;
}

void skill_destroy(struct skill *this) {
    if (this->vtable->on_destroy) {
        this->vtable->on_destroy(this);
    }
    free(this);
}

void skill_update(struct skill *this, const double dt) {
    this->cool_timer += dt;
}

double skill_get_cooldown_left(const struct skill *this) {
    if (this->cool_timer >= this->cooldown) {
        return 0;
    }
    return this->cooldown - this->cool_timer;
}
double skill_get_cooldown_full(const struct skill *this) {
    return this->cooldown;
}


enum skill_invoke_result skill_invoke(struct skill *this) {
    if (this->cool_timer < this->cooldown) {
        logger_debug("Failed to invoke skill %s: cooldown", this->vtable->key);
        return skill_invoke_cooldown;
    }
    if (this->vtable->on_invoke == NULL) {
        logger_debug("Failed to invoke skill %s: vfunc is null", this->vtable->key);
        return skill_invoke_impossible;
    }
    if (this->vtable->on_invoke(this)) {
        logger_debug("Skill %s invoked", this->vtable->key);
        this->cool_timer = 0;
        return skill_invoke_ok;
    }
    logger_debug("Failed to invoke skill %s: child class declined", this->vtable->key);
    return skill_invoke_impossible;
}