//
// Created by nikita on 13.10.2026.
//

#include "mana_regeneration.h"
#include <mcge/mcge.h>

#include "mana.h"


struct mana_regeneration {
    struct component base;

    double speed;
    uint128_t mana_id;
};


const char* mana_regeneration_component_key(void) {
    return "mana_regeneration";
}


static void mana_regeneration_on_create(struct component *base, struct fields *fields) {
    struct mana_regeneration *this = (struct mana_regeneration *)base;
    this->speed = fields_get_double(fields, "speed", 50);
}


static void mana_regeneration_awake(struct component *base) {
    struct mana_regeneration *this = (struct mana_regeneration *)base;
    struct entity *entity = component_get_parent(base);
    struct component* mana = entity_get_component(entity, "mana", entity_query_local);
    if (mana == NULL) {
        entity_mark_destroyed(entity);
        return;
    }
    this->mana_id = component_get_id(mana);
}


static void mana_regeneration_simulation_chunk_update(struct component *base, const struct update_context *context) {
    struct mana_regeneration *this = (struct mana_regeneration *)base;
    struct mana *mana = (struct mana*)scene_try_get_component(this->mana_id);
    if (mana == NULL) {
        return;
    }
    mana_add(mana, this->speed * context->dt);
}


const struct component_vtable mana_regeneration_vtable = {
    .component_key = mana_regeneration_component_key,
    .size = sizeof(struct mana_regeneration),
    .on_create = mana_regeneration_on_create,
    .on_awake = mana_regeneration_awake,
    .on_simulation_chunk_update = mana_regeneration_simulation_chunk_update
};