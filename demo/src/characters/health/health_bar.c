//
// Created by Nikita on 07.10.2026.
//

#include "health_bar.h"
#include <mcge/mcge.h>

#include "health.h"


struct health_bar {
    struct component base;

    struct renderer_layer *l;

    double visibility_timer;
    double visibility_interval;
    struct renderer_square *bar;

    uint128_t health_id;
    unsigned int health_on_changed_subscription_token;
};


const char* health_bar_component_key(void) {
    return "health_bar";
}

static void health_bar_on_create(struct component *base, struct fields *fields) {
    struct health_bar *this = (struct health_bar*)base;
    this->l = renderer_layers_try_get(fields_get_string(fields, "layer", "default"));
    this->visibility_interval = fields_get_double(fields, "visibility_interval", 1);
    this->bar = (struct renderer_square*)renderer_square_create_from_color(color_white);
}

static void health_bar_on_destroy(struct component *base) {
    struct health_bar *this = (struct health_bar*)base;
    renderer_primitive_destroy((struct renderer_primitive*)this->bar);
}

static void handle_health_changed(void *listener, void *context) {
    struct health_bar* this = listener;
    struct health* health = (struct health*)scene_try_get_component(this->health_id);
    if (health == NULL) {
        return;
    }
    const double scale = health_scale(health);
    const struct color target_color = {
        .r = (uint8_t)(255 * (1 - scale)),
        .g = (uint8_t)(255 * scale),
        .b = 0,
        .a = 255
    };
    renderer_square_set_color(this->bar, target_color);
    this->visibility_timer = 0;
}

static void health_bar_awake(struct component *base) {
    struct health_bar* this = (struct health_bar*)base;
    struct entity* entity = component_get_parent(base);
    struct health* health = (struct health*)entity_get_component(entity, "health", entity_query_in_parent);
    if (health == NULL) {
        entity_mark_destroyed(entity);
        return;
    }
    this->health_id = component_get_id((struct component*)health);
    action_subscribe(health_get_on_changed(health), this, handle_health_changed, &this->health_on_changed_subscription_token);
    handle_health_changed(this, NULL);
    this->visibility_timer = this->visibility_interval;
}

static void health_bar_on_disable(struct component *base) {
    const struct health_bar* this = (struct health_bar*)base;
    struct health* health = (struct health*)scene_try_get_component(this->health_id);
    if (health) {
        action_unsubscribe(health_get_on_changed(health), this->health_on_changed_subscription_token);
    }
}

static void health_bar_visible_chunk_update(struct component*base, const struct update_context *context) {
    struct health_bar* this = (struct health_bar*)base;

    this->visibility_timer += context->dt;
    if (this->visibility_timer >= this->visibility_interval) {
        return;
    }

    struct renderer_pipeline *pipeline = renderer_get_pipeline();

    struct renderer_pipeline_draw_call draw_call = {
        .layer = this->l,
        .primitive = (struct renderer_primitive*)this->bar,
        .rect = component_get_rect(base)
    };
    renderer_pipeline_draw_primitive(pipeline, draw_call);
}


const struct component_vtable health_bar_vtable = {
    .component_key = health_bar_component_key,
    .size = sizeof(struct health_bar),
    .on_create = health_bar_on_create,
    .on_destroy = health_bar_on_destroy,
    .on_awake = health_bar_awake,
    .on_disable = health_bar_on_disable,
    .on_visible_chunk_update = health_bar_visible_chunk_update
};