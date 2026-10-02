//
// Created by nikita on 02.10.2026.
//

#include "collider.h"

#include <stdlib.h>

#include "../component_internal.h"


struct collider {
    struct component base;
};

const char* collider_component_key(void) {
    return "collider";
}

static struct component* collider_clone(struct component base, const struct component *component);
static bool collider_is_chunkable() {
    return true;
}

static const struct component_vtable collider_vtable = {
    .component_key = collider_component_key,
    .on_clone = collider_clone,
    .is_chunkable = collider_is_chunkable
};

struct component* collider_create(struct parser *parser, struct entity *parent) {
    struct collider *this = calloc(1, sizeof(struct collider));
    struct component* base = (struct component*)this;
    component_base_create(base, &collider_vtable, parent);
    return base;
}

struct component* collider_clone(struct component base, const struct component *component) {
    struct collider *collider = (struct collider*)component;

    struct collider *this = calloc(1, sizeof(struct collider));
    this->base = base;
    return (struct component*)this;
}