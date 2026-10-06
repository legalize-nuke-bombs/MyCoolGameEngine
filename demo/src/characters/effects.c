//
// Created by Nikita on 04.10.2026.
//

#include "effects.h"

#include <stdlib.h>

#include <mcge/scene/components/component_internal.h>


struct effects {
    struct component base;
    double lengths[EFFECTS_NUM];
};


const char* effects_component_key(void) {
    return "effects";
}
static void effects_simulation_chunk_update(struct component *base, const struct update_context *context);

const struct component_vtable effects_vtable = {
    .component_key = effects_component_key,
    .size = sizeof(struct effects),
    .on_simulation_chunk_update = effects_simulation_chunk_update
};


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
    this->lengths[effect] = length;
}
double effects_get_effect(const struct effects *this, const enum effect effect) {
    return this->lengths[effect];
}
bool effects_has_effect(const struct effects *this, const enum effect effect) {
    return this->lengths[effect] > 1e-3;
}