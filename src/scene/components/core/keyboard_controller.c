//
// Created by Nikita on 26.09.2026.
//

#include "keyboard_controller.h"

#include <stdlib.h>

#include "transform.h"
#include "../component_internal.h"
#include "../../scene.h"
#include "../../../subsystems/subsystem_collection.h"
#include "../../entity.h"
#include "../../../devices/keyboard.h"
#include "controller.h"


struct keyboard_controller {
    struct component base;
    struct controller* controller;
    struct keyboard* keyboard;
};

static void keyboard_controller_on_awake(struct component* base);
static void keyboard_controller_on_update(struct component* base, const struct update_context *context);
static void keyboard_controller_on_disable(struct component* base);

static const struct component_vtable keyboard_controller_vtable = {
    .component_key = keyboard_controller_component_key,
    .on_awake = keyboard_controller_on_awake,
    .on_update = keyboard_controller_on_update,
    .on_disable = keyboard_controller_on_disable
};

const char* keyboard_controller_component_key(void) {
    return "keyboard_controller";
}

static void keyboard_controller_on_update(struct component* base, const struct update_context *context) {
    const struct keyboard_controller* this = (struct keyboard_controller*)base;

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

    controller_move(this->controller, direction, context->dt);
}

static void keyboard_controller_on_awake(struct component* base) {
    struct keyboard_controller* this = (struct keyboard_controller*)base;
    this->controller = (struct controller*)entity_get_component(component_get_parent(base), "controller");
    if (this->controller == NULL) {
        entity_mark_destroyed(component_get_parent(base));
    }
    this->keyboard = (struct keyboard*)subsystem_collection_get(scene_get_subsystems(entity_get_parent(component_get_parent(base))), "keyboard");
}

static void keyboard_controller_on_disable(struct component* base) {
    struct keyboard_controller* this = (struct keyboard_controller*)base;
    this->controller = NULL;
    this->keyboard = NULL;
}

struct component* keyboard_controller_create(struct parser *parser, struct entity *parent) {
    struct keyboard_controller *this = calloc(1, sizeof(struct keyboard_controller));
    struct component *base = (struct component *) this;
    component_create(base, &keyboard_controller_vtable, parser, parent);

    return base;
}