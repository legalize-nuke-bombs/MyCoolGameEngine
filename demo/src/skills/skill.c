//
// Created by nikita on 03.10.2026.
//

#include <stdlib.h>

#include "skill_internal.h"
#include <mcge/mcge.h>
#include "../characters/mana.h"

void skill_base_create(struct skill *this, const struct skill_vtable *vtable, const double manacost, const double cooldown, struct entity *self) {
    this->vtable = vtable;
    this->manacost = manacost;
    this->cool_timer = 1e+9;
    this->cooldown = cooldown;
    this->self = self;
}

struct entity* skill_self(const struct skill* this) {
    return this->self;
}

void skill_destroy(struct skill *this) {
    if (this->vtable->on_destroy) {
        this->vtable->on_destroy(this);
    }
    free(this);
}

void skill_enable(struct skill *this) {
    this->mana = (struct mana*)entity_get_component(this->self, "mana", entity_query_local);
    if (this->vtable->on_enable) {
        this->vtable->on_enable(this);
    }
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
    if (this->cooldown > this->cool_timer) {
        logger_debug("Failed to invoke skill %s: cooldown", this->vtable->key);
        return skill_invoke_cooldown;
    }
    if (this->mana == NULL || this->manacost > mana_amount(this->mana)) {
        logger_debug("Failed to invoke skill %s: insufficient mana", this->vtable->key);
        return skill_invoke_insufficient_mana;
    }
    if (this->vtable->on_invoke == NULL) {
        logger_debug("Failed to invoke skill %s: vfunc is null", this->vtable->key);
        return skill_invoke_impossible;
    }
    if (this->vtable->on_invoke(this)) {
        logger_debug("Skill %s invoked", this->vtable->key);
        this->cool_timer = 0;
        mana_try_take(this->mana, this->manacost);
        return skill_invoke_ok;
    }
    logger_debug("Failed to invoke skill %s: child class declined", this->vtable->key);
    return skill_invoke_impossible;
}