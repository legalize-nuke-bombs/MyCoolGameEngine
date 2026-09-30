#include "scene_switcher.h"

#include <string.h>

#include "../component_internal.h"
#include "../../entity.h"
#include "../../../devices/keyboard.h"
#include "../../scene.h"
#include "../../../subsystems/subsystem_collection.h"
#include "../../../utils/parser.h"


struct scene_switcher {
    struct component base;

    char* hotkey;
    char* map_path;

    struct action* on_hotkey_pressed;
    unsigned int on_hotkey_pressed_token;
};

static struct component* scene_switcher_clone(struct component base, const struct component *component);
static void scene_switcher_awake(struct component *base);
static void scene_switched_on_disable(struct component *base);
static void scene_switcher_on_destroy(struct component *base);

static const struct component_vtable menu_vtable = {
    .component_key = scene_switcher_component_key,
    .on_clone = scene_switcher_clone,
    .on_awake = scene_switcher_awake,
    .on_disable = scene_switched_on_disable,
    .on_destroy = scene_switcher_on_destroy
};

const char* scene_switcher_component_key(void) {
    return "scene_switcher";
}

struct component* scene_switcher_create(struct parser *parser, struct entity *parent) {
    struct scene_switcher *this = calloc(1, sizeof(struct scene_switcher));
    struct component *base = (struct component *) this;
    component_base_create(base, &menu_vtable, parent);

    this->hotkey = parser_next_dup(parser);
    this->map_path = parser_next_dup(parser);

    return base;
}

static struct component* scene_switcher_clone(struct component base, const struct component *component) {
    const struct scene_switcher *scene_switcher = (struct scene_switcher *) component;

    struct scene_switcher* this = calloc(1, sizeof(struct scene_switcher));
    this->base = base;
    this->hotkey = strdup(scene_switcher->hotkey);
    this->map_path = strdup(scene_switcher->map_path);
    return (struct component*)this;
}

static void scene_switcher_execute(void* listener, void *context) {
    const struct scene_switcher* this = listener;
    const struct component* base = listener;
    struct scene *scene = entity_get_scene(component_get_parent(base));
    scene_mark_switch(scene, this->map_path);
}

static void scene_switcher_awake(struct component *base) {
    struct scene_switcher *this = (struct scene_switcher *) base;

    struct keyboard* keyboard = (struct keyboard*)subsystem_collection_get(scene_get_subsystems(entity_get_scene(component_get_parent(base))), "keyboard");
    this->on_hotkey_pressed = keyboard_require_action_on_key_pressed(keyboard, this->hotkey);
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