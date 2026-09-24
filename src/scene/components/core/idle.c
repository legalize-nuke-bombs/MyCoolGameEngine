//
// Created by nikita on 25.09.2026.
//

#include "idle.h"

#include <stdlib.h>
#include "../component_internal.h"

struct idle {
    struct component base;
};

static const struct component_vtable camera_vtable = {
    .component_key = idle_component_key,
    .on_awake = NULL,
    .on_update = NULL,
    .on_disable = NULL
};

const char* idle_component_key(void) {
    return "idle";
}

struct component* idle_create(struct parser *parser, struct entity *parent) {
    struct camera *this = malloc(sizeof(struct idle));
    struct component *base = (struct component *) this;
    component_init(base, &camera_vtable, parser, parent);
    return base;
}