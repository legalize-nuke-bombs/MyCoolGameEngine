#include "scene_switcher.h"

#include <stdlib.h>
#include <string.h>

#include <mcge/scene/components/component_internal.h>
#include <mcge/scene/entity.h>
#include <mcge/devices/keyboard.h>
#include <mcge/scene/scene.h>
#include <mcge/utils/fields.h>
#include <mcge/utils/action.h>


struct scene_switcher {
    struct component base;

    char* hotkey;
    char* map_path;

    struct action* on_hotkey_pressed;
    unsigned int on_hotkey_pressed_token;
};

static void scene_switcher_on_create(struct component *base, struct fields *fields);
static void scene_switcher_awake(struct component *base);
static void scene_switched_on_disable(struct component *base);
static void scene_switcher_on_destroy(struct component *base);

const struct component_vtable scene_switcher_vtable = {
    .component_key = scene_switcher_component_key,
    .size = sizeof(struct scene_switcher),
    .on_create = scene_switcher_on_create,
    .on_awake = scene_switcher_awake,
    .on_disable = scene_switched_on_disable,
    .on_destroy = scene_switcher_on_destroy
};

const char* scene_switcher_component_key(void) {
    return "scene_switcher";
}

static void scene_switcher_on_create(struct component *base, struct fields *fields) {
    struct scene_switcher *this = (struct scene_switcher *) base;
    this->hotkey = fields_dup_string(fields, "key", "");
    this->map_path = fields_dup_string(fields, "script", "");
}

static void scene_switcher_execute(void* listener, void *context) {
    const struct scene_switcher* this = listener;
    scene_mark_switch(this->map_path);
}

static void scene_switcher_awake(struct component *base) {
    struct scene_switcher *this = (struct scene_switcher *) base;

    this->on_hotkey_pressed = keyboard_require_action_on_key_pressed(this->hotkey);
    action_subscribe(this->on_hotkey_pressed, this, scene_switcher_execute, &this->on_hotkey_pressed_token);
}

static void scene_switched_on_disable(struct component *base) {
    const struct scene_switcher *this = (struct scene_switcher *)base;
    action_unsubscribe(this->on_hotkey_pressed, this->on_hotkey_pressed_token);
}

static void scene_switcher_on_destroy(struct component *base) {
    const struct scene_switcher *this = (struct scene_switcher *)base;
    free(this->hotkey);
    free(this->map_path);
}