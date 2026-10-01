#include "keyboard_rigid_controller.h"

#include <stdlib.h>

#include "../component_internal.h"
#include "../../scene.h"
#include "../../../subsystems/subsystem_collection.h"
#include "../../entity.h"
#include "../../../devices/keyboard.h"
#include "../../../utils/parser.h"
#include "../../../utils/vector2_math.h"
#include "rigid_body.h"


struct keyboard_rigid_controller {
    struct component base;
    double force;
    struct rigid_body* rigid_body;
    struct keyboard* keyboard;
};

static struct component* keyboard_rigid_controller_clone(struct component base, const struct component *component);
static void keyboard_rigid_controller_on_awake(struct component* base);
static void keyboard_rigid_controller_on_update(struct component* base, const struct update_context *context);
static void keyboard_rigid_controller_on_disable(struct component* base);

static const struct component_vtable keyboard_rigid_controller_vtable = {
    .component_key = keyboard_rigid_controller_component_key,
    .on_clone = keyboard_rigid_controller_clone,
    .on_awake = keyboard_rigid_controller_on_awake,
    .on_update = keyboard_rigid_controller_on_update,
    .on_disable = keyboard_rigid_controller_on_disable
};

const char* keyboard_rigid_controller_component_key(void) {
    return "keyboard_rigid_controller";
}

static void keyboard_rigid_controller_on_update(struct component* base, const struct update_context *context) {
    const struct keyboard_rigid_controller* this = (struct keyboard_rigid_controller*)base;

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

    rigid_body_push(this->rigid_body, vector_multiply_scalar(vector_normalize(direction), this->force));
}

static void keyboard_rigid_controller_on_awake(struct component* base) {
    struct keyboard_rigid_controller* this = (struct keyboard_rigid_controller*)base;
    this->rigid_body = (struct rigid_body*)entity_get_component(component_get_parent(base), "rigid_body");
    if (this->rigid_body == NULL) {
        entity_mark_destroyed(component_get_parent(base));
    }
    this->keyboard = (struct keyboard*)subsystem_collection_get(scene_get_subsystems(entity_get_scene(component_get_parent(base))), "keyboard");
}

static void keyboard_rigid_controller_on_disable(struct component* base) {
    struct keyboard_rigid_controller* this = (struct keyboard_rigid_controller*)base;
    this->rigid_body = NULL;
    this->keyboard = NULL;
}

struct component* keyboard_rigid_controller_create(struct parser *parser, struct entity *parent) {
    struct keyboard_rigid_controller *this = calloc(1, sizeof(struct keyboard_rigid_controller));
    struct component *base = (struct component *) this;
    component_base_create(base, &keyboard_rigid_controller_vtable, parent);

    parser_next_double(parser, &this->force);

    return base;
}

static struct component* keyboard_rigid_controller_clone(struct component base, const struct component *component) {
    const struct keyboard_rigid_controller *keyboard_rigid_controller = (struct keyboard_rigid_controller *) component;

    struct keyboard_rigid_controller* this = calloc(1, sizeof(struct keyboard_rigid_controller));
    this->base = base;
    this->force = keyboard_rigid_controller->force;
    return (struct component*)this;
}
