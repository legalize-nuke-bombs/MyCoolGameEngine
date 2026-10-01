//
// Created by nikita on 25.09.2026.
//

#include "idle.h"

#include <stdlib.h>
#include "../component_internal.h"

struct idle {
    struct component base;
};

static struct component* idle_clone(struct component base, const struct component *component);

static const struct component_vtable idle_vtable = {
    .component_key = idle_component_key,
    .on_clone = idle_clone,
    .on_awake = NULL,
    .on_update = NULL,
    .on_disable = NULL
};

const char* idle_component_key(void) {
    return "idle";
}

struct component* idle_create(struct parser *parser, struct entity *parent) {
    struct idle *this = malloc(sizeof(struct idle));
    struct component *base = (struct component *) this;
    component_base_create(base, &idle_vtable, parent);
    return base;
}

static struct component* idle_clone(struct component base, const struct component *component) {
    const struct idle *idle = (struct idle *) component;

    struct idle* this = calloc(1, sizeof(struct idle));
    this->base = base;
    return (struct component*)this;
}