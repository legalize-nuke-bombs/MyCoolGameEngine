//
// Created by Nikita on 03.10.2026.
//

#include "player.h"


#include "player.h"

#include <stdlib.h>
#include "../component_internal.h"

struct player {
    struct component base;
};

static struct component* player_clone(struct component base, const struct component *component);

static const struct component_vtable player_vtable = {
    .component_key = player_component_key,
    .on_clone = player_clone
};

const char* player_component_key(void) {
    return "player";
}

struct component* player_create(struct parser *parser, struct entity *parent) {
    struct player *this = malloc(sizeof(struct player));
    struct component *base = (struct component *) this;
    component_base_create(base, &player_vtable, parent);
    return base;
}

static struct component* player_clone(struct component base, const struct component *component) {
    const struct player *player = (const struct player *) component;

    struct player* this = calloc(1, sizeof(struct player));
    this->base = base;
    return (struct component*)this;
}