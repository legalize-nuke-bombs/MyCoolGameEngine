//
// Created by nikita on 01.10.2026.
//

#include "rigid_surface.h"
#include "../component_internal.h"
#include "../../entity.h"
#include "../../scene.h"
#include "../../../utils/parser.h"
#include "../../physics/physics.h"
#include "../../physics/rigid_layers.h"
#include "../../physics/rigid_materials.h"


struct rigid_surface {
    struct component base;

    struct rigid_layer* layer;
    struct rigid_material* material;
};

static struct component* rigid_surface_clone(struct component base, const struct component *component);
static bool rigid_surface_is_chunkable() {
    return true;
}

static const struct component_vtable rigid_surface_vtable = {
    .component_key = rigid_surface_component_key,
    .on_clone = rigid_surface_clone,
    .is_chunkable = rigid_surface_is_chunkable
};

const char* rigid_surface_component_key(void) {
    return "rigid_surface";
}

struct component* rigid_surface_create(struct parser *parser, struct entity *parent) {
    struct rigid_surface *this = calloc(1, sizeof(struct rigid_surface));
    struct component *base = (struct component *) this;
    component_base_create(base, &rigid_surface_vtable, parent);

    const struct physics* physics = scene_get_physics(entity_get_scene(component_get_parent(base)));
    const struct rigid_layers* layers = physics_get_layers(physics);
    const struct rigid_materials* materials = physics_get_materials(physics);

    const char* layer_name = parser_next(parser);
    this->layer = rigid_layers_get(layers, layer_name);

    const char* material_name = parser_next(parser);
    this->material = rigid_materials_get(materials, material_name);

    return base;
}

static struct component* rigid_surface_clone(struct component base, const struct component *component) {
    const struct rigid_surface *rigid_surface = (struct rigid_surface *) component;

    struct rigid_surface* this = calloc(1, sizeof(struct rigid_surface));
    this->base = base;
    this->layer = rigid_surface->layer;
    this->material = rigid_surface->material;
    return (struct component*)this;
}