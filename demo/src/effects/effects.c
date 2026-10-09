//
// Created by nikita on 09.10.2026.
//

#include "effects.h"

#include <string.h>

#include "effect.h"
#include <mcge/mcge.h>

struct effects {
    struct component base;

    struct list effects;
    bool updating;
};

static void effects_on_create(struct component *base, struct fields *fields);
static void effects_destroy(struct component *base);
static void effects_disable(struct component *base);
static void effects_simulation_chunk_update(struct component *base, const struct update_context *context);

const struct component_vtable effects_vtable = {
    .component_key = effects_component_key,
    .size = sizeof(struct effects),
    .on_create = effects_on_create,
    .on_destroy = effects_destroy,
    .on_disable = effects_disable,
    .on_simulation_chunk_update = effects_simulation_chunk_update
};

const char* effects_component_key(void) {
    return "effects";
}

static void effects_on_create(struct component *base, struct fields *fields) {
    struct effects *this = (struct effects *) base;
    this->effects = list_create(1);
}

static void effects_mark_all_destroyed(const struct effects *this) {
    for (int i = 0; i < list_count(&this->effects); i++) {
        struct effect *effect = list_get(&this->effects, i);
        if (effect) {
            effect_mark_destroyed(effect);
        }
    }
}

static void effects_collect_garbage(struct effects *this) {
    for (int i = 0; i < list_count(&this->effects); i++) {
        struct effect *effect = list_get(&this->effects, i);
        if (effect == NULL || effect_is_alive(effect)) {
            continue;
        }
        list_set(&this->effects, i, NULL);
        effect_destroy(effect);
    }
    list_remove_nulls(&this->effects);
}

static void effects_destroy(struct component *base) {
    struct effects *this = (struct effects *) base;
    effects_mark_all_destroyed(this);
    effects_collect_garbage(this);
    list_destroy(&this->effects);
}

static void effects_disable(struct component *base) {
    struct effects *this = (struct effects *) base;
    effects_mark_all_destroyed(this);
    if (!this->updating) {
        effects_collect_garbage(this);
    }
}

static void effects_simulation_chunk_update(struct component *base, const struct update_context *context) {
    struct effects *this = (struct effects *) base;
    this->updating = true;
    for (int i = 0; i < list_count(&this->effects); i++) {
        struct effect *effect = list_get(&this->effects, i);
        if (effect_is_alive(effect)) {
            effect_update(effect, context->dt);
        }
    }
    this->updating = false;
    effects_collect_garbage(this);
}

void effects_apply(struct effects *this, struct effect *effect) {
    if (effect == NULL) {
        return;
    }
    if (!component_is_alive(&this->base)) {
        effect_destroy(effect);
        return;
    }
    struct effect *same = effects_find(this, effect_key(effect));
    if (same && effect_try_refresh(same, effect)) {
        effect_destroy(effect);
        return;
    }
    list_add(&this->effects, effect);
}

struct effect* effects_find(const struct effects *this, const char *key) {
    for (int i = 0; i < list_count(&this->effects); i++) {
        struct effect *effect = list_get(&this->effects, i);
        if (effect == NULL || !effect_is_alive(effect)) {
            continue;
        }
        if (strcmp(effect_key(effect), key) == 0) {
            return effect;
        }
    }
    return NULL;
}

bool effects_has(const struct effects *this, const char *key) {
    return effects_find(this, key) != NULL;
}
