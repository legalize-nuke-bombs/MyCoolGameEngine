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

    struct vector2 direction = {0.0, 0.0};

    if (keyboard_is_pressed(this->keyboard, "W") || keyboard_is_pressed(this->keyboard, "Up")) {
        direction.y += 1.0;
    }
    if (keyboard_is_pressed(this->keyboard, "S") || keyboard_is_pressed(this->keyboard, "Down")) {
        direction.y -= 1.0;
    }
    if (keyboard_is_pressed(this->keyboard, "A") || keyboard_is_pressed(this->keyboard, "Left")) {
        direction.x -= 1.0;
    }
    if (keyboard_is_pressed(this->keyboard, "D") || keyboard_is_pressed(this->keyboard, "Right")) {
        direction.x += 1.0;
    }

    direction = vector_normalize(direction);

    const struct vector2 offset = vector_multiply_vector(direction, vector_multiply_scalar(this->v, dt));

    const struct vector2 current_position = transform_get_position(this->transform);
    const struct vector2 new_position = vector_sum(current_position, offset);

    transform_set_position(this->transform, new_position);
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

    parser_next_double(parser, &this->v.x);
    parser_next_double(parser, &this->v.y);

    return base;
}