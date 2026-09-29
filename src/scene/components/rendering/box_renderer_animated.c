#include "box_renderer_animated.h"

#include <stdlib.h>

#include "box_renderer.h"
#include "../component_internal.h"
#include "../../entity.h"
#include "../../../utils/parser.h"


struct box_renderer_animated {
    struct component base;

    double frame_interval;
    double frame_timer;

    struct box_renderer *box_renderer;
};

static struct component* box_renderer_animated_clone(struct component base, const struct component *component);
static void box_renderer_animated_awake(struct component *base);
static void box_renderer_animated_chunked_update(struct component *base, const struct update_context *context);

static const struct component_vtable box_renderer_animated_vtable = {
    .component_key = box_renderer_animated_component_key,
    .on_clone = box_renderer_animated_clone,
    .on_awake = box_renderer_animated_awake,
    .on_chunked_update = box_renderer_animated_chunked_update
};

const char* box_renderer_animated_component_key(void) {
    return "box_renderer_animated";
}

struct component* box_renderer_animated_create(struct parser *parser, struct entity *parent) {
    struct box_renderer_animated *this = calloc(1, sizeof(struct box_renderer_animated));
    struct component *base = (struct component *) this;
    component_base_create(base, &box_renderer_animated_vtable, parser, parent);

    parser_next_double(parser, &this->frame_interval);

    return base;
}

static struct component* box_renderer_animated_clone(struct component base, const struct component *component) {
    struct box_renderer_animated *box_renderer_animated = (struct box_renderer_animated *) component;

    struct box_renderer_animated* this = calloc(1, sizeof(struct box_renderer_animated));
    this->base = base;
    this->frame_interval = box_renderer_animated->frame_interval;
    return (struct component*)this;
}

static void box_renderer_animated_awake(struct component *base) {
    struct box_renderer_animated *this = (struct box_renderer_animated *) base;
    struct entity *parent = component_get_parent(base);

    this->box_renderer = (struct box_renderer*)entity_get_component(parent, "box_renderer");
    if (this->box_renderer == NULL) {
        entity_mark_destroyed(parent);
    }
}

static void box_renderer_animated_chunked_update(struct component *base, const struct update_context *context) {
    struct box_renderer_animated *this = (struct box_renderer_animated *) base;

    this->frame_timer += context->dt;
    if (this->frame_timer >= this->frame_interval) {
        this->frame_timer -= this->frame_interval;
        box_renderer_bump_texture_frame(this->box_renderer);
    }
}