//
// Created by Nikita on 08.10.2026.
//

#include "arrow.h"
#include <mcge/mcge.h>


struct arrow {
    struct component base;

    double speed;

    uint128_t target_id;
};


const char* arrow_component_key(void) {
    return "arrow";
}

static void arrow_create(struct component* base, struct fields *fields) {
    struct arrow* this = (struct arrow*)base;
    this->speed = fields_get_double(fields, "speed", 10);
}

static void arrow_update(struct component* base, const struct update_context *context);

const struct component_vtable arrow_vtable = {
    .component_key = arrow_component_key,
    .size = sizeof(struct arrow),
    .on_create = arrow_create,
    .on_update = arrow_update
};

void arrow_launch(struct arrow *this, struct character *target) {
    this->target_id = component_get_id((struct component*)target);
}


static void arrow_update(struct component* base, const struct update_context *context) {
    const struct arrow *this = (struct arrow*)base;

    const struct component* target = scene_try_get_component(this->target_id);
    if (target == NULL) {
        entity_mark_destroyed(component_get_parent(base));
        return;
    }

    const struct vector2 position = component_get_rect(base).position;
    const struct vector2 target_position = component_get_rect(target).position;

    const struct vector2 direction = vector_normalize(vector_sub(target_position, position));
    const struct vector2 offset = vector_multiply_scalar(direction, this->speed * context->dt);

    struct rect rect = entity_get_local_rect(component_get_parent(base));
    rect.position = vector_sum(rect.position, offset);
    entity_set_local_rect(component_get_parent(base), rect);
}