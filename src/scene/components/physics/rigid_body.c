//
// Created by nikita on 01.10.2026.
//

#include "rigid_body.h"

#include <stdlib.h>

#include "rigid_surface.h"
#include "../component_internal.h"
#include "../../entity.h"
#include "../../scene.h"
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

    const struct chunks* chunks;
};

static struct component* rigid_body_clone(struct component base, const struct component *component);
static void rigid_body_awake(struct component *base);
static void rigid_body_simulation_chunk_update(struct component* base, const struct update_context *context);

static const struct component_vtable rigid_body_vtable = {
    .component_key = rigid_body_component_key,
    .on_clone = rigid_body_clone,
    .on_awake = rigid_body_awake,
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

static void rigid_body_awake(struct component *base) {
    struct rigid_body *this = (struct rigid_body *) base;
    this->chunks = scene_get_chunks(entity_get_scene(component_get_parent(base)));
}

static void rigid_body_apply_force(struct rigid_body *this, const double dt) {
    const struct vector2 a = vector_multiply_scalar(this->f_sum, 1 / this->m);
    this->v = vector_sum(this->v, vector_multiply_scalar(a, dt));
    this->f_sum = vector2_zero;
}

static void rigid_body_apply_friction(struct rigid_body *this, const double dt) {
    const double v_scalar = vector_get_length(this->v);
    if (v_scalar <= 0.0001f) {
        this->v = vector2_zero;
        return;
    }
    const struct vector2 v_direction = vector_multiply_scalar(this->v, 1 / v_scalar);
    const double f_input_scalar = this->m * v_scalar / dt;

    const struct vector2 f_friction_direction = vector_multiply_scalar(v_direction, -1);
    const double f_max_friction_scalar = this->m * GRAVITY * rigid_surface_get_friction(this->chunks, component_get_rect((struct component*)this));
    const double f_friction_scalar = f_input_scalar >= f_max_friction_scalar ? f_max_friction_scalar : f_input_scalar;
    const struct vector2 f_friction = vector_multiply_scalar(f_friction_direction, f_friction_scalar);

    const struct vector2 a = vector_multiply_scalar(f_friction, 1 / this->m);
    this->v = vector_sum(this->v, vector_multiply_scalar(a, dt));
}

static void rigid_body_move(struct rigid_body *this, const double dt) {
    const struct vector2 d_pos = vector_multiply_scalar(this->v, dt);

    struct entity* parent = component_get_parent((struct component*)this);
    struct rect local_rect = entity_get_local_rect(parent);
    local_rect.position = vector_sum(local_rect.position, d_pos);
    entity_set_local_rect(parent, local_rect);
}

static void rigid_body_simulation_chunk_update(struct component* base, const struct update_context *context) {
    struct rigid_body *this = (struct rigid_body *) base;
    const double dt = context->dt;

    rigid_body_apply_force(this, dt);
    rigid_body_apply_friction(this, dt);
    rigid_body_move(this, dt);
}

void rigid_body_push(struct rigid_body *this, const struct vector2 f) {
    this->f_sum = vector_sum(this->f_sum, f);
}
