//
// Created by nikita on 09.10.2026.
//

#include "bowman.h"
#include <mcge/mcge.h>
#include "bow.h"


struct bowman {
    struct component base;

    struct bowman_stats stats;

    uint128_t bow_id;
};


const char* bowman_component_key(void) {
    return "bowman";
}

static void bowman_on_stats_update(struct bowman* this) {
    struct bow* bow = (struct bow*)scene_try_get_component(this->bow_id);
    if (bow) {
        bow_set_stats(bow, this->stats);
    }
}


static void bowman_on_create(struct component* base, struct fields *fields) {
    struct bowman *this = (struct bowman*)base;
    this->stats.damage.amount = fields_get_double(fields, "damage_amount", 10);
    this->stats.interval = fields_get_double(fields, "interval", 0.25);
    this->stats.speed = fields_get_double(fields, "speed", 0.5);
    this->stats.range = fields_get_double(fields, "range", 10);
}

static void bowman_awake(struct component* base) {
    struct bowman *this = (struct bowman*)base;
    struct entity* entity = component_get_parent(base);

    struct bow* bow = (struct bow*)entity_get_component(entity, "bow", entity_query_local);
    if (bow == NULL) {
        entity_mark_destroyed(entity);
        return;
    }
    this->bow_id = component_get_id((struct component*)bow);
    bowman_on_stats_update(this);
}

const struct component_vtable bowman_vtable = {
    .component_key = bowman_component_key,
    .size = sizeof(struct bowman),
    .on_create = bowman_on_create,
    .on_awake = bowman_awake
};