//
// Created by nikita on 01.10.2026.
//

#include "rigid_surface.h"

#include <stdlib.h>
#include "../component_internal.h"
#include "../../entity.h"
#include "../../../utils/fields.h"
#include "../../chunks/chunks_algorithms.h"
#include "../../../modules/physics/rigid_layer.h"
#include "../../../modules/physics/rigid_layers.h"
#include "../../../modules/physics/rigid_material.h"
#include "../../../modules/physics/rigid_materials.h"


struct rigid_surface {
    struct component base;

    char* layer_name;
    struct rigid_layer* layer;

    char* material_name;
    struct rigid_material* material;
};

static void rigid_surface_on_create(struct component *base, struct fields *fields);
static void rigid_surface_awake(struct component *base);
static void rigid_surface_on_destroy(struct component *base);
static bool rigid_surface_is_chunkable() {
    return true;
}

const struct component_vtable rigid_surface_vtable = {
    .component_key = rigid_surface_component_key,
    .size = sizeof(struct rigid_surface),
    .on_create = rigid_surface_on_create,
    .on_awake = rigid_surface_awake,
    .is_chunkable = rigid_surface_is_chunkable,
    .on_destroy = rigid_surface_on_destroy
};

const char* rigid_surface_component_key(void) {
    return "rigid_surface";
}

static void rigid_surface_on_create(struct component *base, struct fields *fields) {
    struct rigid_surface *this = (struct rigid_surface *) base;
    this->layer_name = fields_dup_string(fields, "layer", NULL);
    this->material_name = fields_dup_string(fields, "material", NULL);
}

static void rigid_surface_awake(struct component *base) {
    struct rigid_surface *this = (struct rigid_surface *) base;

    if (this->layer == NULL) {
        this->layer = rigid_layers_get(this->layer_name);
    }
    free(this->layer_name);
    this->layer_name = NULL;

    if (this->material == NULL) {
        this->material = rigid_materials_get(this->material_name);
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



struct rigid_surface_get_friction_context {
    const struct rigid_surface *highest_surface;
    uint8_t highest_surface_priority;
};
static void rigid_surface_notice_surface(struct component *component, void *context) {
    const struct rigid_surface *surface = (struct rigid_surface *) component;
    struct rigid_surface_get_friction_context *cont = context;
    const uint8_t surface_priority = surface->layer ? rigid_layer_get_priority(surface->layer) : 0;
    if (surface_priority >= cont->highest_surface_priority) {
        cont->highest_surface = surface;
        cont->highest_surface_priority = surface_priority;
    }
}
double rigid_surface_get_friction(const struct rect rect) {
    struct rigid_surface_get_friction_context context = {};
    chunks_algorithms_for_each_typed(rect, "rigid_surface", rigid_surface_notice_surface, &context);
    if (context.highest_surface == NULL || context.highest_surface->material == NULL) {
        return DEFAULT_FRICTION;
    }
    return rigid_material_get_friction(context.highest_surface->material);
}