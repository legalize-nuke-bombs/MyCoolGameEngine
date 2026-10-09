//
// Created by Nikita on 08.10.2026.
//

#include "arrow.h"
#include <mcge/mcge.h>


struct arrow {
    struct component base;

    double speed;

    struct action on_hit;

    uint128_t collider_id;
    unsigned int collider_on_enter_token;

    uint128_t target_id;
};


const char* arrow_component_key(void) {
    return "arrow";
}

static void arrow_create(struct component* base, struct fields *fields) {
    struct arrow* this = (struct arrow*)base;
    this->speed = fields_get_double(fields, "speed", 10);
    this->on_hit = action_create();
}
static void arrow_destroy(struct component* base) {
    struct arrow* this = (struct arrow*)base;
    action_destroy(&this->on_hit);
}

static void handle_collider_trigger_enter(void *listener, void *context);

static void arrow_awake(struct component* base) {
    struct arrow* this = (struct arrow*)base;
    struct entity* entity = component_get_parent(base);

    struct collider* collider = (struct collider*)entity_get_component(entity, "collider", entity_query_in_children);
    if (collider == NULL) {
        entity_mark_destroyed(entity);
        return;
    }
    action_subscribe(collider_on_enter(collider), this, handle_collider_trigger_enter, &this->collider_on_enter_token);
    this->collider_id = component_get_id((struct component*)collider);
}
static void arrow_on_disable(struct component* base) {
    struct arrow* this = (struct arrow*)base;

    struct collider* collider = (struct collider*)scene_try_get_component(this->collider_id);
    if (collider) {
        action_unsubscribe(collider_on_enter(collider), this->collider_on_enter_token);
    }
}

static void arrow_update(struct component* base, const struct update_context *context);

const struct component_vtable arrow_vtable = {
    .component_key = arrow_component_key,
    .size = sizeof(struct arrow),
    .on_create = arrow_create,
    .on_destroy = arrow_destroy,
    .on_awake = arrow_awake,
    .on_disable = arrow_on_disable,
    .on_update = arrow_update
};

void arrow_launch(struct arrow *this, struct character *target) {
    this->target_id = component_get_id((struct component*)target);
}


static void arrow_update(struct component* base, const struct update_context *context) {
    const struct arrow *this = (struct arrow*)base;

    const struct component* target = scene_try_get_component(this->target_id);
    if (target == NULL) {
        entity_mark_destroyed(component_get_parent(base));
        return;
    }

    const struct vector2 position = component_get_rect(base).position;
    const struct vector2 target_position = component_get_rect(target).position;

    const struct vector2 direction = vector_normalize(vector_sub(target_position, position));
    const struct vector2 offset = vector_multiply_scalar(direction, this->speed * context->dt);

    struct rect rect = entity_get_local_rect(component_get_parent(base));
    rect.position = vector_sum(rect.position, offset);
    entity_set_local_rect(component_get_parent(base), rect);
}


static void handle_collider_trigger_enter(void *listener, void *context) {
    struct arrow* this = listener;
    struct entity* target = context;

    struct component* character = entity_try_get_component(target, "character", entity_query_local);

    if (character == NULL || uint128_cmp(component_get_id(character), this->target_id) != 0) {
        return;
    }

    action_invoke(&this->on_hit, target);
    entity_mark_destroyed(component_get_parent((struct component*)this));
}

struct action* arrow_on_hit(struct arrow *this) {
    return &this->on_hit;
}