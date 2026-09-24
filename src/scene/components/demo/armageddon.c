//
// Created by nikita on 24.09.2026.
//

#include "armageddon.h"

#include <stdlib.h>


#include "../component_internal.h"
#include "../../entity.h"
#include "../../scene.h"
#include "../../../devices/devices.h"
#include "../../../devices/keyboard.h"
#include "../../../engine/engine.h"
#include "../../../utils/action.h"
#include "../../../utils/parser.h"
#include "SDL3/SDL_keyboard.h"


struct armageddon {
    struct component base;
    char* keycode;
    unsigned int subscription_token;
    struct action *on_key_pressed;
    const struct tmap *tmap;
};

static void armageddon_awake(struct component *base);
static void armageddon_on_disable(struct component *base);
static void armageddon_on_destroy(struct component *base);

static const struct component_vtable armageddon_vtable = {
    .component_key = armageddon_component_key,
    .on_awake = armageddon_awake,
    .on_update = NULL,
    .on_disable = armageddon_on_disable,
    .on_destroy = armageddon_on_destroy
};

const char* armageddon_component_key(void) {
    return "armageddon";
}

struct component* armageddon_create(struct parser *parser, struct entity *parent) {
    struct armageddon *this = malloc(sizeof(struct armageddon));
    struct component *base = (struct component *)this;
    component_init(base, &armageddon_vtable, parser, parent);

    this->keycode = parser_next_dup(parser);

    return base;
}

static void armageddon_on_destroy(struct component *base) {
    const struct armageddon *this = (struct armageddon *)base;

    free(this->keycode);
}

static void armageddon_execute(void *base, void *context) {
    struct armageddon *this = (struct armageddon *)base;

    const struct list *everyone = tmap_try_get_components(this->tmap, "transform");
    if (everyone == NULL) {
        return;
    }

    for (int i = 0; i < list_count(everyone); i++) {
        const struct component* target_component = list_get(everyone, i);
        if (target_component == NULL || !component_is_alive(target_component)) {
            continue;
        }
        struct entity* target_entity = component_get_parent(target_component);
        if (target_entity == component_get_parent((struct component*)this)) {
            continue;
        }
        entity_mark_destroyed(target_entity);
    }
}

static void armageddon_awake(struct component *base) {
    struct armageddon *this = (struct armageddon *)base;
    const struct engine *engine = entity_get_engine(component_get_parent(base));
    struct keyboard *keyboard = devices_get_keyboard(engine_get_devices(engine));
    this->on_key_pressed = keyboard_require_action_on_key_pressed(keyboard, this->keycode);
    action_subscribe(this->on_key_pressed, this, armageddon_execute, &this->subscription_token);
    this->tmap = scene_get_tmap(entity_get_parent(component_get_parent(base)));
}

static void armageddon_on_disable(struct component *base) {
    const struct armageddon *this = (struct armageddon *)base;
    action_unsubscribe(this->on_key_pressed, this->subscription_token);
}