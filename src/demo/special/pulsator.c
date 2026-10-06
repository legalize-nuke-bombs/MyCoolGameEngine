#include "pulsator.h"

#include <math.h>
#include <stdlib.h>

#include "../../scene/components/component_internal.h"
#include "../../scene/entity.h"
#include "../../utils/fields.h"
#include "../../utils/vector2_math.h"


struct pulsator {
    struct component base;

    double lower_coefficient;
    double upper_coefficient;
    double speed;

    double timer;
    struct vector2 origin_size;
};

static void pulsator_on_create(struct component *base, struct fields *fields);
static void pulsator_awake(struct component *base);
static void pulsator_visible_chunk_update(struct component *base, const struct update_context *context);

const struct component_vtable pulsator_vtable = {
    .component_key = pulsator_component_key,
    .size = sizeof(struct pulsator),
    .on_create = pulsator_on_create,
    .on_awake = pulsator_awake,
    .on_visible_chunk_update = pulsator_visible_chunk_update,
};

const char* pulsator_component_key(void) {
    return "pulsator";
}

static void pulsator_on_create(struct component *base, struct fields *fields) {
    struct pulsator *this = (struct pulsator *) base;
    this->lower_coefficient = fields_get_double(fields, "lower", 1);
    this->upper_coefficient = fields_get_double(fields, "upper", 1);
    this->speed = fields_get_double(fields, "speed", 1);
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
