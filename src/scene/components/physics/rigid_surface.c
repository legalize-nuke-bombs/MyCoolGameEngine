//
// Created by nikita on 01.10.2026.
//

#include "rigid_surface.h"
#include "../component_internal.h"


struct rigid_surface {
    struct component base;
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
    struct rigid_surface *this = malloc(sizeof(struct rigid_surface));
    struct component *base = (struct component *) this;
    component_base_create(base, &rigid_surface_vtable, parent);
    return base;
}

static struct component* rigid_surface_clone(struct component base, const struct component *component) {
    const struct rigid_surface *rigid_surface = (struct rigid_surface *) component;

    struct rigid_surface* this = calloc(1, sizeof(struct rigid_surface));
    this->base = base;
    return (struct component*)this;
}