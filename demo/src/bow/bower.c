//
// Created by nikita on 09.10.2026.
//

#include "bower.h"
#include <mcge/mcge.h>
#include "bow.h"


struct bower {
    struct component base;

    struct bower_stats stats;

    uint128_t bow_id;
};


const char* bower_component_key(void) {
    return "bower";
}

static void bower_on_stats_update(struct bower* this) {
    struct bow* bow = (struct bow*)scene_try_get_component(this->bow_id);
    if (bow) {
        bow_set_stats(bow, this->stats);
    }
}


static void bower_on_create(struct component* base, struct fields *fields) {
    struct bower *this = (struct bower*)base;
    this->stats.damage.amount = fields_get_double(fields, "damage_amount", 10);
    this->stats.interval = fields_get_double(fields, "interval", 0.25);
    this->stats.speed = fields_get_double(fields, "speed", 0.5);
    this->stats.range = fields_get_double(fields, "range", 10);
}

static void bower_awake(struct component* base) {
    struct bower *this = (struct bower*)base;
    struct entity* entity = component_get_parent(base);

    struct bow* bow = (struct bow*)entity_get_component(entity, "bow", entity_query_local);
    if (bow == NULL) {
        entity_mark_destroyed(entity);
        return;
    }
    this->bow_id = component_get_id((struct component*)bow);
    bower_on_stats_update(this);
}

const struct component_vtable bower_vtable = {
    .component_key = bower_component_key,
    .size = sizeof(struct bower),
    .on_create = bower_on_create,
    .on_awake = bower_awake
};