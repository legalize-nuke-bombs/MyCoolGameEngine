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
#include "../../../logging/logger.h"
#include "../../../modules/physics/rigid_material.h"
#include "../../../utils/fields.h"
#include "../../../utils/vector2_math.h"
#include "../../../utils/action.h"


#define FRICTION_DEFAULT 0.5f
#define GRAVITY 9.8f


static const struct vector2 axis_x = { .x = 1, .y = 0 };
static const struct vector2 axis_y = { .x = 0, .y = 1 };


struct rigid_body {
    struct component base;
    double m;
    double base_friction_coefficient;
    double rolling_friction_coefficient;

    struct vector2 v;
    struct vector2 impulse_sum;
    struct vector2 target_velocity;
    bool driven;

    struct action on_drive;

    uint128_t collider_id;
};

static void rigid_body_on_create(struct component *base, struct fields *fields);
static void rigid_body_awake(struct component *base);
static void rigid_body_simulation_chunk_update(struct component* base, const struct update_context *context);
static void rigid_body_on_disable(struct component *base);
static void rigid_body_on_destroy(struct component *base);

const struct component_vtable rigid_body_vtable = {
    .component_key = rigid_body_component_key,
    .size = sizeof(struct rigid_body),
    .on_create = rigid_body_on_create,
    .on_awake = rigid_body_awake,
    .on_simulation_chunk_update = rigid_body_simulation_chunk_update,
    .on_disable = rigid_body_on_disable,
    .on_destroy = rigid_body_on_destroy
};

const char* rigid_body_component_key(void) {
    return "rigid_body";
}

static void rigid_body_on_create(struct component *base, struct fields *fields) {
    struct rigid_body *this = (struct rigid_body *) base;

    this->m = fields_get_double(fields, "mass", 1);
    if (this->m <= 0) {
        logger_warn("Rigid body expected positive mass, got %f, 1 will be used instead", this->m);
        this->m = 1;
    }
    this->base_friction_coefficient = fields_get_double(fields, "friction", 1);
    this->rolling_friction_coefficient = fields_get_double(fields, "rolling", 0.1);

    this->on_drive = action_create();
}

static void rigid_body_on_destroy(struct component *base) {
    struct rigid_body *this = (struct rigid_body *) base;
    action_destroy(&this->on_drive);
}

static void rigid_body_awake(struct component *base) {
    struct rigid_body *this = (struct rigid_body *) base;
    struct entity *parent = component_get_parent(base);

    const struct component *collider = entity_get_component(parent, collider_component_key(), entity_query_in_children);
    if (collider == NULL) {
        entity_mark_destroyed(parent);
        return;
    }
    this->collider_id = component_get_id(collider);
}

static void rigid_body_on_disable(struct component *base) {
    struct rigid_body *this = (struct rigid_body *) base;
}

static const struct collider* rigid_body_collider(const struct rigid_body *this) {
    return (const struct collider*)scene_try_get_component(this->collider_id);
}

static void rigid_body_apply_impulses(struct rigid_body *this, const double impulse_drive_max_scalar) {
    struct vector2 impulse = this->impulse_sum;
    if (this->driven) {
        struct vector2 impulse_drive = vector_multiply_scalar(vector_sub(this->target_velocity, this->v), this->m);
        const double impulse_drive_scalar = vector_mod(impulse_drive);
        if (impulse_drive_scalar > impulse_drive_max_scalar) {
            impulse_drive = vector_multiply_scalar(impulse_drive, impulse_drive_max_scalar / impulse_drive_scalar);
        }
        impulse = vector_sum(impulse, impulse_drive);
    }
    this->v = vector_sum(this->v, vector_multiply_scalar(impulse, 1.0 / this->m));

    this->impulse_sum = vector2_zero;
    this->driven = false;
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

static struct vector2 rigid_body_get_pushed_velocity(const struct rigid_body *this) {
    return vector_sum(this->v, vector_multiply_scalar(this->impulse_sum, 1.0 / this->m));
}

static void rigid_body_hit(struct rigid_body *this, const struct collider *collider, const struct entity *obstacle, const struct vector2 direction) {
    struct rigid_body *other = (struct rigid_body*)entity_try_get_component(obstacle, rigid_body_component_key(), entity_query_in_children);

    const struct collider *other_collider = (struct collider*)entity_get_component(obstacle, "collider", entity_query_in_children);
    const double elasticity = rigid_material_get_restitution(collider_get_rigid_material(collider)) * rigid_material_get_restitution(collider_get_rigid_material(other_collider));

    const struct vector2 other_v = other ? rigid_body_get_pushed_velocity(other) : vector2_zero;
    const double approach_speed = vector_dot(vector_sub(rigid_body_get_pushed_velocity(this), other_v), direction);
    if (approach_speed <= 0) {
        return;
    }

    const double reduced_mass = other ? this->m * other->m / (this->m + other->m) : this->m;
    const struct vector2 impulse = vector_multiply_scalar(direction, (1.0 + elasticity) * reduced_mass * approach_speed);

    rigid_body_push(this, vector_multiply_scalar(impulse, -1.0));
    if (other) {
        rigid_body_push(other, impulse);
    }
}

static void rigid_body_move_along(struct rigid_body *this, const struct collider *collider, const struct vector2 axis, const double dt) {
    const double distance = vector_dot(this->v, axis) * dt;
    if (distance == 0) {
        return;
    }
    const struct vector2 d_pos = vector_multiply_scalar(axis, distance);

    struct rect rect = component_get_rect((const struct component*)collider);
    rect.position = vector_sum(rect.position, d_pos);
    const struct entity *obstacle = collider_try_get_obstacle(collider, rect);
    if (obstacle != NULL) {
        rigid_body_hit(this, collider, obstacle, vector_multiply_scalar(axis, distance > 0 ? 1.0 : -1.0));
        return;
    }

    struct entity* parent = component_get_parent((struct component*)this);
    struct rect local_rect = entity_get_local_rect(parent);
    local_rect.position = vector_sum(local_rect.position, d_pos);
    entity_set_local_rect(parent, local_rect);
}

static void rigid_body_move(struct rigid_body *this, const struct collider *collider, const double dt) {
    rigid_body_move_along(this, collider, axis_x, dt);
    rigid_body_move_along(this, collider, axis_y, dt);
}

static void rigid_body_simulation_chunk_update(struct component* base, const struct update_context *context) {
    struct rigid_body *this = (struct rigid_body *) base;
    const double dt = context->dt;

    const double friction_impulse_max_scalar = this->m * GRAVITY * this->base_friction_coefficient * rigid_surface_get_friction(component_get_rect(base)) * dt;

    rigid_body_apply_impulses(this, friction_impulse_max_scalar);
    rigid_body_apply_rolling_friction(this, friction_impulse_max_scalar * this->rolling_friction_coefficient);

    const struct collider *collider = rigid_body_collider(this);
    if (collider != NULL) {
        rigid_body_move(this, collider, dt);
    }
}

void rigid_body_push(struct rigid_body *this, const struct vector2 impulse) {
    this->impulse_sum = vector_sum(this->impulse_sum, impulse);
}

void rigid_body_drive(struct rigid_body *this, const struct vector2 target_velocity) {
    this->target_velocity = target_velocity;
    this->driven = true;
    action_invoke(&this->on_drive, (void*)&target_velocity);
}

struct vector2 rigid_body_get_velocity(const struct rigid_body *this) {
    return this->v;
}
double rigid_body_get_mass(const struct rigid_body *this) {
    return this->m;
}

MCGE_API struct action* rigid_body_get_on_drive(struct rigid_body *this) {
    return &this->on_drive;
}