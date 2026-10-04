//
// Created by Nikita on 04.10.2026.
//

#include "effects.h"

#include <stdlib.h>

#include "../../scene/components/component_internal.h"


struct effects {
    struct component base;
    double lengths[EFFECTS_NUM];
};


const char* effects_component_key(void) {
    return "effects";
}
static struct component* effects_clone(struct component base, const struct component* component);
static void effects_simulation_chunk_update(struct component *base, const struct update_context *context);

static const struct component_vtable effects_vtable = {
    .component_key = effects_component_key,
    .on_clone = effects_clone,
    .on_simulation_chunk_update = effects_simulation_chunk_update
};

struct component* effects_create(struct parser *parser, struct entity *parent) {
    struct effects* this = calloc(1, sizeof(struct effects));
    struct component* base = (struct component*)this;
    component_base_create(base, &effects_vtable, parent);
    return base;
}

static struct component* effects_clone(struct component base, const struct component* component) {
    struct effects* effects = (struct effects*)component;
    struct effects* this = calloc(1, sizeof(struct effects));
    this->base = base;
    return (struct component*)this;
}

static void effects_simulation_chunk_update(struct component *base, const struct update_context *context) {
    struct effects* this = (struct effects*)base;
    double dt = context->dt;
    for (int i = 0; i < EFFECTS_NUM; i++) {
        this->lengths[i] -= dt;
        if (this->lengths[i] < 0) {
            this->lengths[i] = 0;
        }
    }
}

void effects_set_effect(struct effects *this, const enum effect effect, const double length) {
    if (length > this->lengths[effect]) {
        this->lengths[effect] = length;
    }
}
bool effects_has_effect(const struct effects *this, const enum effect effect) {
    return this->lengths[effect] > 1e-3;
}