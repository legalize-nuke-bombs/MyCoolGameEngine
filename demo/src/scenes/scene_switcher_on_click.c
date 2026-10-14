//
// Created by Nikita on 14.10.2026.
//


#include "scene_switcher_on_click.h"
#include <mcge/mcge.h>


struct scene_switcher_on_click {
    struct component base;

    char *script;

    uint128_t clickable_id;
    unsigned int on_mouse_button_pressed_token;
};


const char* scene_switcher_on_click_component_key(void) {
    return "scene_switcher_on_click";
}



static void scene_switcher_on_click_on_create(struct component* base, struct fields *fields) {
    struct scene_switcher_on_click* this = (struct scene_switcher_on_click*)base;
    this->script = fields_dup_string(fields, "script", "<null>");
}
static void scene_switcher_on_click_on_destroy(struct component* base) {
    const struct scene_switcher_on_click* this = (struct scene_switcher_on_click*)base;
    free(this->script);
}




static void handle_mouse_button_pressed(void *listener, void *context);

static void scene_switcher_on_click_awake(struct component* base) {
    struct scene_switcher_on_click *this = (struct scene_switcher_on_click*)base;
    struct entity* entity = component_get_parent(base);

    struct clickable* clickable = (struct clickable*)entity_get_component(entity, "clickable", entity_query_local);
    if (clickable == NULL) {
        entity_mark_destroyed(entity);
        return;
    }
    this->clickable_id = component_get_id((struct component*)clickable);
    action_subscribe(clickable_get_on_pressed(clickable), this, handle_mouse_button_pressed, &this->on_mouse_button_pressed_token);
}
static void scene_switcher_on_click_on_disable(struct component* base) {
    const struct scene_switcher_on_click* this = (struct scene_switcher_on_click*)base;
    struct clickable* clickable = (struct clickable*)scene_try_get_component(this->clickable_id);
    if (clickable) {
        action_unsubscribe(clickable_get_on_pressed(clickable), this->on_mouse_button_pressed_token);
    }
}


const struct component_vtable scene_switcher_on_click_vtable = {
    .component_key = scene_switcher_on_click_component_key,
    .size = sizeof(struct scene_switcher_on_click),
    .on_create = scene_switcher_on_click_on_create,
    .on_destroy = scene_switcher_on_click_on_destroy,
    .on_awake = scene_switcher_on_click_awake,
    .on_disable = scene_switcher_on_click_on_disable
};



static void handle_mouse_button_pressed(void *listener, void *context) {
    struct scene_switcher_on_click *this = listener;
    struct mouse_on_button_callback_context *cnt = context;
    if (cnt->button == MOUSE_BUTTON_LEFT) {
        scene_mark_switch(this->script);
    }
}