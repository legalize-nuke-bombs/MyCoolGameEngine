//
// Created by Nikita on 26.09.2026.
//

#include "controller.h"

#include <stdlib.h>

#include "../component_internal.h"
#include "../../entity.h"
#include "../../../utils/fields.h"
#include "../../../utils/vector2_math.h"


struct controller {
    struct component base;
    double v;
};

static void controller_on_create(struct component *base, struct fields *fields);

const struct component_vtable controller_vtable = {
    .component_key = controller_component_key,
    .size = sizeof(struct controller),
    .on_create = controller_on_create,
    .on_update = NULL
};

const char* controller_component_key(void) {
    return "controller";
}

static void controller_on_create(struct component *base, struct fields *fields) {
    struct controller *this = (struct controller *) base;
    this->v = fields_get_double(fields, "speed", 0);
}

void controller_move(const struct controller* this, struct vector2 direction, const double dt) {
    direction = vector_normalize(direction);

    const struct vector2 offset = vector_multiply_scalar(direction, this->v * dt);

    struct entity* parent = component_get_parent((const struct component*)this);
    struct rect rect = entity_get_local_rect(parent);
    rect.position = vector_sum(rect.position, offset);

    entity_set_local_rect(parent, rect);
}