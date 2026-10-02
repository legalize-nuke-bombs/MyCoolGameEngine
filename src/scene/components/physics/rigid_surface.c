//
// Created by nikita on 01.10.2026.
//

#include "rigid_surface.h"

#include <stdlib.h>
#include <string.h>

#include "../component_internal.h"
#include "../../entity.h"
#include "../../scene.h"
#include "../../../utils/parser.h"
#include "../../../catalogs/catalogs.h"
#include "../../../subsystems/subsystem_collection.h"
#include "../../../utils/dictionary.h"
#include "../../chunks/chunks.h"
#include "../../../catalogs/core/rigid_layer.h"
#include "../../../catalogs/core/rigid_material.h"


struct rigid_surface {
    struct component base;

    char* layer_name;
    struct rigid_layer* layer;

    char* material_name;
    struct rigid_material* material;
};

static struct component* rigid_surface_clone(struct component base, const struct component *component);
static void rigid_surface_awake(struct component *base);
static void rigid_surface_on_destroy(struct component *base);
static bool rigid_surface_is_chunkable() {
    return true;
}

static const struct component_vtable rigid_surface_vtable = {
    .component_key = rigid_surface_component_key,
    .on_clone = rigid_surface_clone,
    .on_awake = rigid_surface_awake,
    .is_chunkable = rigid_surface_is_chunkable,
    .on_destroy = rigid_surface_on_destroy
};

const char* rigid_surface_component_key(void) {
    return "rigid_surface";
}

struct component* rigid_surface_create(struct parser *parser, struct entity *parent) {
    struct rigid_surface *this = calloc(1, sizeof(struct rigid_surface));
    struct component *base = (struct component *) this;
    component_base_create(base, &rigid_surface_vtable, parent);

    this->layer_name = parser_next_dup(parser);
    this->material_name = parser_next_dup(parser);

    return base;
}

static struct component* rigid_surface_clone(struct component base, const struct component *component) {
    const struct rigid_surface *rigid_surface = (struct rigid_surface *) component;

    struct rigid_surface* this = calloc(1, sizeof(struct rigid_surface));
    this->base = base;
    if (rigid_surface->layer_name != NULL) {
        this->layer_name = strdup(rigid_surface->layer_name);
    }
    this->layer = rigid_surface->layer;
    if (rigid_surface->material_name != NULL) {
        this->material_name = strdup(rigid_surface->material_name);
    }
    this->material = rigid_surface->material;
    return (struct component*)this;
}

static void rigid_surface_awake(struct component *base) {
    struct rigid_surface *this = (struct rigid_surface *) base;

    const struct catalogs* catalogs = (struct catalogs*)subsystem_collection_get(scene_get_subsystems(entity_get_scene(component_get_parent(base))), "catalogs");

    if (this->layer == NULL) {
        this->layer = catalogs_get_item(catalogs, "rigid_layer", this->layer_name);
    }
    free(this->layer_name);
    this->layer_name = NULL;

    if (this->material == NULL) {
        this->material = catalogs_get_item(catalogs, "rigid_material", this->material_name);
    }
    free(this->material_name);
    this->material_name = NULL;
}

static void rigid_surface_on_destroy(struct component *base) {
    const struct rigid_surface *this = (struct rigid_surface *) base;
    free(this->layer_name);
    free(this->material_name);
}



#define DEFAULT_FRICTION 0.5



double rigid_surface_get_friction(const struct chunks *chunks, const struct rect rect) {
    const struct rigid_surface *highest_surface = NULL;
    uint8_t highest_surface_priority = 0;

    int x_start, x_end, y_start, y_end;
    chunks_get_rect_indexes(chunks, rect, &x_start, &x_end, &y_start, &y_end);
    for (int x = x_start; x <= x_end; x++) {
        for (int y = y_start; y <= y_end; y++) {
            const struct dictionary* surfaces = chunks_chunk_get_components_by_type(chunks, x, y, "rigid_surface");
            if (surfaces == NULL) {
                continue;
            }

            struct dictionary_iterator iterator = dictionary_begin(surfaces);
            struct dictionary_node node;
            while (dictionary_next(surfaces, &iterator, &node)) {
                struct rigid_surface *surface = node.value;
                const struct rect surface_rect = component_get_rect((struct component*)surface);
                if (!rects_intersection(rect, surface_rect)) {
                    continue;
                }
                const uint8_t surface_priority = surface->layer ? rigid_layer_get_priority(surface->layer) : 0;
                if (surface_priority >= highest_surface_priority) {
                    highest_surface = surface;
                    highest_surface_priority = surface_priority;
                }
            }
        }
    }

    if (highest_surface == NULL) {
        return DEFAULT_FRICTION;
    }

    return highest_surface->material ? rigid_material_get_friction(highest_surface->material) : DEFAULT_FRICTION;
}