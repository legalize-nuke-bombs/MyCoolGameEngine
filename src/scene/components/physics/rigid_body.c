//
// Created by nikita on 01.10.2026.
//

#include "rigid_body.h"
#include "../component_internal.h"
#include "../../../utils/parser.h"


#define INF 1e+9


struct rigid_body {
    struct component base;
    double m;
    struct vector2 a, v;
};

static struct component* rigid_body_clone(struct component base, const struct component *component);
static void rigid_body_simulation_chunk_update(struct component* base, const struct update_context *context);

static const struct component_vtable rigid_body_vtable = {
    .component_key = rigid_body_component_key,
    .on_clone = rigid_body_clone,
    .on_simulation_chunk_update = rigid_body_simulation_chunk_update
};

const char* rigid_body_component_key(void) {
    return "rigid_body";
}

struct component* rigid_body_create(struct parser *parser, struct entity *parent) {
    struct rigid_body *this = calloc(1, sizeof(struct rigid_body));
    struct component *base = (struct component *) this;
    component_base_create(base, &rigid_body_vtable, parent);

    parser_next_double(parser, &this->m);
    if (this->m <= 0) this->m = INF;

    return base;
}

static struct component* rigid_body_clone(struct component base, const struct component *component) {
    const struct rigid_body *rigid_body = (struct rigid_body *) component;

    struct rigid_body* this = calloc(1, sizeof(struct rigid_body));
    this->base = base;
    this->m = rigid_body->m;
    return (struct component*)this;
}

static void rigid_body_simulation_chunk_update(struct component* base, const struct update_context *context) {
    struct rigid_body *this = (struct rigid_body *) base;

}