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
#define GRAVITY 320


struct rigid_body {
    struct component base;
    double m;

    struct vector2 v;
    struct vector2 f_sum;
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

    rigid_body_push(this, vector2_10000);

    const double normal_force = this->m * GRAVITY;
    const double max_friction = normal_force * FRICTION_DEFAULT;

    const double force_sqr_len = vector_get_sqr_length(this->f_sum);
    const double friction_sqr_len = max_friction * max_friction;

    struct vector2 f_result = vector2_zero;

    if (friction_sqr_len >= force_sqr_len) {
        f_result = this->f_sum;
    }
    else if (force_sqr_len > 0.0) {
        const struct vector2 direction = vector_normalize(this->f_sum);
        f_result = vector_multiply_scalar(direction, max_friction);
    }

    const struct vector2 a = vector_multiply_scalar(f_result, 1.0 / this->m);

    this->v = vector_sum(this->v, vector_multiply_scalar(a, dt));

    const struct vector2 d_pos = vector_multiply_scalar(this->v, dt);
    struct rect current_rect = component_get_rect(base);
    current_rect.position = vector_sum(current_rect.position, d_pos);
    entity_set_local_rect(component_get_parent(base), current_rect);

    this->f_sum = vector2_zero;
}

void rigid_body_push(struct rigid_body *this, const struct vector2 f) {
    this->f_sum = vector_sum(this->f_sum, f);
}
