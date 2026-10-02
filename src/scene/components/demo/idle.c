//
// Created by nikita on 25.09.2026.
//

#include "idle.h"

#include <stdlib.h>
#include "../component_internal.h"
#include "../../scene.h"
#include "../physics/rigid_body.h"

struct idle {
    struct component base;
    double timer;
};

static struct component* idle_clone(struct component base, const struct component *component);
static void idle_simulation_chunk_update(struct component *base, const struct update_context *context); // TODO remove. test behaviour

static const struct component_vtable idle_vtable = {
    .component_key = idle_component_key,
    .on_clone = idle_clone,
    .on_simulation_chunk_update = idle_simulation_chunk_update
};

const char* idle_component_key(void) {
    return "idle";
}

struct component* idle_create(struct parser *parser, struct entity *parent) {
    struct idle *this = calloc(1, sizeof(struct idle));
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

static void idle_simulation_chunk_update(struct component *base, const struct update_context *context) {
    struct idle *this = (struct idle *) base;
    this->timer += context->dt;
    if (this->timer >= 10) {
        rigid_body_explosion(component_get_rect(base).position, 1500, scene_get_chunks(component_get_scene(base)));
        this->timer = 0;
    }
}