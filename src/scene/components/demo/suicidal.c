//
// Created by Nikita on 24.09.2026.
//

#include "suicidal.h"

#include <stdlib.h>

#include "../component_internal.h"
#include "../../entity.h"
#include "../../scene.h"
#include "../../../devices/keyboard.h"
#include "../../../utils/action.h"
#include "../../../utils/parser.h"
#include "../../../subsystems/subsystem_collection.h"


struct suicidal {
    struct component base;
    char *keycode;
    unsigned int subscription_token;
    struct action *on_key_pressed;
};

static void suicidal_on_awake(struct component *base);
static void suicidal_on_disable(struct component *base);
static void suicidal_on_destroy(struct component *base);

static const struct component_vtable suicidal_vtable = {
    .component_key = suicidal_component_key,
    .on_awake = suicidal_on_awake,
    .on_update = NULL,
    .on_disable = suicidal_on_disable,
    .on_destroy = suicidal_on_destroy
};

const char* suicidal_component_key(void) {
    return "suicidal";
}

static void suicidal_suicide(void* base, void *context) {
    entity_mark_destroyed(component_get_parent(base));
}

static void suicidal_on_awake(struct component *base) {
    struct suicidal *this = (struct suicidal *)base;

    struct keyboard *keyboard = (struct keyboard*)subsystem_collection_get(scene_get_subsystems(entity_get_parent(component_get_parent(base))), "keyboard");
    this->on_key_pressed = keyboard_require_action_on_key_pressed(keyboard, this->keycode);
    action_subscribe(this->on_key_pressed, this, suicidal_suicide, &this->subscription_token);
}
static void suicidal_on_disable(struct component *base) {
    const struct suicidal *this = (struct suicidal *)base;

    action_unsubscribe(this->on_key_pressed, this->subscription_token);
}

struct component* suicidal_create(struct parser *parser, struct entity *parent) {
    struct suicidal *this = malloc(sizeof(struct suicidal));
    struct component *base = (struct component *)this;
    component_create(base, &suicidal_vtable, parser, parent);

    this->keycode = parser_next_dup(parser);

    return base;
}

static void suicidal_on_destroy(struct component *base) {
    const struct suicidal *this = (struct suicidal *)base;

    free(this->keycode);
}
