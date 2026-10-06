//
// Created by Nikita on 04.10.2026.
//

#include "fear_ball.h"

#include <stdlib.h>

#include "../../../effects.h"
#include "../../../../../logging/logger.h"
#include "../../../../../scene/entity.h"
#include "../../../../../scene/components/component_internal.h"
#include "../../../../../scene/components/physics/collider.h"
#include "../../../../../utils/action.h"
#include "../../../../../utils/fields.h"
#include "../../../../../utils/vector2_math.h"

struct fear_ball {
    struct component base;
    double speed;
    double a;
    bool coming_back;
    double fear_length;
    const struct entity *launcher;
    struct vector2 v;
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
    this->a = fields_get_double(fields, "acceleration", 0);
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

static void fear_ball_on_update(struct component *base, const struct update_context *context) {
    struct fear_ball* this = (struct fear_ball*)base;

    if (this->launcher == NULL) { // TODO This safeguard will not work without smart refs and will crash program if launcher was destroyed
        entity_mark_destroyed(this->self);
        return;
    }

    struct rect rect = entity_get_local_rect(this->self);
    const struct vector2 to_launcher = vector_normalize(vector_sub(entity_get_local_rect(this->launcher).position, rect.position));

    // The acceleration always points along the line to the launcher: it slows the ball on its way out and then brings it back
    this->v = vector_sum(this->v, vector_multiply_scalar(to_launcher, -this->a * context->dt));
    this->coming_back = vector_dot(this->v, to_launcher) > 0;

    rect.position = vector_sum(rect.position, vector_multiply_scalar(this->v, context->dt));
    entity_set_local_rect(this->self, rect);
}

void fear_ball_launch(const struct entity *launcher, struct fear_ball* this, const struct vector2 direction) {
    this->launcher = launcher;
    this->v = vector_multiply_scalar(vector_normalize(direction), this->speed);
}

static void handle_on_enter(void *listener, void *context) {
    struct fear_ball* this = listener;
    const struct entity* entity = context;
    if (this->launcher) // TODO See the first note
    {
        if (this->launcher == entity) {
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