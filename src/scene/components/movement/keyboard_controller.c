//
// Created by Nikita on 26.09.2026.
//

#include "keyboard_controller.h"

#include <stdlib.h>

#include "../component_internal.h"
#include "../../scene.h"
#include "../../../subsystems/subsystem_collection.h"
#include "../../entity.h"
#include "../../../devices/keyboard.h"
#include "controller.h"
#include "../../../utils/parser.h"


struct keyboard_controller {
    struct component base;

    char* up;
    char* down;
    char* left;
    char* right;

    struct controller* controller;
    struct keyboard* keyboard;
};

static struct component* keyboard_controller_clone(struct component base, const struct component *component);
static void keyboard_controller_on_awake(struct component* base);
static void keyboard_controller_on_update(struct component* base, const struct update_context *context);
static void keyboard_controller_on_disable(struct component* base);
static void keyboard_controller_on_destroy(struct component* base);

static const struct component_vtable keyboard_controller_vtable = {
    .component_key = keyboard_controller_component_key,
    .on_clone = keyboard_controller_clone,
    .on_awake = keyboard_controller_on_awake,
    .on_update = keyboard_controller_on_update,
    .on_disable = keyboard_controller_on_disable,
    .on_destroy = keyboard_controller_on_destroy
};

const char* keyboard_controller_component_key(void) {
    return "keyboard_controller";
}

static void keyboard_controller_on_update(struct component* base, const struct update_context *context) {
    const struct keyboard_controller* this = (struct keyboard_controller*)base;

    struct vector2 direction = vector2_zero;

    if (this->up && keyboard_is_pressed(this->keyboard, this->up)) {
        direction.y += 1.0;
    }
    if (this->down && keyboard_is_pressed(this->keyboard, this->down)) {
        direction.y -= 1.0;
    }
    if (this->left && keyboard_is_pressed(this->keyboard, this->left)) {
        direction.x -= 1.0;
    }
    if (this->right && keyboard_is_pressed(this->keyboard, this->right)) {
        direction.x += 1.0;
    }

    controller_move(this->controller, direction, context->dt);
}

static void keyboard_controller_on_awake(struct component* base) {
    struct keyboard_controller* this = (struct keyboard_controller*)base;
    this->controller = (struct controller*)entity_get_component(component_get_parent(base), "controller", entity_query_local);
    if (this->controller == NULL) {
        entity_mark_destroyed(component_get_parent(base));
    }
    this->keyboard = (struct keyboard*)subsystem_collection_get(scene_get_subsystems(entity_get_scene(component_get_parent(base))), "keyboard");
}

static void keyboard_controller_on_disable(struct component* base) {
    struct keyboard_controller* this = (struct keyboard_controller*)base;
    this->controller = NULL;
    this->keyboard = NULL;
}

struct component* keyboard_controller_create(struct parser *parser, struct entity *parent) {
    struct keyboard_controller *this = calloc(1, sizeof(struct keyboard_controller));
    struct component *base = (struct component *) this;
    component_base_create(base, &keyboard_controller_vtable, parent);

    this->up = parser_next_dup(parser);
    this->down = parser_next_dup(parser);
    this->left = parser_next_dup(parser);
    this->right = parser_next_dup(parser);

    return base;
}

static void keyboard_controller_on_destroy(struct component* base) {
    const struct keyboard_controller* this = (struct keyboard_controller*)base;
    if (this->up) free(this->up);
    if (this->down) free(this->down);
    if (this->left) free(this->left);
    if (this->right) free(this->right);
}

static struct component* keyboard_controller_clone(struct component base, const struct component *component) {
    const struct keyboard_controller *keyboard_controller = (struct keyboard_controller *) component;

    struct keyboard_controller* this = calloc(1, sizeof(struct keyboard_controller));
    this->base = base;
    return (struct component*)this;
}