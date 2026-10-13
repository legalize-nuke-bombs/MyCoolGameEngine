//
// Created by Nikita on 13.10.2026.
//

#include "health_hud_bar.h"
#include <mcge/mcge.h>


struct health_hud_bar {
    struct component base;

    struct renderer_layer *layer1;
    struct renderer_layer *layer2;

    double scale;
    struct renderer_square *bar1;
    struct renderer_square *bar2;

    uint128_t entity_id;
    uint128_t health_id;
    unsigned int health_on_changed_token;
};


const char* health_hud_bar_component_key(void) {
    return "health_hud_bar";
}


static void health_hud_bar_on_create(struct component* base, struct fields *fields) {

}
static void health_hud_bar_on_destroy(struct component* base) {

}

static void health_hud_bar_awake(struct component* base) {

}
static void health_hud_bar_on_disable(struct component* base) {

}

static void health_hud_bar_on_visible_chunk_update(struct component* base, const struct update_context *context) {

}


const struct component_vtable health_hud_bar_vtable = {
    .component_key = health_hud_bar_component_key,
    .size = sizeof(struct health_hud_bar),
    .on_create = health_hud_bar_on_create,
    .on_destroy = health_hud_bar_on_destroy,
    .on_awake = health_hud_bar_awake,
    .on_disable = health_hud_bar_on_disable,
    .on_visible_chunk_update = health_hud_bar_on_visible_chunk_update
};