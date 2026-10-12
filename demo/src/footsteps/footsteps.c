//
// Created by nikita on 12.10.2026.
//

#include "footsteps.h"

#include <math.h>
#include <mcge/mcge.h>


struct footsteps {
    struct component base;

    double footstep_distance;
    double footstep_distance_counter;

    uint128_t rigid_body_id;
    unsigned int rigid_body_on_movement_token;

    struct action on_footstep;
};

const char* footsteps_component_key(void) {
    return "footsteps";
}

static void footsteps_on_create(struct component *base, struct fields *fields) {
    struct footsteps *this = (struct footsteps*)base;
    this->footstep_distance = fields_get_double(fields, "footstep_distance", 0.5);
    this->on_footstep = action_create();
}
static void footsteps_on_destroy(struct component *base) {
    struct footsteps *this = (struct footsteps*)base;
    action_destroy(&this->on_footstep);
}

static void handle_movement(void *listener, void *context);
static void footsteps_awake(struct component *base) {
    struct footsteps *this = (struct footsteps*)base;
    struct entity* entity = component_get_parent(base);

    struct rigid_body *rb = (struct rigid_body*)entity_get_component(entity, "rigid_body", entity_query_local);
    if (rb == NULL) {
        entity_mark_destroyed(entity);
        return;
    }

    this->rigid_body_id = component_get_id((struct component*)rb);
    action_subscribe(rigid_body_get_on_movement(rb), this, handle_movement, &this->rigid_body_on_movement_token);
}
static void footsteps_on_disable(struct component *base) {
    const struct footsteps *this = (struct footsteps*)base;

    struct rigid_body *rb = (struct rigid_body*)scene_try_get_component(this->rigid_body_id);
    if (rb) {
        action_unsubscribe(rigid_body_get_on_movement(rb), this->rigid_body_on_movement_token);
    }
}

const struct component_vtable footsteps_vtable = {
    .component_key = footsteps_component_key,
    .size = sizeof(struct footsteps),
    .on_create = footsteps_on_create,
    .on_destroy = footsteps_on_destroy,
    .on_awake = footsteps_awake,
    .on_disable = footsteps_on_disable
};


static void handle_movement(void *listener, void *context) {
    struct footsteps *this = listener;
    struct rigid_body_on_movement_callback_context *rb_cnt = context;

    const double distance = vector_mod(rb_cnt->step);
    this->footstep_distance_counter += distance;
    if (this->footstep_distance_counter >= this->footstep_distance) {
        struct footsteps_on_footstep_callback_context cnt = {
            .surface = rb_cnt->surface
        };
        logger_debug("Entity %s footstep", component_get_global_parent_name((struct component*)this));
        action_invoke(&this->on_footstep, &cnt);
        this->footstep_distance_counter = 0;
    }
}


struct action* footsteps_get_on_footstep(struct footsteps *this) {
    return &this->on_footstep;
}