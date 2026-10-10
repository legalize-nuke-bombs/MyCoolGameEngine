#include "simulator.h"

#include <stdlib.h>

#include "../component_internal.h"
#include "../../entity.h"
#include "../../chunks/chunks_algorithms.h"
#include "../../../utils/fields.h"


struct simulator {
    struct component base;

    double simulation_distance;
};

static void simulator_on_create(struct component *base, struct fields *fields);
static void simulator_update(struct component *base, const struct update_context *context);

const struct component_vtable simulator_vtable = {
    .component_key = simulator_component_key,
    .size = sizeof(struct simulator),
    .on_create = simulator_on_create,
    .on_update = simulator_update
};

const char* simulator_component_key(void) {
    return "simulator";
}

static void simulator_on_create(struct component *base, struct fields *fields) {
    struct simulator *this = (struct simulator *) base;
    this->simulation_distance = fields_get_double(fields, "distance", 0);
}

static void simulator_update_component(struct component *component, void *context) {
    const struct update_context *update_context = context;
    component_simulation_chunk_update(component, update_context);
}

static void simulator_simulate(const struct simulator *this, const struct update_context *context) {
    const struct rect rect = {
        .position = component_get_rect((const struct component*)this).position,
        .size.x = 2 * this->simulation_distance,
        .size.y = 2 * this->simulation_distance
    };
    chunks_algorithms_for_each(rect, component_is_simulation_chunkable, simulator_update_component, (void*)context);
}

static void simulator_update(struct component *base, const struct update_context *context) {
    const struct simulator *this = (struct simulator *) base;
    simulator_simulate(this, context);
}