//
// Created by nikita on 08.10.2026.
//

#include "arrow_damager.h"
#include <mcge/mcge.h>

#include "arrow.h"
#include "demo/src/health/health.h"


struct arrow_damager {
    struct component base;

    uint128_t arrow_id;
    unsigned int arrow_on_hit_token;
};


const char* arrow_damager_component_key(void) {
    return "arrow_damager";
}


static void arrow_damager_on_create(struct component* base, struct fields *fields) {
    struct arrow_damager *this = (struct arrow_damager*)base;
}


static void handle_arrow_hit(void *listener, void *context);

static void arrow_damager_awake(struct component* base) {
    struct arrow_damager *this = (struct arrow_damager*)base;
    struct entity* entity = component_get_parent(base);

    struct arrow *arrow = (struct arrow*)entity_get_component(entity, "arrow", entity_query_local);
    if (arrow == NULL) {
        entity_mark_destroyed(entity);
        return;
    }
    this->arrow_id = component_get_id((struct component*)arrow);
    action_subscribe(arrow_on_hit(arrow), this, handle_arrow_hit, &this->arrow_on_hit_token);
}
static void arrow_damager_disable(struct component* base) {
    struct arrow_damager *this = (struct arrow_damager*)base;

    struct arrow* arrow = (struct arrow*)scene_try_get_component(this->arrow_id);
    if (arrow) {
        action_unsubscribe(arrow_on_hit(arrow), this->arrow_on_hit_token);
    }
}

const struct component_vtable arrow_damager_vtable = {
    .component_key = arrow_damager_component_key,
    .size = sizeof(struct arrow_damager),
    .on_create = arrow_damager_on_create,
    .on_awake = arrow_damager_awake,
    .on_disable = arrow_damager_disable
};

static void handle_arrow_hit(void *listener, void *context) {
    struct arrow_damager *this = listener;
    struct entity *target = context;

    const struct arrow* arrow = (const struct arrow*)scene_try_get_component(this->arrow_id);
    if (arrow == NULL) {
        return;
    }

    struct health *health = (struct health*)entity_try_get_component(target, "health", entity_query_local);
    if (health == NULL) {
        return;
    }

    const struct damage damage = arrow_stats(arrow).damage;
    health_take_damage(health, damage);
}