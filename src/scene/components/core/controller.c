//
// Created by Nikita on 26.09.2026.
//

#include "controller.h"

#include <stdlib.h>

#include "transform.h"
#include "../component_internal.h"
#include "../../entity.h"
#include "../../../utils/parser.h"
#include "../../../utils/vector2_math.h"


struct controller {
    struct component base;
    double v;
    struct transform* transform;
};

static struct component* controller_clone(struct component base, const struct component *component);
static void controller_on_awake(struct component* base);
static void controller_on_disable(struct component* base);

static const struct component_vtable controller_vtable = {
    .component_key = controller_component_key,
    .on_clone = controller_clone,
    .on_awake = controller_on_awake,
    .on_update = NULL,
    .on_disable = controller_on_disable
};

const char* controller_component_key(void) {
    return "controller";
}

static void controller_on_awake(struct component* base) {
    struct controller* this = (struct controller*)base;
    this->transform = entity_get_transform(component_get_parent(base));
}

static void controller_on_disable(struct component* base) {
    struct controller* this = (struct controller*)base;
    this->transform = NULL;
}

struct component* controller_create(struct parser *parser, struct entity *parent) {
    struct controller *this = calloc(1, sizeof(struct controller));
    struct component *base = (struct component *) this;
    component_base_create(base, &controller_vtable, parser, parent);

    parser_next_double(parser, &this->v);

    return base;
}

static struct component* controller_clone(struct component base, const struct component *component) {
    const struct controller *controller = (struct controller *) component;

    struct controller* this = calloc(1, sizeof(struct controller));
    this->base = base;
    this->v = controller->v;
    return (struct component*)this;
}

void controller_move(const struct controller* this, struct vector2 direction, const double dt) {
    direction = vector_normalize(direction);

    const struct vector2 offset = vector_multiply_scalar(direction, this->v * dt);

    struct rect rect = transform_get_rect(this->transform);
    rect.position = vector_sum(rect.position, offset);

    transform_set_rect(this->transform, rect);
}