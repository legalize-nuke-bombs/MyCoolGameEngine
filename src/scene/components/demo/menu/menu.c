#include "menu.h"

#include <string.h>

#include "../../component_internal.h"
#include "../../../entity.h"
#include "../../../../devices/keyboard.h"
#include "../../../../interpreter/interpreter.h"
#include "../../../../logging/logger.h"
#include "../../../../scene/scene.h"
#include "../../../../subsystems/subsystem_collection.h"
#include "../../../../utils/parser.h"


struct menu {
    struct component base;

    char* hotkey;
    char* map_path;

    struct action* on_hotkey_pressed;
    unsigned int on_hotkey_pressed_token;
};

static struct component* menu_clone(struct component base, const struct component *component);
static void menu_awake(struct component *base);
static void menu_on_disable(struct component *base);
static void menu_on_destroy(struct component *base);

static const struct component_vtable menu_vtable = {
    .component_key = menu_component_key,
    .on_clone = menu_clone,
    .on_awake = menu_awake,
    .on_disable = menu_on_disable,
    .on_destroy = menu_on_destroy
};

const char* menu_component_key(void) {
    return "menu";
}

struct component* menu_create(struct parser *parser, struct entity *parent) {
    struct menu *this = calloc(1, sizeof(struct menu));
    struct component *base = (struct component *) this;
    component_base_create(base, &menu_vtable, parent);

    this->hotkey = parser_next_dup(parser);
    this->map_path = parser_next_dup(parser);

    return base;
}

static struct component* menu_clone(struct component base, const struct component *component) {
    const struct menu *menu = (struct menu *) component;

    struct menu* this = calloc(1, sizeof(struct menu));
    this->base = base;
    this->hotkey = strdup(menu->hotkey);
    this->map_path = strdup(menu->map_path);
    return (struct component*)this;
}

static void menu_switch_scene(void* listener, void *context) {
    const struct menu* this = listener;
    const struct component* base = listener;
    logger_info("Entity %s is switching scenes...", component_get_global_parent_name(base));

    // This entity will no longer exist after scene_clear so we should save this shit
    char* map_path = strdup(this->map_path);
    // This is kinda bad
    // TODO

    const struct scene *scene = entity_get_scene(component_get_parent(base));
    scene_clear(scene);
    const struct interpreter* interpreter = (struct interpreter*)subsystem_collection_get(scene_get_subsystems(scene), "interpreter");
    interpreter_eval(interpreter, map_path);

    free(map_path);
}

static void menu_awake(struct component *base) {
    struct menu *this = (struct menu *) base;

    struct keyboard* keyboard = (struct keyboard*)subsystem_collection_get(scene_get_subsystems(entity_get_scene(component_get_parent(base))), "keyboard");
    this->on_hotkey_pressed = keyboard_require_action_on_key_pressed(keyboard, this->hotkey);
    action_subscribe(this->on_hotkey_pressed, this, menu_switch_scene, &this->on_hotkey_pressed_token);
}

static void menu_on_disable(struct component *base) {
    const struct menu *this = (struct menu *)base;
    action_unsubscribe(this->on_hotkey_pressed, this->on_hotkey_pressed_token);
}

static void menu_on_destroy(struct component *base) {
    const struct menu *this = (struct menu *)base;
    free(this->hotkey);
    free(this->map_path);
}