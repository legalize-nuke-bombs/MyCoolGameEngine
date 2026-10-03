//
// Created by nikita on 03.10.2026.
//

#include "skilled.h"



#include <stdlib.h>
#include "../../../../scene/components/component_internal.h"

struct skilled {
    struct component base;
};

static struct component* skilled_clone(struct component base, const struct component *component);

static const struct component_vtable skilled_vtable = {
    .component_key = skilled_component_key,
    .on_clone = skilled_clone
};

const char* skilled_component_key(void) {
    return "skilled";
}

struct component* skilled_create(struct parser *parser, struct entity *parent) {
    struct skilled *this = calloc(1, sizeof(struct skilled));
    struct component *base = (struct component *) this;
    component_base_create(base, &skilled_vtable, parent);
    return base;
}

static struct component* skilled_clone(struct component base, const struct component *component) {
    const struct skilled *skilled = (const struct skilled *) component;

    struct skilled* this = calloc(1, sizeof(struct skilled));
    this->base = base;
    return (struct component*)this;
}