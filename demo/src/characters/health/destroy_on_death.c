//
// Created by Nikita on 07.10.2026.
//

#include "destroy_on_death.h"
#include <mcge/mcge.h>

#include "health.h"

struct destroy_on_death {
    struct component base;
    uint128_t health_id;
    unsigned int health_on_death_token;
};

const char* destroy_on_death_component_key(void) {
    return "destroy_on_death";
}

static void handle_death(void *listener, void *context) {
    struct destroy_on_death* this = listener;
    logger_debug("Entity %s will be destroyed via destroy_on_death", component_get_global_parent_name((struct component*)this));
    entity_mark_destroyed(component_get_parent((struct component*)this));
}

static void destroy_on_death_awake(struct component *base) {
    struct destroy_on_death* this = (struct destroy_on_death*)base;
    struct entity* entity = component_get_parent(base);
    struct health* health = (struct health*)entity_get_component(entity, "health", entity_query_local);
    if (health == NULL) {
        entity_mark_destroyed(entity);
        return;
    }
    this->health_id = component_get_id((struct component*)health);
    action_subscribe(health_get_on_death(health), this, handle_death, &this->health_on_death_token);
}
static void destroy_on_death_on_disable(struct component *base) {
    const struct destroy_on_death* this = (struct destroy_on_death*)base;
    struct health* health = (struct health*)scene_try_get_component(this->health_id);
    if (health) {
        action_unsubscribe(health_get_on_death(health), this->health_on_death_token);
    }
}

const struct component_vtable destroy_on_death_vtable = {
    .component_key = destroy_on_death_component_key,
    .size = sizeof(struct destroy_on_death),
    .on_awake = destroy_on_death_awake,
    .on_disable = destroy_on_death_on_disable
};