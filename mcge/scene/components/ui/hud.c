//
// Created by nikita on 13.10.2026.
//

#include "hud.h"
#include "../component_internal.h"
#include "mcge/rendering/renderer.h"
#include "mcge/rendering/renderer_pipeline.h"
#include "mcge/utils/action.h"


struct hud {
    struct component base;

    unsigned int on_viewport_changed_token;
    unsigned int on_output_size_changed_token;
};


const char* hud_component_key(void) {
    return "hud";
}

static void handle_viewport_changed(void *listener, void *context);
static void handle_output_size_changed(void *listener, void *context);

static void hud_awake(struct component* base) {
    struct hud *this = (struct hud*)base;
    struct renderer_pipeline* pipeline = renderer_get_pipeline();
    action_subscribe(renderer_pipeline_get_on_viewport_changed(pipeline), this, handle_viewport_changed, &this->on_viewport_changed_token);
    action_subscribe(renderer_pipeline_get_on_output_size_changed(pipeline), this, handle_output_size_changed, &this->on_output_size_changed_token);
}
static void hud_on_disable(struct component* base) {
    struct hud *this = (struct hud*)base;
    struct renderer_pipeline* pipeline = renderer_get_pipeline();
    action_unsubscribe(renderer_pipeline_get_on_viewport_changed(pipeline), this->on_viewport_changed_token);
    action_unsubscribe(renderer_pipeline_get_on_output_size_changed(pipeline), this->on_output_size_changed_token);
}

const struct component_vtable hud_vtable = {
    .component_key = hud_component_key,
    .size = sizeof(struct hud),
    .on_awake = hud_awake,
    .on_disable = hud_on_disable
};


static void handle_viewport_changed(void *listener, void *context) {
    struct hud *this = listener;
    struct rect *viewport = context;
}
static void handle_output_size_changed(void *listener, void *context) {
    struct hud *this = listener;
    struct rect *viewport = context;
}