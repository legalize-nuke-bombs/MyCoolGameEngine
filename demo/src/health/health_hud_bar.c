//
// Created by Nikita on 13.10.2026.
//

#include "health_hud_bar.h"
#include <mcge/mcge.h>

#include "health.h"
#include <stdio.h>


struct health_hud_bar {
    struct component base;

    struct renderer_layer *layer1;
    struct renderer_layer *layer2;
    struct renderer_layer *layer3;

    double scale;
    struct renderer_square *bar1;
    struct renderer_square *bar2;
    struct renderer_text *text;

    uint128_t entity_id;
    uint128_t health_id;
    unsigned int health_on_changed_token;
};


const char* health_hud_bar_component_key(void) {
    return "health_hud_bar";
}


static void health_hud_bar_on_create(struct component* base, struct fields *fields) {
    struct health_hud_bar* this = (struct health_hud_bar*)base;

    this->layer1 = renderer_layers_try_get(fields_get_string(fields, "layer1", "<null>"));
    this->layer2 = renderer_layers_try_get(fields_get_string(fields, "layer2", "<null>"));
    this->layer3 = renderer_layers_try_get(fields_get_string(fields, "layer3", "<null>"));

    const struct color color1 = fields_get_color(fields, "color1", color_black);
    const struct color color2 = fields_get_color(fields, "color2", color_green);
    const struct color color3 = fields_get_color(fields, "color3", color_white);

    const char* font_name = fields_get_string(fields, "font", NULL);
    struct font *font = fonts_get(font_name);

    this->bar1 = (struct renderer_square*)renderer_square_create_from_color(color1);
    this->bar2 = (struct renderer_square*)renderer_square_create_from_color(color2);
    this->text = (struct renderer_text*)renderer_text_create_from_string("", color3, font);

    const char* entity_id_str = fields_get_string(fields, "entity_id", NULL);
    if (entity_id_str == NULL) {
        logger_warn("Entity %s health hud bar does not have specified entity id", component_get_global_parent_name(base));
        return;
    }
    if (!uint128_from_string(entity_id_str, &this->entity_id)) {
        logger_warn("Entity %s health hud bar failed to convert entity id %s to uint128_t", component_get_global_parent_name(base), entity_id_str);
    }
}
static void health_hud_bar_on_destroy(struct component* base) {
    const struct health_hud_bar* this = (struct health_hud_bar*)base;
    renderer_primitive_destroy((struct renderer_primitive*)this->bar1);
    renderer_primitive_destroy((struct renderer_primitive*)this->bar2);
    renderer_primitive_destroy((struct renderer_primitive*)this->text);
}

static void handle_health_changed(void *listener, void *context);

static void health_hud_bar_awake(struct component* base) {
    struct health_hud_bar* this = (struct health_hud_bar*)base;
    struct entity *entity = scene_try_get_entity(this->entity_id);
    if (entity == NULL) {
        logger_warn("Entity %s health hud bar failed to find specified entity", component_get_global_parent_name(base));
        return;
    }
    struct health *health = (struct health*)entity_get_component(entity, "health", entity_query_local);
    if (health == NULL) {
        return;
    }
    this->health_id = component_get_id((struct component*)health);
    action_subscribe(health_get_on_changed(health), this, handle_health_changed, &this->health_on_changed_token);
    handle_health_changed(this, NULL);
}
static void health_hud_bar_on_disable(struct component* base) {
    struct health_hud_bar* this = (struct health_hud_bar*)base;
    struct health* health = (struct health*)scene_try_get_component(this->health_id);
    if (health) {
        action_unsubscribe(health_get_on_changed(health), this->health_on_changed_token);
    }
}

static void health_hud_bar_on_visible_chunk_update(struct component* base, const struct update_context *context) {
    struct health_hud_bar* this = (struct health_hud_bar*)base;

    const struct rect rect1 = component_get_rect(base);
    const struct rect rect2 = {
        .position.x = rect1.position.x - (1 - this->scale) * rect1.size.x / 2,
        .position.y = rect1.position.y,
        .size.x = rect1.size.x * this->scale,
        .size.y = rect1.size.y
    };

    const struct renderer_pipeline_draw_call dc1 = {
        .primitive = (struct renderer_primitive*)this->bar1,
        .layer = this->layer1,
        .rect = rect1
    };
    const struct renderer_pipeline_draw_call dc2 = {
        .primitive = (struct renderer_primitive*)this->bar2,
        .layer = this->layer2,
        .rect = rect2
    };
    const struct renderer_pipeline_draw_call dc3 = {
        .primitive = (struct renderer_primitive*)this->text,
        .layer = this->layer3,
        .rect = rect1
    };

    struct renderer_pipeline* pipeline = renderer_get_pipeline();
    renderer_pipeline_draw_primitive(pipeline, dc1);
    renderer_pipeline_draw_primitive(pipeline, dc2);
    renderer_pipeline_draw_primitive(pipeline, dc3);
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


static void handle_health_changed(void *listener, void *context) {
    struct health_hud_bar* this = listener;

    const struct health* health = (struct health*)scene_try_get_component(this->health_id);
    if (health == NULL) {
        this->scale = 0;
        return;
    }

    this->scale = health_scale(health);

    char string[32];
    snprintf(string, sizeof(string), "%d / %d", (int)health_amount(health), (int)health_max_amount(health));
    renderer_text_set_string(this->text, string);
}
