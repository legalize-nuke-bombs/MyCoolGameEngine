#include "game_closer.h"

#include <string.h>

#include "../component_internal.h"
#include "../../entity.h"
#include "../../../devices/keyboard.h"
#include "../../scene.h"
#include "../../../engine/lifecycle/engine_lifecycle.h"
#include "../../../subsystems/subsystem_collection.h"
#include "../../../utils/parser.h"


struct game_closer {
    struct component base;

    char* hotkey;

    struct action* on_hotkey_pressed;
    unsigned int on_hotkey_pressed_token;
};

static struct component* game_closer_clone(struct component base, const struct component *component);
static void game_closer_awake(struct component *base);
static void game_closer_on_disable(struct component *base);
static void game_closer_on_destroy(struct component *base);

static const struct component_vtable game_closer_vtable = {
    .component_key = game_closer_component_key,
    .on_clone = game_closer_clone,
    .on_awake = game_closer_awake,
    .on_disable = game_closer_on_disable,
    .on_destroy = game_closer_on_destroy
};

const char* game_closer_component_key(void) {
    return "game_closer";
}

struct component* game_closer_create(struct parser *parser, struct entity *parent) {
    struct game_closer *this = calloc(1, sizeof(struct game_closer));
    struct component *base = (struct component *) this;
    component_base_create(base, &game_closer_vtable, parent);

    this->hotkey = parser_next_dup(parser);

    return base;
}

static struct component* game_closer_clone(struct component base, const struct component *component) {
    const struct game_closer *game_closer = (struct game_closer *) component;

    struct game_closer* this = calloc(1, sizeof(struct game_closer));
    this->base = base;
    this->hotkey = strdup(game_closer->hotkey);
    return (struct component*)this;
}

static void game_closer_execute(void* listener, void *context) {
    const struct game_closer* this = listener;
    const struct component* base = listener;
    struct engine_lifecycle* engine_lifecycle = (struct engine_lifecycle*)subsystem_collection_get(scene_get_subsystems(entity_get_scene(component_get_parent(base))), "engine_lifecycle");
    engine_lifecycle_mark_stop(engine_lifecycle);
}

static void game_closer_awake(struct component *base) {
    struct game_closer *this = (struct game_closer *) base;

    struct keyboard* keyboard = (struct keyboard*)subsystem_collection_get(scene_get_subsystems(entity_get_scene(component_get_parent(base))), "keyboard");
    this->on_hotkey_pressed = keyboard_require_action_on_key_pressed(keyboard, this->hotkey);
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