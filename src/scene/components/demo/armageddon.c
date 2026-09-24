//
// Created by nikita on 24.09.2026.
//

#include "armageddon.h"

#include <stdlib.h>


#include "../component_internal.h"
#include "../../entity.h"
#include "../../scene.h"
#include "../../../utils/parser.h"
#include "SDL3/SDL_keyboard.h"


struct armageddon {
    struct component base;
    SDL_Scancode code;
    const struct tmap *tmap;
};

static void armageddon_awake(struct component *base);
static void armageddon_update(struct component *base, const struct update_context *context);

static const struct component_vtable armageddon_vtable = {
    .component_key = armageddon_component_key,
    .on_awake = armageddon_awake,
    .on_update = armageddon_update,
    .on_disable = NULL,
    .on_destroy = NULL
};

const char* armageddon_component_key(void) {
    return "armageddon";
}

struct component* armageddon_create(struct parser *parser, struct entity *parent) {
    struct armageddon *this = malloc(sizeof(struct armageddon));
    struct component *base = (struct component *)this;
    component_init(base, &armageddon_vtable, parser, parent);

    const char* active_key = parser_next(parser);
    this->code = SDL_GetScancodeFromName(active_key);

    return base;
}

static void armageddon_awake(struct component *base) {
    struct armageddon *this = (struct armageddon *)base;
    this->tmap = scene_get_tmap(entity_get_parent(component_get_parent(base)));
}

static void armageddon_update(struct component *base, const struct update_context *context) {
    const struct armageddon *this = (struct armageddon *)base;

    // TODO This shit must be implemented via actions.
    // However, actions do not support unsubscribes yet
    if (!SDL_GetKeyboardState(NULL)[this->code]) {
        return;
    }

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
        if (target_entity == component_get_parent(base)) {
            continue;
        }
        entity_mark_destroyed(target_entity);
    }
}