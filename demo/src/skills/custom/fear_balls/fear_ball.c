//
// Created by Nikita on 04.10.2026.
//

#include "fear_ball.h"

#include <stdlib.h>

#include "../../../characters/effects.h"
#include <mcge/mcge.h>

struct fear_ball {
    struct component base;
    double speed;
    double range;
    double travelled;
    bool coming_back;
    double fear_length;
    uint128_t launcher_id;
    struct vector2 direction;
    struct entity* self;
};

const char* fear_ball_component_key(void) {
    return "fear_ball";
}
static void fear_ball_on_create(struct component *base, struct fields *fields);
static void fear_ball_awake(struct component *base);
static void fear_ball_on_update(struct component *base, const struct update_context *context);

const struct component_vtable fear_ball_vtable = {
    .component_key = fear_ball_component_key,
    .size = sizeof(struct fear_ball),
    .on_create = fear_ball_on_create,
    .on_awake = fear_ball_awake,
    .on_update = fear_ball_on_update
};

static void fear_ball_on_create(struct component *base, struct fields *fields) {
    struct fear_ball *this = (struct fear_ball *) base;
    this->speed = fields_get_double(fields, "speed", 0);
    this->range = fields_get_double(fields, "range", 0);
    this->fear_length = fields_get_double(fields, "fear", 0);
}


static void handle_on_enter(void *listener, void *context);

static void fear_ball_awake(struct component *base) {
    struct fear_ball* this = (struct fear_ball*)base;
    this->self = component_get_parent(base);
    struct collider *collider = (struct collider*)entity_get_component(this->self, "collider", entity_query_recursive);
    if (collider == NULL) {
        entity_mark_destroyed(this->self);
        return;
    }
    struct action* on_enter = collider_on_enter(collider);
    action_subscribe_no_token(on_enter, this, handle_on_enter);
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

    // The speed is constant: the ball flies out for range metres and then turns back at once
    const double step = this->speed * context->dt;
    this->travelled += step;
    if (this->travelled >= this->range) {
        this->coming_back = true;
    }

    struct vector2 direction;
    if (this->coming_back) {
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
    entity_set_local_rect(this->self, rect);
}

static void handle_on_enter(void *listener, void *context) {
    struct fear_ball* this = listener;
    const struct entity* entity = context;
    const struct entity* launcher = fear_ball_launcher(this);
    if (launcher)
    {
        if (launcher == entity) {
            if (this->coming_back) {
                entity_mark_destroyed(component_get_parent((struct component*)this));
            }
            return;
        }
    }
    struct effects* effects = (struct effects*)entity_try_get_component(entity, "effects", entity_query_local);
    if (effects == NULL) {
        return;
    }
    effects_set_effect(effects, effect_fear, effects_get_effect(effects, effect_fear) + this->fear_length);
}