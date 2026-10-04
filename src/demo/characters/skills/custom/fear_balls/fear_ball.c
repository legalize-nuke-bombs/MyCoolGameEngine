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
#include "../../../../../utils/parser.h"
#include "../../../../../utils/vector2_math.h"

struct fear_ball {
    struct component base;
    double v;
    double a;
    double lifetime_timer;
    double lifetime;
    double fear_length;
    struct vector2 direction;
    struct entity* self;
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
    parser_next_double(parser, &this->v);
    parser_next_double(parser, &this->a);
    parser_next_double(parser, &this->lifetime);
    parser_next_double(parser, &this->fear_length);
    return base;
}


static struct component* fear_ball_clone(struct component base, const struct component* component) {
    const struct fear_ball* fear_ball = (struct fear_ball*)component;
    struct fear_ball *this = calloc(1, sizeof(struct fear_ball));
    this->base = base;
    this->v = fear_ball->v;
    this->a = fear_ball->a;
    this->lifetime = fear_ball->lifetime;
    this->fear_length = fear_ball->fear_length;
    return (struct component*)this;
}

static void handle_on_trigger_enter(void *listener, void *context);

static void fear_ball_awake(struct component *base) {
    struct fear_ball* this = (struct fear_ball*)base;
    this->self = component_get_parent(base);
    struct collider *collider = (struct collider*)entity_get_component(this->self, "collider", entity_query_recursive);
    if (collider == NULL) {
        entity_mark_destroyed(this->self);
        return;
    }
    struct action* on_trigger_enter = collider_on_trigger_enter(collider);
    action_subscribe_no_token(on_trigger_enter, this, handle_on_trigger_enter);
}

static void fear_ball_on_simulation_chunk_update(struct component *base, const struct update_context *context) {
    struct fear_ball* this = (struct fear_ball*)base;

    this->v += this->a * context->dt;

    struct rect rect = entity_get_local_rect(this->self);
    rect.position = vector_sum(rect.position, vector_multiply_scalar(this->direction, this->v * context->dt));
    entity_set_local_rect(this->self, rect);

    this->lifetime_timer += context->dt;
    if (this->lifetime_timer >= this->lifetime) {
        entity_mark_destroyed(this->self);
    }
}

void fear_ball_set_direction(struct fear_ball* this, struct vector2 direction) {
    this->direction = vector_normalize(direction);
}

static void handle_on_trigger_enter(void *listener, void *context) {
    struct fear_ball* this = listener;
    const struct entity* entity = context;
    struct effects* effects = (struct effects*)entity_try_get_component(entity, "effects", entity_query_local);
    if (effects == NULL) {
        return;
    }
    effects_set_effect(effects, effect_fear, effects_get_effect(effects, effect_fear) + this->fear_length);
}