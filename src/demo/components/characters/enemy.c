//
// Created by nikita on 03.10.2026.
//

#include "enemy.h"

#include <stdlib.h>

#include "../../../scene/components/component_internal.h"


struct enemy {
    struct component base;
};

static struct component* enemy_clone(struct component base, const struct component *component);

static const struct component_vtable enemy_vtable = {
    .component_key = enemy_component_key,
    .on_clone = enemy_clone
};

const char* enemy_component_key(void) {
    return "enemy";
}

struct component* enemy_create(struct parser *parser, struct entity *parent) {
    struct enemy *this = calloc(1, sizeof(struct enemy));
    struct component *base = (struct component *) this;
    component_base_create(base, &enemy_vtable, parent);
    return base;
}

static struct component* enemy_clone(struct component base, const struct component *component) {
    const struct enemy *enemy = (const struct enemy *) component;

    struct enemy* this = calloc(1, sizeof(struct enemy));
    this->base = base;
    return (struct component*)this;
}