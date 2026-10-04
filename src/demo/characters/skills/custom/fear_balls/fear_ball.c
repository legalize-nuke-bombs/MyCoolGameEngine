//
// Created by Nikita on 04.10.2026.
//

#include "fear_ball.h"

#include <stdlib.h>

#include "../../../../../scene/components/component_internal.h"

struct fear_ball {
    struct component base;
};

const char* fear_ball_component_key(void) {
    return "fear_ball";
}
static struct component* fear_ball_clone(struct component base, const struct component* component);
static void fear_ball_awake(struct component *base);
static void fear_ball_on_simulation_chunk_update(struct component *base, const struct update_context *context);

static const struct component_vtable fear_ball_vtable = {
    .component_key = fear_ball_component_key,
    .on_clone = fear_ball_clone,
    .on_awake = fear_ball_awake,
    .on_update = fear_ball_on_simulation_chunk_update
};

struct component* fear_ball_create(struct parser *parser, struct entity *parent) {
    struct fear_ball *this = calloc(1, sizeof(struct fear_ball));
    struct component* base = (struct component*)this;
    component_base_create(base, &fear_ball_vtable, parent);
    return base;
}


static struct component* fear_ball_clone(struct component base, const struct component* component) {
    struct fear_ball* fear_ball = (struct fear_ball*)component;
    struct fear_ball *this = calloc(1, sizeof(struct fear_ball));
    this->base = base;
    return (struct component*)this;
}

static void fear_ball_awake(struct component *base) {
    struct fear_ball* this = (struct fear_ball*)base;
    struct entity* self = component_get_parent(base);
}

static void fear_ball_on_simulation_chunk_update(struct component *base, const struct update_context *context) {

}