//
// Created by nikita on 01.10.2026.
//

#include "rigid_body.h"

#include <stdlib.h>

#include "collider.h"
#include "rigid_surface.h"
#include "../component_internal.h"
#include "../../entity.h"
#include "../../scene.h"
#include "../../../utils/parser.h"
#include "../../../utils/vector2_math.h"


#define FRICTION_DEFAULT 0.5f
#define GRAVITY 9.8f
// 1 - a hit loses no kinetic energy
#define ELASTICITY 1.f


static const struct vector2 axis_x = { .x = 1, .y = 0 };
static const struct vector2 axis_y = { .x = 0, .y = 1 };


struct rigid_body {
    struct component base;
    double m;
    double base_friction_coefficient;
    double rolling_friction_coefficient;

    struct vector2 v;
    struct vector2 impulse_sum;
    struct vector2 impulse_drive_sum;

    const struct chunks* chunks;
    const struct collider* collider;
};

static struct component* rigid_body_clone(struct component base, const struct component *component);
static void rigid_body_awake(struct component *base);
static void rigid_body_simulation_chunk_update(struct component* base, const struct update_context *context);
static void rigid_body_on_disable(struct component *base);

static const struct component_vtable rigid_body_vtable = {
    .component_key = rigid_body_component_key,
    .on_clone = rigid_body_clone,
    .on_awake = rigid_body_awake,
    .on_simulation_chunk_update = rigid_body_simulation_chunk_update,
    .on_disable = rigid_body_on_disable
};

const char* rigid_body_component_key(void) {
    return "rigid_body";
}

struct component* rigid_body_create(struct parser *parser, struct entity *parent) {
    struct rigid_body *this = calloc(1, sizeof(struct rigid_body));
    struct component *base = (struct component *) this;
    component_base_create(base, &rigid_body_vtable, parent);

    parser_next_double(parser, &this->m);
    if (this->m < 0) this->m = 1;
    parser_next_double(parser, &this->base_friction_coefficient);
    if (this->base_friction_coefficient < 0) this->base_friction_coefficient = 1.f;
    parser_next_double(parser, &this->rolling_friction_coefficient);
    if (this->rolling_friction_coefficient < 0) this->rolling_friction_coefficient = 0.1f;

    return base;
}

static struct component* rigid_body_clone(struct component base, const struct component *component) {
    const struct rigid_body *rigid_body = (struct rigid_body *) component;

    struct rigid_body* this = calloc(1, sizeof(struct rigid_body));
    this->base = base;
    this->m = rigid_body->m;
    this->base_friction_coefficient = rigid_body->base_friction_coefficient;
    this->rolling_friction_coefficient = rigid_body->rolling_friction_coefficient;
    return (struct component*)this;
}

static void rigid_body_awake(struct component *base) {
    struct rigid_body *this = (struct rigid_body *) base;
    struct entity *parent = component_get_parent(base);

    this->chunks = scene_get_chunks(entity_get_scene(parent));
    this->collider = (struct collider*)entity_get_component(parent, collider_component_key());
    if (this->collider == NULL) {
        entity_mark_destroyed(parent);
    }
}

static void rigid_body_on_disable(struct component *base) {
    struct rigid_body *this = (struct rigid_body *) base;
    this->chunks = NULL;
    this->collider = NULL;
}

static void rigid_body_apply_impulses(struct rigid_body *this, const double impulse_drive_max_scalar) {
    const double impulse_drive_scalar = vector_mod(this->impulse_drive_sum);
    if (impulse_drive_scalar > impulse_drive_max_scalar) {
        this->impulse_drive_sum = vector_multiply_scalar(this->impulse_drive_sum, impulse_drive_max_scalar / impulse_drive_scalar);
    }

    const struct vector2 impulse = vector_sum(this->impulse_sum, this->impulse_drive_sum);
    this->v = vector_sum(this->v, vector_multiply_scalar(impulse, 1.0 / this->m));

    this->impulse_sum = vector2_zero;
    this->impulse_drive_sum = vector2_zero;
}

static void rigid_body_apply_rolling_friction(struct rigid_body *this, const double impulse_max_scalar) {
    const double impulse_need_scalar = this->m * vector_mod(this->v);

    if (impulse_need_scalar <= impulse_max_scalar) {
        this->v = vector2_zero;
    }
    else {
        this->v = vector_multiply_scalar(this->v, 1.0 - impulse_max_scalar / impulse_need_scalar);
    }
}

// Velocity the body will have once the pushes it has already got are applied
static struct vector2 rigid_body_get_pushed_velocity(const struct rigid_body *this) {
    return vector_sum(this->v, vector_multiply_scalar(this->impulse_sum, 1.0 / this->m));
}

static void rigid_body_hit(struct rigid_body *this, const struct entity *obstacle, const struct vector2 direction) {
    struct rigid_body *other = (struct rigid_body*)entity_try_get_component(obstacle, rigid_body_component_key());

    const struct vector2 other_v = other ? rigid_body_get_pushed_velocity(other) : vector2_zero;
    const double approach_speed = vector_dot(vector_sub(rigid_body_get_pushed_velocity(this), other_v), direction);
    if (approach_speed <= 0) {
        return;
    }

    // An obstacle without rigid_body has infinite mass
    const double reduced_mass = other ? this->m * other->m / (this->m + other->m) : this->m;
    const struct vector2 impulse = vector_multiply_scalar(direction, (1.0 + ELASTICITY) * reduced_mass * approach_speed);

    rigid_body_push(this, vector_multiply_scalar(impulse, -1.0));
    if (other) {
        rigid_body_push(other, impulse);
    }
}

static void rigid_body_move_along(struct rigid_body *this, const struct vector2 axis, const double dt) {
    const double distance = vector_dot(this->v, axis) * dt;
    if (distance == 0) {
        return;
    }
    const struct vector2 d_pos = vector_multiply_scalar(axis, distance);

    struct rect rect = component_get_rect((struct component*)this);
    rect.position = vector_sum(rect.position, d_pos);
    const struct entity *obstacle = collider_try_get_obstacle(this->collider, rect);
    if (obstacle != NULL) {
        rigid_body_hit(this, obstacle, vector_multiply_scalar(axis, distance > 0 ? 1.0 : -1.0));
        return;
    }

    struct entity* parent = component_get_parent((struct component*)this);
    struct rect local_rect = entity_get_local_rect(parent);
    local_rect.position = vector_sum(local_rect.position, d_pos);
    entity_set_local_rect(parent, local_rect);
}

// The body only moves into free space. The axes go one by one, so a body slides along what it hits
static void rigid_body_move(struct rigid_body *this, const double dt) {
    rigid_body_move_along(this, axis_x, dt);
    rigid_body_move_along(this, axis_y, dt);
}

static void rigid_body_simulation_chunk_update(struct component* base, const struct update_context *context) {
    struct rigid_body *this = (struct rigid_body *) base;
    const double dt = context->dt;

    const double friction_impulse_max_scalar = this->m * GRAVITY * this->base_friction_coefficient * rigid_surface_get_friction(this->chunks, component_get_rect(base)) * dt;

    rigid_body_apply_impulses(this, friction_impulse_max_scalar);
    rigid_body_apply_rolling_friction(this, friction_impulse_max_scalar * this->rolling_friction_coefficient);
    rigid_body_move(this, dt);
}

void rigid_body_push(struct rigid_body *this, const struct vector2 impulse) {
    this->impulse_sum = vector_sum(this->impulse_sum, impulse);
}

void rigid_body_drive(struct rigid_body *this, const struct vector2 impulse) {
    this->impulse_drive_sum = vector_sum(this->impulse_drive_sum, impulse);
}

struct vector2 rigid_body_get_velocity(const struct rigid_body *this) {
    return this->v;
}

double rigid_body_get_mass(const struct rigid_body *this) {
    return this->m;
}