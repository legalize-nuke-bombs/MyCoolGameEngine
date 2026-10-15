//
// Created by Nikita on 14.10.2026.
//

#include "game_closer_on_click.h"
#include <mcge/mcge.h>


struct game_closer_on_click {
    struct component base;

    uint128_t clickable_id;
    unsigned int on_mouse_button_pressed_token;
};


const char* game_closer_on_click_component_key(void) {
    return "game_closer_on_click";
}





static void handle_mouse_button_pressed(void *listener, void *context);

static void game_closer_on_click_awake(struct component* base) {
    struct game_closer_on_click *this = (struct game_closer_on_click*)base;
    struct entity* entity = component_get_parent(base);

    struct clickable* clickable = (struct clickable*)entity_get_component(entity, "clickable", entity_query_local);
    if (clickable == NULL) {
        entity_mark_destroyed(entity);
        return;
    }
    this->clickable_id = component_get_id((struct component*)clickable);
    action_subscribe(clickable_get_on_pressed(clickable), this, handle_mouse_button_pressed, &this->on_mouse_button_pressed_token);
}
static void game_closer_on_click_on_disable(struct component* base) {
    const struct game_closer_on_click* this = (struct game_closer_on_click*)base;
    struct clickable* clickable = (struct clickable*)scene_try_get_component(this->clickable_id);
    if (clickable) {
        action_unsubscribe(clickable_get_on_pressed(clickable), this->on_mouse_button_pressed_token);
    }
}


const struct component_vtable game_closer_on_click_vtable = {
    .component_key = game_closer_on_click_component_key,
    .size = sizeof(struct game_closer_on_click),
    .on_awake = game_closer_on_click_awake,
    .on_disable = game_closer_on_click_on_disable
};



static void handle_mouse_button_pressed(void *listener, void *context) {
    struct game_closer_on_click *this = listener;
    struct mouse_on_button_callback_context *cnt = context;
    if (cnt->button == MOUSE_BUTTON_LEFT) {
        engine_lifecycle_mark_stop();
    }
}
