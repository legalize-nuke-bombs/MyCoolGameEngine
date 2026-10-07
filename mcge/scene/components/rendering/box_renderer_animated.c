#include "box_renderer_animated.h"

#include <stdlib.h>

#include "box_renderer.h"
#include "../component_internal.h"
#include "../../entity.h"
#include "../../scene.h"
#include "../../../utils/fields.h"


struct box_renderer_animated {
    struct component base;

    double cycles_per_second;
    double frame_timer;

    uint128_t box_renderer_id;
};

static void box_renderer_animated_on_create(struct component *base, struct fields *fields);
static void box_renderer_animated_awake(struct component *base);
static void box_renderer_animated_visible_chunk_update(struct component *base, const struct update_context *context);

const struct component_vtable box_renderer_animated_vtable = {
    .component_key = box_renderer_animated_component_key,
    .size = sizeof(struct box_renderer_animated),
    .on_create = box_renderer_animated_on_create,
    .on_awake = box_renderer_animated_awake,
    .on_visible_chunk_update = box_renderer_animated_visible_chunk_update
};

const char* box_renderer_animated_component_key(void) {
    return "box_renderer_animated";
}

static void box_renderer_animated_on_create(struct component *base, struct fields *fields) {
    struct box_renderer_animated *this = (struct box_renderer_animated *) base;
    this->cycles_per_second = fields_get_double(fields, "speed", 1);
}

static void box_renderer_animated_awake(struct component *base) {
    struct box_renderer_animated *this = (struct box_renderer_animated *) base;
    struct entity *parent = component_get_parent(base);

    const struct component *box_renderer = entity_get_component(parent, "box_renderer", entity_query_local);
    if (box_renderer == NULL) {
        entity_mark_destroyed(parent);
        return;
    }
    this->box_renderer_id = component_get_id(box_renderer);
}

static void box_renderer_animated_visible_chunk_update(struct component *base, const struct update_context *context) {
    struct box_renderer_animated *this = (struct box_renderer_animated *) base;

    struct box_renderer *box_renderer = (struct box_renderer*)scene_try_get_component(this->box_renderer_id);
    if (box_renderer == NULL) {
        return;
    }

    const double frame_time = 1 / (this->cycles_per_second * box_renderer_get_texture_frames(box_renderer));

    this->frame_timer += context->dt;
    if (this->frame_timer >= frame_time) {
        this->frame_timer -= frame_time;
        box_renderer_bump_texture_frame(box_renderer);
    }
}