#include "simulator.h"

#include <stdlib.h>

#include "../component_internal.h"
#include "../../entity.h"
#include "../../scene.h"
#include "../../chunks/chunks.h"
#include "../../../utils/dictionary.h"
#include "../../../utils/parser.h"


struct simulator {
    struct component base;

    double simulation_distance;

    const struct chunks *chunks;
};

static struct component* simulator_clone(struct component base, const struct component *component);
static void simulator_awake(struct component *base);
static void simulator_update(struct component *base, const struct update_context *context);

static const struct component_vtable simulator_vtable = {
    .component_key = simulator_component_key,
    .on_clone = simulator_clone,
    .on_awake = simulator_awake,
    .on_update = simulator_update
};

const char* simulator_component_key(void) {
    return "simulator";
}

struct component* simulator_create(struct parser *parser, struct entity *parent) {
    struct simulator *this = calloc(1, sizeof(struct simulator));
    struct component *base = (struct component *) this;
    component_base_create(base, &simulator_vtable, parent);

    parser_next_double(parser, &this->simulation_distance);

    return base;
}

static struct component* simulator_clone(struct component base, const struct component *component) {
    const struct simulator *simulator = (struct simulator *) component;

    struct simulator* this = calloc(1, sizeof(struct simulator));
    this->base = base;
    this->simulation_distance = simulator->simulation_distance;
    return (struct component*)this;
}

static void simulator_awake(struct component *base) {
    struct simulator *this = (struct simulator *) base;

    const struct scene* scene = entity_get_scene(component_get_parent(base));
    this->chunks = scene_get_chunks(scene);
}

static void simulator_simulate(const struct simulator *this, const struct update_context *context) {
    const struct rect rect = {
        .position = component_get_rect((const struct component*)this).position,
        .size.x = 2 * this->simulation_distance,
        .size.y = 2 * this->simulation_distance
    };

    int x_start, x_end, y_start, y_end;
    chunks_get_rect_indexes(this->chunks, rect, &x_start, &x_end, &y_start, &y_end);

    for (int x = x_start; x <= x_end; x++) {
        for (int y = y_start; y <= y_end; y++) {
            const struct dictionary *types = chunks_chunk_get_types(this->chunks, x, y);
            if (types == NULL) {
                continue;
            }
            struct dictionary_iterator types_iterator = dictionary_begin(types);
            struct dictionary_node node;
            while (dictionary_next(types, &types_iterator, &node)) {
                const struct dictionary* typed_components = node.value;

                struct dictionary_iterator typed_components_iterator = dictionary_begin(typed_components);
                while (dictionary_next(typed_components, &typed_components_iterator, &node)) {
                    struct component* component = node.value;
                    if (!component_is_simulation_chunkable(component)) {
                        break;
                    }
                    component_simulation_chunk_update(component, context);
                }
            }
        }
    }
}

static void simulator_update(struct component *base, const struct update_context *context) {
    const struct simulator *this = (struct simulator *) base;
    simulator_simulate(this, context);
}