//
// Created by Nikita on 08.10.2026.
//

#include "hands_bar.h"
#include <mcge/mcge.h>

#include "hands.h"


struct hands_bar {
    struct component base;

    struct renderer_layer *layer1;
    struct renderer_layer *layer2;

    struct renderer_square *square1;
    struct renderer_square *square2;

    double scale;

    uint128_t hands_id;
    unsigned int hands_on_changed_token;
};


const char* hands_bar_component_key(void) {
    return "hands_bar";
}

static void hands_bar_create(struct component* base, struct fields *fields) {
    struct hands_bar* this = (struct hands_bar*)base;
    this->layer1 = renderer_layers_try_get(fields_get_string(fields, "layer1", "default"));
    this->layer2 = renderer_layers_try_get(fields_get_string(fields, "layer2", "default"));
    this->square1 = (struct renderer_square*)renderer_square_create_from_color(fields_get_color(fields, "color1", color_black));
    this->square2 = (struct renderer_square*)renderer_square_create_from_color(fields_get_color(fields, "color2", color_white));
}

static void hands_bar_destroy(struct component* base) {
    const struct hands_bar* this = (struct hands_bar*)base;
    renderer_primitive_destroy((struct renderer_primitive*)this->square1);
    renderer_primitive_destroy((struct renderer_primitive*)this->square2);
}

static void hands_bar_handle_hands_changed(void *executor, void *context);

static void hands_bar_awake(struct component* base) {
    struct hands_bar* this = (struct hands_bar*)base;

    struct entity* entity = component_get_parent(base);
    struct hands* hands = (struct hands*)entity_get_component(entity, "hands", entity_query_in_parent);
    if (hands == NULL) {
        entity_mark_destroyed(entity);
        return;
    }

    this->hands_id = component_get_id((struct component*)hands);
    action_subscribe(hands_on_changed(hands), this, hands_bar_handle_hands_changed, &this->hands_on_changed_token);
    hands_bar_handle_hands_changed(this, NULL);
}

static void hands_bar_on_disable(struct component* base) {
    struct hands_bar* this = (struct hands_bar*)base;

    struct hands* hands = (struct hands*)scene_try_get_component(this->hands_id);
    if (hands) {
        action_unsubscribe(hands_on_changed(hands), this->hands_on_changed_token);
    }
}

static void hands_bar_visible_chunk_update(struct component* base, const struct update_context *context) {
    struct hands_bar* this = (struct hands_bar*)base;

    if (this->scale == 0) {
        return;
    }

    const struct rect rect = component_get_rect((struct component*)this);
    const struct rect rect_square1 = {
        .position.x = rect.position.x + rect.size.x / 2,
        .position.y = rect.position.y,
        .size = rect.size
    };
    const struct rect rect_square2 = {
        .position.x = rect.position.x + rect.size.x * this->scale / 2,
        .position.y = rect.position.y,
        .size.x = rect.size.x * this->scale,
        .size.y = rect.size.y
    };

    struct renderer_pipeline_draw_call draw_call = {
        .primitive = (struct renderer_primitive*)this->square1,
        .layer = this->layer1,
        .rect = rect_square1
    };
    renderer_pipeline_draw_primitive(renderer_get_pipeline(), draw_call);

    draw_call = (struct renderer_pipeline_draw_call){
        .primitive = (struct renderer_primitive*)this->square2,
        .layer = this->layer2,
        .rect = rect_square2
    };
    renderer_pipeline_draw_primitive(renderer_get_pipeline(), draw_call);
}

const struct component_vtable hands_bar_vtable = {
    .component_key = hands_bar_component_key,
    .size = sizeof(struct hands_bar),
    .on_create = hands_bar_create,
    .on_destroy = hands_bar_destroy,
    .on_awake = hands_bar_awake,
    .on_disable = hands_bar_on_disable,
    .on_visible_chunk_update = hands_bar_visible_chunk_update
};

static void hands_bar_handle_hands_changed(void *executor, void *context) {
    struct hands_bar* this = executor;

    struct hands *hands = (struct hands*)scene_try_get_component(this->hands_id);
    if (hands == NULL) {
        return;
    }

    this->scale = hands_get_current_action_scale(hands);
}