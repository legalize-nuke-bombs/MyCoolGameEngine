#include "forest.h"

#include <stdlib.h>

#include "../component_internal.h"
#include "../../../rendering/renderer_pipeline.h"


struct forest {
    struct component base;
};

static struct component* forest_clone(struct component base, const struct component *component);
static void forest_awake(struct component *base);

static const struct component_vtable forest_vtable = {
    .component_key = forest_component_key,
    .on_clone = forest_clone,
    .on_awake = forest_awake,
    .on_update = NULL,
    .on_disable = NULL
};

const char* forest_component_key(void) {
    return "forest";
}

struct component* forest_create(struct parser *parser, struct entity *parent) {
    struct forest *this = calloc(1, sizeof(struct forest));
    struct component *base = (struct component *) this;
    component_base_create(base, &forest_vtable, parser, parent);

    return base;
}

static struct component* forest_clone(struct component base, const struct component *component) {
    struct forest *forest = (struct forest *) component;

    struct forest* this = calloc(1, sizeof(struct forest));
    this->base = base;
    return (struct component*)this;
}

static void forest_awake(struct component *base) {
    struct camera *forest = (struct camera *) base;

}
