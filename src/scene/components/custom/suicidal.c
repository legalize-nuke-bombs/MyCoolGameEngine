//
// Created by Nikita on 24.09.2026.
//

#include "suicidal.h"

#include <stddef.h>
#include <stdlib.h>

#include "../component_internal.h"
#include "../../entity.h"
#include "../../../utils/parser.h"
#include "SDL3/SDL_keyboard.h"


struct suicidal {
    struct component base;
    SDL_Scancode code;
};

static void suicidal_update(struct component *base, const struct update_context *context);

static const struct component_vtable suicidal_vtable = {
    .component_key = suicidal_component_key,
    .on_awake = NULL,
    .on_update = suicidal_update,
    .on_disable = NULL,
    .on_destroy = NULL
};

const char* suicidal_component_key(void) {
    return "suicidal";
}

struct component* suicidal_create(struct parser *parser, struct entity *parent) {
    struct suicidal *this = malloc(sizeof(struct suicidal));
    struct component *base = (struct component *)this;
    component_init(base, &suicidal_vtable, parser, parent);

    const char* active_key = parser_next(parser);
    this->code = SDL_GetScancodeFromName(active_key);

    return base;
}

static void suicidal_update(struct component *base, const struct update_context *context) {
    const struct suicidal *this = (struct suicidal *)base;

    if (SDL_GetKeyboardState(NULL)[this->code]) {
        // TODO This shit must be implemented via actions.
        // However, actions do not support unsubscribes yet
        entity_mark_destroyed(component_get_parent(base));
    }
}
