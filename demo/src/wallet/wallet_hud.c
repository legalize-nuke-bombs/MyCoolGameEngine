//
// Created by nikita on 14.10.2026.
//

#include "wallet_hud.h"
#include <mcge/mcge.h>

#include "wallet.h"
#include <stdio.h>


struct wallet_hud {
    struct component base;

    struct renderer_layer *layer;

    struct renderer_text* text;

    uint128_t entity_id;
    uint128_t wallet_id;
    unsigned int wallet_on_changed_token;
};


const char* wallet_hud_component_key(void) {
    return "wallet_hud";
}


static void wallet_hud_on_create(struct component* base, struct fields *fields) {
    struct wallet_hud *this = (struct wallet_hud*)base;

    const char *layer_name = fields_get_string(fields, "layer", NULL);
    this->layer = renderer_layers_try_get(layer_name);

    const struct color color = fields_get_color(fields, "color", color_white);

    const char* font_name = fields_get_string(fields, "font", NULL);
    struct font* font = fonts_get(font_name);

    this->text = (struct renderer_text*)renderer_text_create_from_string("", color, font);

    const char* entity_id_str = fields_get_string(fields, "entity_id", NULL);
    if (entity_id_str == NULL) {
        logger_warn("Entity %s wallet hud does not have specified entity id", component_get_global_parent_name(base));
    }
    else {
        if (!uint128_from_string(entity_id_str, &this->entity_id)) {
            logger_warn("Entity %s wallet failed to convert entity id %s to uint128_t", component_get_global_parent_name(base), entity_id_str);
        }
    }
}
static void wallet_hud_on_destroy(struct component* base) {
    const struct wallet_hud *this = (struct wallet_hud*)base;
    renderer_primitive_destroy((struct renderer_primitive*)this->text);
}



static void handle_wallet_changed(void *listener, void *context);

static void wallet_hud_awake(struct component* base) {
    struct wallet_hud *this = (struct wallet_hud*)base;

    const struct entity *entity = scene_try_get_entity(this->entity_id);
    if (entity == NULL) {
        logger_warn("Entity %s wallet hud failed to find specified entity", component_get_global_parent_name(base));
        return;
    }
    struct wallet* wallet = (struct wallet*)entity_get_component(entity, "wallet", entity_query_local);
    if (wallet == NULL) {
        return;
    }
    this->wallet_id = component_get_id((struct component*)wallet);
    action_subscribe(wallet_get_on_changed(wallet), this, handle_wallet_changed, &this->wallet_on_changed_token);
    handle_wallet_changed(this, NULL);
}
static void wallet_hud_on_disable(struct component* base) {
    const struct wallet_hud *this = (struct wallet_hud*)base;

    struct wallet* wallet = (struct wallet*)scene_try_get_component(this->wallet_id);
    if (wallet) {
        action_unsubscribe(wallet_get_on_changed(wallet), this->wallet_on_changed_token);
    }
}


static void wallet_hud_on_visible_chunk_update(struct component* base, const struct update_context *context) {
    const struct wallet_hud *this = (struct wallet_hud*)base;

    const struct renderer_pipeline_draw_call dc = {
        .primitive = (struct renderer_primitive*)this->text,
        .layer = this->layer,
        .rect = component_get_rect(base)
    };
    renderer_pipeline_draw_primitive(renderer_get_pipeline(), dc);
}



const struct component_vtable wallet_hud_vtable = {
    .component_key = wallet_hud_component_key,
    .size = sizeof(struct wallet_hud),
    .on_create = wallet_hud_on_create,
    .on_destroy = wallet_hud_on_destroy,
    .on_awake = wallet_hud_awake,
    .on_disable = wallet_hud_on_disable,
    .on_visible_chunk_update = wallet_hud_on_visible_chunk_update
};



static void handle_wallet_changed(void *listener, void *context) {
    const struct wallet_hud *this = listener;

    const struct wallet *wallet = (struct wallet*)scene_try_get_component(this->wallet_id);
    if (wallet == NULL) {
        return;
    }

    char string[16];
    snprintf(string, sizeof(string), "%d", wallet_amount(wallet));
    renderer_text_set_string(this->text, string);
}
