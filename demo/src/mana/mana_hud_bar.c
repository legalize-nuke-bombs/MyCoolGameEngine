//
// Created by Nikita on 13.10.2026.
//

#include "mana_hud_bar.h"
#include <mcge/mcge.h>

#include "mana.h"


struct mana_hud_bar {
    struct component base;

    struct renderer_layer* layer1;
    struct renderer_layer* layer2;

    double scale;
    struct renderer_square* bar1;
    struct renderer_square* bar2;

    uint128_t entity_id;
    uint128_t mana_id;
    unsigned int mana_on_changed_token;
};


const char* mana_hud_bar_component_key(void) {
    return "mana_hud_bar";
}

static void mana_hud_bar_on_create(struct component* base, struct fields *fields) {
    struct mana_hud_bar* this = (struct mana_hud_bar*)base;

    this->layer1 = renderer_layers_try_get(fields_get_string(fields, "layer1", "<null>"));
    this->layer2 = renderer_layers_try_get(fields_get_string(fields, "layer2", "<null>"));
    const struct color color1 = fields_get_color(fields, "color1", color_black);
    const struct color color2 = fields_get_color(fields, "color2", color_blue);

    this->bar1 = (struct renderer_square*)renderer_square_create_from_color(color1);
    this->bar2 = (struct renderer_square*)renderer_square_create_from_color(color2);

    const char* entity_id_str = fields_get_string(fields, "entity_id", NULL);
    if (entity_id_str == NULL) {
        logger_warn("Entity %s mana hud bar does not have specified entity id", component_get_global_parent_name(base));
        return;
    }
    if (!uint128_from_string(entity_id_str, &this->entity_id)) {
        logger_warn("Entity %s mana hud bar failed to convert entity id `%s` to uint128_t", component_get_global_parent_name(base), entity_id_str);
    }
}
static void mana_hud_bar_on_destroy(struct component* base) {
    const struct mana_hud_bar* this = (struct mana_hud_bar*)base;
    renderer_primitive_destroy((struct renderer_primitive*)this->bar1);
    renderer_primitive_destroy((struct renderer_primitive*)this->bar2);
}

static void handle_mana_changed(void *listener, void *context);

static void mana_hud_bar_awake(struct component* base) {
    struct mana_hud_bar* this = (struct mana_hud_bar*)base;
    const struct entity *entity = scene_try_get_entity(this->entity_id);
    if (entity == NULL) {
        logger_warn("Entity %s mana hud bar failed to find specified entity", component_get_global_parent_name(base));
        return;
    }
    struct mana* mana = (struct mana*)entity_get_component(entity, "mana", entity_query_local);
    if (mana == NULL) {
        return;
    }
    this->mana_id = component_get_id((struct component*)mana);
    action_subscribe(mana_get_on_changed(mana), this, handle_mana_changed, &this->mana_on_changed_token);
    handle_mana_changed(this, NULL);
}
static void mana_hud_bar_on_disable(struct component* base) {
    struct mana_hud_bar* this = (struct mana_hud_bar*)base;
    struct mana* mana = (struct mana*)scene_try_get_component(this->mana_id);
    if (mana) {
        action_unsubscribe(mana_get_on_changed(mana), this->mana_on_changed_token);
    }
}

static void mana_hud_bar_on_visible_chunk_update(struct component* base, const struct update_context *context) {
    const struct mana_hud_bar *this = (struct mana_hud_bar*)base;

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

    struct renderer_pipeline* pipeline = renderer_get_pipeline();
    renderer_pipeline_draw_primitive(pipeline, dc1);
    renderer_pipeline_draw_primitive(pipeline, dc2);
}

const struct component_vtable mana_hud_bar_vtable = {
    .component_key = mana_hud_bar_component_key,
    .size = sizeof(struct mana_hud_bar),
    .on_create = mana_hud_bar_on_create,
    .on_destroy = mana_hud_bar_on_destroy,
    .on_awake = mana_hud_bar_awake,
    .on_disable = mana_hud_bar_on_disable,
    .on_visible_chunk_update = mana_hud_bar_on_visible_chunk_update
};


static void handle_mana_changed(void *listener, void *context) {
    struct mana_hud_bar* this = listener;

    struct mana* mana = (struct mana*)scene_try_get_component(this->mana_id);
    if (mana == NULL) {
        this->scale = 0;
        return;
    }

    this->scale = mana_scale(mana);
}