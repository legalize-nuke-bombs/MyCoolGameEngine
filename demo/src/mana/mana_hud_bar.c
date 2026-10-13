//
// Created by Nikita on 13.10.2026.
//

#include "mana_hud_bar.h"
#include <mcge/mcge.h>


struct mana_hud_bar {
    struct component base;
    struct renderer_layer* layer1;
    struct renderer_layer* layer2;
    uint128_t entity_id;
    uint128_t mana_id;
};


const char* mana_hud_bar_component_key(void) {
    return "mana_hud_bar";
}

static void mana_hud_bar_on_create(struct component* base, struct fields *fields) {

}

static void mana_hud_bar_awake(struct component* base) {

}

static void mana_hud_bar_on_visible_chunk_update(struct component* base, const struct update_context *context) {

}

const struct component_vtable mana_hud_bar_vtable = {
    .component_key = mana_hud_bar_component_key,
    .size = sizeof(struct mana_hud_bar),
    .on_create = mana_hud_bar_on_create,
    .on_awake = mana_hud_bar_awake,
    .on_visible_chunk_update = mana_hud_bar_on_visible_chunk_update
};