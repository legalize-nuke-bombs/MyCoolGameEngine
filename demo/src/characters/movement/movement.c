//
// Created by nikita on 08.10.2026.
//

#include "movement.h"
#include "demo/src/characters/hands/hands.h"


struct movement {
    struct component base;

    double speed;

    uint128_t hands_id;
    uint128_t rigid_body_id;
};


const char* movement_component_key() {
    return "movement";
}

static void movement_on_create(struct component* base, struct fields *fields);
static void movement_awake(struct component* base);

const struct component_vtable movement_vtable = {
    .component_key = movement_component_key,
    .size = sizeof(struct movement),
    .on_create = movement_on_create,
    .on_awake = movement_awake
};

static void movement_on_create(struct component *base, struct fields *fields) {
    struct movement *this = (struct movement *) base;
    this->speed = fields_get_double(fields, "speed", 0);
}

static void movement_awake(struct component* base) {
    struct movement* this = (struct movement*)base;

    const struct component* hands = entity_get_component(component_get_parent(base), "hands", entity_query_local);
    if (hands == NULL) {
        entity_mark_destroyed(component_get_parent(base));
        return;
    }
    this->hands_id = component_get_id(hands);

    const struct component *rigid_body = entity_get_component(component_get_parent(base), "rigid_body", entity_query_local);
    if (rigid_body == NULL) {
        entity_mark_destroyed(component_get_parent(base));
        return;
    }
    this->rigid_body_id = component_get_id(rigid_body);
}

static void movement_execute(void *executor, void *context) {
    struct movement* this = executor;
    struct vector2* direction = context;

    struct rigid_body *rigid_body = (struct rigid_body*)scene_try_get_component(this->rigid_body_id);
    if (rigid_body == NULL) {
        return;
    }

    rigid_body_drive(rigid_body, vector_multiply_scalar(vector_normalize(*direction), this->speed));
}

void movement_try_move(struct movement *this, const struct vector2 direction) {
    struct hands *hands = (struct hands*)scene_try_get_component(this->hands_id);
    if (hands == NULL) {
        return;
    }

    struct hands_action movement_hands_action = {
        .name = "movement",
        .priority = HANDS_ACTION_PRIORITY_MOVEMENT,
        .duration = 0,
        .method.executor = this,
        .method.context = (void*)&direction, // Actions with duration 0 execute instantly or do not execute at all
        .method.func = movement_execute
    };
    hands_try_put(hands, movement_hands_action);
}