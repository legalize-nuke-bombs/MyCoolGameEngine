//
// Created by nikita on 28.09.2026.
//

#include "box_light_animated.h"

#include <math.h>
#include <stdlib.h>

#include "../component_internal.h"
#include "../../entity.h"
#include "../../../rendering/renderer_pipeline.h"
#include "../../../utils/parser.h"
#include "../../../utils/vector2_math.h"


struct box_light_animated {
    struct component base;

    double lower_coefficient;
    double upper_coefficient;
    double speed;

    double timer;
    struct vector2 box_light_origin_scale;
    struct box_light *box_light;
};

static struct component* box_light_animated_clone(struct component base, const struct component *component);
static void box_light_animated_awake(struct component *base);
static void box_light_animated_update(struct component *base, const struct update_context *context);
static bool box_light_animated_is_chunkable() {
    return true;
}

static const struct component_vtable box_light_animated_vtable = {
    .component_key = box_light_animated_component_key,
    .on_clone = box_light_animated_clone,
    .on_awake = box_light_animated_awake,
    .on_update = box_light_animated_update,
    .is_chunkable = box_light_animated_is_chunkable,
    .on_disable = NULL
};

const char* box_light_animated_component_key(void) {
    return "box_light_animated";
}

struct component* box_light_animated_create(struct parser *parser, struct entity *parent) {
    struct box_light_animated *this = calloc(1, sizeof(struct box_light_animated));
    struct component *base = (struct component *) this;
    component_base_create(base, &box_light_animated_vtable, parser, parent);

    parser_next_double(parser, &this->lower_coefficient);
    parser_next_double(parser, &this->upper_coefficient);
    parser_next_double(parser, &this->speed);

    return base;
}

static struct component* box_light_animated_clone(struct component base, const struct component *component) {
    struct box_light_animated *box_light_animated = (struct box_light_animated *) component;

    struct box_light_animated* this = calloc(1, sizeof(struct box_light_animated));
    this->base = base;
    this->lower_coefficient = box_light_animated->lower_coefficient;
    this->upper_coefficient = box_light_animated->upper_coefficient;
    this->speed = box_light_animated->speed;
    return (struct component*)this;
}

static void box_light_animated_awake(struct component *base) {
    struct box_light_animated *this = (struct box_light_animated *) base;
    struct entity *parent = component_get_parent(base);

    this->box_light = (struct box_light*)entity_get_component(parent, "box_light");
    if (this->box_light == NULL) {
        entity_mark_destroyed(parent);
    }
    this->box_light_origin_scale = component_get_local_rect((const struct component*)this->box_light).size;
}

static void box_light_animated_update(struct component *base, const struct update_context *context) {
    struct box_light_animated *this = (struct box_light_animated *) base;

    this->timer += context->dt;
    const double normalized_sin = (sin(this->timer * this->speed) + 1.0) / 2.0;
    const double k = this->lower_coefficient + normalized_sin * (this->upper_coefficient - this->lower_coefficient);

    struct rect rect = component_get_local_rect((struct component*)this->box_light);
    rect.size = vector_multiply_scalar(this->box_light_origin_scale, k);
    component_set_local_rect((struct component*)this->box_light, rect);
}