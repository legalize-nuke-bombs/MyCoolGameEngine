#include "transform.h"

#include <stdlib.h>

#include "../component_internal.h"
#include "../../../utils/parser.h"

struct transform {
    struct component base;
    struct rect rect;
};

static struct component* transform_clone(struct component base, const struct component *component);

static const struct component_vtable transform_vtable = {
    .component_key = transform_component_key,
    .on_clone = transform_clone,
    .on_awake = NULL,
    .on_update = NULL,
    .on_destroy = NULL
};

const char* transform_component_key(void) {
    return "transform";
}

struct component* transform_create(struct parser *parser, struct entity *parent) {
    struct transform *this = malloc(sizeof(struct transform));
    component_base_create((struct component*)(this), &transform_vtable, parser, parent);

    parser_next_double(parser, &this->rect.position.x);
    parser_next_double(parser, &this->rect.position.y);
    parser_next_double(parser, &this->rect.size.x);
    parser_next_double(parser, &this->rect.size.y);

    return (struct component *)this;
}

struct component* transform_clone(struct component base, const struct component *component) {
    const struct transform* transform = (const struct transform*)component;

    struct transform *this = calloc(1, sizeof(struct transform));
    this->base = base;
    this->rect = transform->rect;
    return (struct component*)this;
}

struct rect transform_get_rect(const struct transform *this) {
    return this->rect;
}
void transform_set_rect(struct transform *this, const struct rect rect) {
    this->rect = rect;
}
