//
// Created by Nikita on 04.10.2026.
//

#include "fear_ball.h"

#include <math.h>

#include "../../../characters/effects.h"
#include <mcge/mcge.h>
#include "demo/src/characters/damage.h"
#include "demo/src/characters/health.h"

struct fear_ball {
    struct component base;
    double speed;
    double range;
    double travelled;
    struct damage damage;
    double fear_length;
    struct vector2 direction;
    struct entity* self;

    struct vector2 base_scale;

    uint128_t collider_id;
    unsigned int collider_token;

    uint128_t launcher_id;
};

const char* fear_ball_component_key(void) {
    return "fear_ball";
}
static void fear_ball_on_create(struct component *base, struct fields *fields);
static void fear_ball_awake(struct component *base);
static void fear_ball_disable(struct component *base);
static void fear_ball_on_update(struct component *base, const struct update_context *context);

const struct component_vtable fear_ball_vtable = {
    .component_key = fear_ball_component_key,
    .size = sizeof(struct fear_ball),
    .on_create = fear_ball_on_create,
    .on_awake = fear_ball_awake,
    .on_disable = fear_ball_disable,
    .on_update = fear_ball_on_update
};

static void fear_ball_on_create(struct component *base, struct fields *fields) {
    struct fear_ball *this = (struct fear_ball *) base;
    this->speed = fields_get_double(fields, "speed", 0);
    this->range = fields_get_double(fields, "range", 0);
    this->damage.amount = fields_get_double(fields, "damage", 25);
    this->fear_length = fields_get_double(fields, "fear", 0);
}


static void handle_on_enter(void *listener, void *context);

static void fear_ball_awake(struct component *base) {
    struct fear_ball* this = (struct fear_ball*)base;
    this->self = component_get_parent(base);
    struct collider *collider = (struct collider*)entity_get_component(this->self, "collider", entity_query_in_children);
    if (collider == NULL) {
        entity_mark_destroyed(this->self);
        return;
    }
    this->base_scale = entity_get_local_rect(this->self).size;
    this->collider_id = component_get_id((struct component*)collider);
    action_subscribe(collider_on_enter(collider), this, handle_on_enter, &this->collider_token);
}

static void fear_ball_disable(struct component *base) {
    const struct fear_ball* this = (struct fear_ball*)base;
    struct collider *collider = (struct collider*)scene_try_get_component(this->collider_id);
    if (collider) action_unsubscribe(collider_on_enter(collider), this->collider_token);
}

void fear_ball_launch(const struct entity *launcher, struct fear_ball* this, const struct vector2 direction) {
    this->launcher_id = entity_get_id(launcher);
    this->direction = vector_normalize(direction);
}

static struct entity* fear_ball_launcher(const struct fear_ball *this) {
    return scene_try_get_entity(this->launcher_id);
}

static void fear_ball_on_update(struct component *base, const struct update_context *context) {
    struct fear_ball* this = (struct fear_ball*)base;

    const double step = this->speed * context->dt;
    this->travelled += step;

    struct vector2 direction;
    if (this->travelled >= this->range) {
        const struct entity *launcher = fear_ball_launcher(this);
        if (launcher == NULL) {
            direction = vector2_zero;
            entity_mark_destroyed(component_get_parent(base));
        }
        else {
            const struct rect launcher_rect = entity_get_local_rect(launcher);
            direction = vector_normalize(vector_sub(launcher_rect.position, component_get_rect(base).position));
        }
    }
    else {
        direction = this->direction;
    }

    struct rect rect = entity_get_local_rect(this->self);
    rect.position = vector_sum(rect.position, vector_multiply_scalar(direction, step));
    const double distance_coefficient = this->travelled >= this->range ? powl(2 - fminl((this->travelled - this->range) / this->range, 1), 2) : powl(1 + this->travelled / this->range, 2);
    rect.size = vector_multiply_scalar(this->base_scale, distance_coefficient);
    entity_set_local_rect(this->self, rect);
}

static void handle_on_enter(void *listener, void *context) {
    struct fear_ball* this = listener;
    const struct entity* entity = context;
    const struct entity* launcher = fear_ball_launcher(this);
    if (launcher)
    {
        if (launcher == entity) {
            if (this->travelled >= this->range) {
                entity_mark_destroyed(component_get_parent((struct component*)this));
            }
            return;
        }
    }
    struct effects* effects = (struct effects*)entity_try_get_component(entity, "effects", entity_query_local);
    if (effects) {
        effects_set_effect(effects, effect_fear, effects_get_effect(effects, effect_fear) + this->fear_length);
    }
    struct health* health = (struct health*)entity_try_get_component(entity, "health", entity_query_local);
    if (health) {
        health_take_damage(health, this->damage);
    }
}