#include "game_closer.h"

#include <stdlib.h>
#include <string.h>

#include "scene/components/component_internal.h"
#include "scene/entity.h"
#include "devices/keyboard.h"
#include "scene/scene.h"
#include "engine/lifecycle/engine_lifecycle.h"
#include "utils/fields.h"
#include "utils/action.h"


struct game_closer {
    struct component base;

    char* hotkey;

    struct action* on_hotkey_pressed;
    unsigned int on_hotkey_pressed_token;
};

static void game_closer_on_create(struct component *base, struct fields *fields);
static void game_closer_awake(struct component *base);
static void game_closer_on_disable(struct component *base);
static void game_closer_on_destroy(struct component *base);

const struct component_vtable game_closer_vtable = {
    .component_key = game_closer_component_key,
    .size = sizeof(struct game_closer),
    .on_create = game_closer_on_create,
    .on_awake = game_closer_awake,
    .on_disable = game_closer_on_disable,
    .on_destroy = game_closer_on_destroy
};

const char* game_closer_component_key(void) {
    return "game_closer";
}

static void game_closer_on_create(struct component *base, struct fields *fields) {
    struct game_closer *this = (struct game_closer *) base;
    this->hotkey = fields_dup_string(fields, "key", "");
}

static void game_closer_execute(void* listener, void *context) {
    engine_lifecycle_mark_stop();
}

static void game_closer_awake(struct component *base) {
    struct game_closer *this = (struct game_closer *) base;

    this->on_hotkey_pressed = keyboard_require_action_on_key_pressed(this->hotkey);
    action_subscribe(this->on_hotkey_pressed, this, game_closer_execute, &this->on_hotkey_pressed_token);
}

static void game_closer_on_disable(struct component *base) {
    const struct game_closer *this = (struct game_closer *)base;
    action_unsubscribe(this->on_hotkey_pressed, this->on_hotkey_pressed_token);
}

static void game_closer_on_destroy(struct component *base) {
    const struct game_closer *this = (struct game_closer *)base;
    free(this->hotkey);
}