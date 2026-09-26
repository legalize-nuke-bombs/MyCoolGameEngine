//
// Created by Nikita on 26.09.2026.
//

#include "controller.h"

#include <math.h>
#include <stdlib.h>

#include "transform.h"
#include "../component_internal.h"
#include "../../scene.h"
#include "../../../subsystems/subsystem_collection.h"
#include "../../entity.h"
#include "../../../devices/keyboard.h"
#include "../../../utils/parser.h"
#include "../../../utils/vector2_math.h"


struct controller {
    struct component base;
    struct vector2 a;
    struct vector2 a_slowdown;
    struct vector2 v;
    struct transform* transform;
    struct keyboard* keyboard;
};

static void controller_on_awake(struct component* base);
static void controller_on_update(struct component* base, const struct update_context *context);
static void controller_on_disable(struct component* base);

static const struct component_vtable controller_vtable = {
    .component_key = controller_component_key,
    .on_awake = controller_on_awake,
    .on_update = controller_on_update,
    .on_disable = controller_on_disable
};

const char* controller_component_key(void) {
    return "controller";
}

static void controller_on_awake(struct component* base) {
    struct controller* this = (struct controller*)base;
    this->transform = entity_get_transform(component_get_parent(base));
    this->keyboard = (struct keyboard*)subsystem_collection_get(scene_get_subsystems(entity_get_parent(component_get_parent(base))), "keyboard");
}
static void controller_on_update(struct component* base, const struct update_context *context) {
    struct controller* this = (struct controller*)base;

    const double dt = context->dt;

    bool y_idle = true;
    bool x_idle = true;
    if (keyboard_is_pressed(this->keyboard, "W")) {
        this->v.y += this->a.y * dt;
        y_idle = false;
    }
    if (keyboard_is_pressed(this->keyboard, "S")) {
        this->v.y -= this->a.y * dt;
        y_idle = false;
    }
    if (keyboard_is_pressed(this->keyboard, "A")) {
        this->v.x -= this->a.x * dt;
        x_idle = false;
    }
    if (keyboard_is_pressed(this->keyboard, "D")) {
        this->v.x += this->a.x * dt;
        x_idle = false;
    }

    if (y_idle) {
        this->v.y = ((this->v.y > 0) - 0.5) * 2 * fmaxl(0, fabsl(this->v.y) - this->a_slowdown.y * dt);
    }
    if (x_idle) {
        this->v.x = ((this->v.x > 0) - 0.5) * 2 * fmaxl(0, fabsl(this->v.x) - this->a_slowdown.x * dt);
    }



    transform_set_position(this->transform,
        vector_sum(transform_get_position(this->transform), vector_multiply_scalar(&this->v, dt)));
}
static void controller_on_disable(struct component* base) {
    struct controller* this = (struct controller*)base;
    this->transform = NULL;
    this->keyboard = NULL;
}

struct component* controller_create(struct parser *parser, struct entity *parent) {
    struct controller *this = calloc(1, sizeof(struct controller));
    struct component *base = (struct component *) this;
    component_create(base, &controller_vtable, parser, parent);

    parser_next_double(parser, &this->a.x);
    this->a.y = this->a.x;
    parser_next_double(parser, &this->a_slowdown.x);
    this->a_slowdown.y = this->a_slowdown.x;

    this->v = vector2_zero;
    return base;
}