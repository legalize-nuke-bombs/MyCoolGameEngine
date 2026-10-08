//
// Created by nikita on 03.10.2026.
//

#include <stdlib.h>

#include "skill_internal.h"
#include <mcge/mcge.h>
#include "../characters/mana/mana.h"
#include "../characters/hands/hands.h"

void skill_base_create(struct skill *this, const struct skill_vtable *vtable, const double manacost, const double cooldown, const double execution_time, const bool interruptable, struct entity *self) {
    this->vtable = vtable;
    this->manacost = manacost;
    this->cool_timer = 1e+9;
    this->cooldown = cooldown;
    this->execution_time = execution_time;
    this->interruptable = interruptable;
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
    const struct component* hands = entity_get_component(this->self, "hands", entity_query_local);
    if (hands) this->hands_id = component_get_id(hands);

    const struct component *mana = entity_get_component(this->self, "mana", entity_query_local);
    if (mana) this->mana_id = component_get_id(mana);

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

static void skill_invoke(void *executor, void *context) {
    struct skill* this = executor;
    logger_debug("Skill %s is invoked", this->vtable->key);
    this->cool_timer = 0;
    if (this->vtable->on_invoke) this->vtable->on_invoke(this);
}


enum skill_invoke_result skill_schedule_invoke(struct skill *this) {
    if (this->cooldown > this->cool_timer) {
        logger_debug("Failed to schedule skill invoke %s: cooldown", this->vtable->key);
        return skill_invoke_cooldown;
    }
    struct mana *mana = (struct mana*)scene_try_get_component(this->mana_id);
    if (mana == NULL || this->manacost > mana_amount(mana)) {
        logger_debug("Failed to schedule skill invoke %s: insufficient mana", this->vtable->key);
        return skill_invoke_insufficient_mana;
    }
    struct hands *hands = (struct hands*)scene_try_get_component(this->hands_id);
    if (hands == NULL) {
        logger_warn("Failed to schedule skill invoke %s: hands component is null", this->vtable->key);
        return skill_invoke_runtime_error;
    }

    const struct hands_action hands_action = {
        .name = "skill",
        .priority = this->interruptable ? HANDS_ACTION_PRIORITY_INTERRUPTIBLE_SKILL : HANDS_ACTION_UNINTERRUPTIBLE_SKILL,
        .duration = this->execution_time,
        .method.executor = this,
        .method.context = NULL,
        .method.func = skill_invoke
    };
    if (hands_try_put(hands, hands_action)) {
        logger_debug("Skill %s invoke scheduled", this->vtable->key);
        mana_try_take(mana, this->manacost);
        return skill_invoke_scheduled;
    }

    logger_debug("Failed to schedule skill invoke %s: hands are busy", this->vtable->key);
    return skill_invoke_hands_are_busy;
}