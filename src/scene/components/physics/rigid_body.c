//
// Created by nikita on 01.10.2026.
//

#include "rigid_body.h"
#include "../component_internal.h"
#include "../../entity.h"
#include "../../../logging/logger.h"
#include "../../../utils/parser.h"
#include "../../../utils/vector2_math.h"


#define MASS_INF 1e+9
#define FRICTION_DEFAULT 0.5
#define GRAVITY 10000


struct rigid_body {
    struct component base;
    double m;
    struct vector2 v;
};

static struct component* rigid_body_clone(struct component base, const struct component *component);
static void rigid_body_simulation_chunk_update(struct component* base, const struct update_context *context);

static const struct component_vtable rigid_body_vtable = {
    .component_key = rigid_body_component_key,
    .on_clone = rigid_body_clone,
    .on_simulation_chunk_update = rigid_body_simulation_chunk_update
};

const char* rigid_body_component_key(void) {
    return "rigid_body";
}

struct component* rigid_body_create(struct parser *parser, struct entity *parent) {
    struct rigid_body *this = calloc(1, sizeof(struct rigid_body));
    struct component *base = (struct component *) this;
    component_base_create(base, &rigid_body_vtable, parent);

    parser_next_double(parser, &this->m);
    if (this->m <= 0) this->m = MASS_INF;

    return base;
}

static struct component* rigid_body_clone(struct component base, const struct component *component) {
    const struct rigid_body *rigid_body = (struct rigid_body *) component;

    struct rigid_body* this = calloc(1, sizeof(struct rigid_body));
    this->base = base;
    this->m = rigid_body->m;
    return (struct component*)this;
}

static void rigid_body_simulation_chunk_update(struct component* base, const struct update_context *context) {
    struct rigid_body *this = (struct rigid_body *) base;

    const double dt = context->dt;

    rigid_body_push_off(this, vector2_1000, dt);

    const struct vector2 d_pos = vector_multiply_scalar(this->v, dt);

    struct rect current_rect = component_get_rect(base);
    current_rect.position = vector_sum(current_rect.position, d_pos);
    entity_set_local_rect(component_get_parent(base), current_rect);
}

void rigid_body_push_off(struct rigid_body *this, const struct vector2 push_force, const double dt) {
    const double n = this->m * GRAVITY;
    const double max_friction = n * FRICTION_DEFAULT;

    const double push_sqr_len = vector_get_sqr_length(push_force);
    const double friction_sqr_len = max_friction * max_friction;

    struct vector2 result_f;

    if (friction_sqr_len >= push_sqr_len) {
        result_f = push_force;
    }
    else {
        const struct vector2 direction = vector_normalize(push_force);
        result_f = vector_multiply_scalar(direction, max_friction);
    }

    const struct vector2 result_v = vector_multiply_scalar(result_f, dt / this->m);
    this->v = vector_sum(this->v, result_v);
}