//
// Created by Nikita on 26.09.2026.
//

#include "keyboard_controller.h"

#include <stdlib.h>

#include "../component_internal.h"
#include "../../scene.h"
#include "../../entity.h"
#include "../../../devices/keyboard.h"
#include "controller.h"
#include "../../../utils/fields.h"


struct keyboard_controller {
    struct component base;

    char* up;
    char* down;
    char* left;
    char* right;

    uint128_t controller_id;
};

static void keyboard_controller_on_create(struct component *base, struct fields *fields);
static void keyboard_controller_on_awake(struct component* base);
static void keyboard_controller_on_update(struct component* base, const struct update_context *context);
static void keyboard_controller_on_destroy(struct component* base);

const struct component_vtable keyboard_controller_vtable = {
    .component_key = keyboard_controller_component_key,
    .size = sizeof(struct keyboard_controller),
    .on_create = keyboard_controller_on_create,
    .on_awake = keyboard_controller_on_awake,
    .on_update = keyboard_controller_on_update,
    .on_destroy = keyboard_controller_on_destroy
};

const char* keyboard_controller_component_key(void) {
    return "keyboard_controller";
}

static void keyboard_controller_on_update(struct component* base, const struct update_context *context) {
    const struct keyboard_controller* this = (struct keyboard_controller*)base;

    const struct controller *controller = (struct controller*)scene_try_get_component(this->controller_id);
    if (controller == NULL) {
        return;
    }

    struct vector2 direction = vector2_zero;

    if (this->up && keyboard_is_pressed(this->up)) {
        direction.y += 1.0;
    }
    if (this->down && keyboard_is_pressed(this->down)) {
        direction.y -= 1.0;
    }
    if (this->left && keyboard_is_pressed(this->left)) {
        direction.x -= 1.0;
    }
    if (this->right && keyboard_is_pressed(this->right)) {
        direction.x += 1.0;
    }

    controller_move(controller, direction, context->dt);
}

static void keyboard_controller_on_awake(struct component* base) {
    struct keyboard_controller* this = (struct keyboard_controller*)base;
    const struct component *controller = entity_get_component(component_get_parent(base), "controller", entity_query_local);
    if (controller == NULL) {
        entity_mark_destroyed(component_get_parent(base));
        return;
    }
    this->controller_id = component_get_id(controller);
}

static void keyboard_controller_on_create(struct component *base, struct fields *fields) {
    struct keyboard_controller *this = (struct keyboard_controller *) base;
    this->up = fields_dup_string(fields, "up", NULL);
    this->down = fields_dup_string(fields, "down", NULL);
    this->left = fields_dup_string(fields, "left", NULL);
    this->right = fields_dup_string(fields, "right", NULL);
}

static void keyboard_controller_on_destroy(struct component* base) {
    const struct keyboard_controller* this = (struct keyboard_controller*)base;
    if (this->up) free(this->up);
    if (this->down) free(this->down);
    if (this->left) free(this->left);
    if (this->right) free(this->right);
}

