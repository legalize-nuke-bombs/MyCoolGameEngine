#include "pulsator.h"

#include <math.h>
#include <stdlib.h>

#include "../../scene/components/component_internal.h"
#include "../../scene/entity.h"
#include "../../utils/parser.h"
#include "../../utils/vector2_math.h"


struct pulsator {
    struct component base;

    double lower_coefficient;
    double upper_coefficient;
    double speed;

    double timer;
    struct vector2 origin_size;
};

static struct component* pulsator_clone(struct component base, const struct component *component);
static void pulsator_awake(struct component *base);
static void pulsator_visible_chunk_update(struct component *base, const struct update_context *context);

static const struct component_vtable pulsator_vtable = {
    .component_key = pulsator_component_key,
    .on_clone = pulsator_clone,
    .on_awake = pulsator_awake,
    .on_visible_chunk_update = pulsator_visible_chunk_update,
};

const char* pulsator_component_key(void) {
    return "pulsator";
}

struct component* pulsator_create(struct parser *parser, struct entity *parent) {
    struct pulsator *this = calloc(1, sizeof(struct pulsator));
    struct component *base = (struct component *) this;
    component_base_create(base, &pulsator_vtable, parent);

    parser_next_double(parser, &this->lower_coefficient);
    parser_next_double(parser, &this->upper_coefficient);
    parser_next_double(parser, &this->speed);

    return base;
}

static struct component* pulsator_clone(struct component base, const struct component *component) {
    const struct pulsator *pulsator = (const struct pulsator *) component;

    struct pulsator* this = calloc(1, sizeof(struct pulsator));
    this->base = base;
    this->lower_coefficient = pulsator->lower_coefficient;
    this->upper_coefficient = pulsator->upper_coefficient;
    this->speed = pulsator->speed;
    return (struct component*)this;
}

static void pulsator_awake(struct component *base) {
    struct pulsator *this = (struct pulsator *) base;
    this->origin_size = entity_get_local_rect(component_get_parent(base)).size;
}

static void pulsator_visible_chunk_update(struct component *base, const struct update_context *context) {
    struct pulsator *this = (struct pulsator *) base;
    struct entity *parent = component_get_parent(base);

    this->timer += context->dt;
    const double normalized_sin = (sin(this->timer * this->speed) + 1.0) / 2.0;
    const double k = this->lower_coefficient + normalized_sin * (this->upper_coefficient - this->lower_coefficient);

    struct rect rect = entity_get_local_rect(parent);
    rect.size = vector_multiply_scalar(this->origin_size, k);
    entity_set_local_rect(parent, rect);
}
