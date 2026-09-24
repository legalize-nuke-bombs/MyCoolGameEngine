#include "transform.h"

#include <stdlib.h>

#include "../component_internal.h"
#include "../../../utils/parser.h"

struct transform {
    struct component base;
    struct vector2 position;
    struct vector2 scale;
};

static const struct component_vtable transform_vtable = {
    .component_key = transform_component_key,
    .on_awake = NULL,
    .on_update = NULL,
    .on_destroy = NULL
};

const char* transform_component_key(void) {
    return "transform";
}

struct component* transform_create(struct parser *parser, struct entity *parent) {
    struct transform *this = malloc(sizeof(struct transform));
    component_init((struct component*)(this), &transform_vtable, parser, parent);

    parser_next_double(parser, &this->position.x);
    parser_next_double(parser, &this->position.y);
    parser_next_double(parser, &this->scale.x);
    parser_next_double(parser, &this->scale.y);

    return (struct component *)this;
}

struct vector2 transform_get_position(const struct transform *this) {
    return this->position;
}
void transform_set_position(struct transform *this, const struct vector2 *position) {
    this->position = *position;
}
struct vector2 transform_get_scale(const struct transform *this) {
    return this->scale;
}
void transform_set_scale(struct transform *this, const struct vector2 *scale) {
    this->scale = *scale;
}
