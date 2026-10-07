#include "keyboard_rigid_controller.h"

#include <stdlib.h>

#include "../component_internal.h"
#include "../../scene.h"
#include "../../entity.h"
#include "../../../devices/keyboard.h"
#include "../../../utils/fields.h"
#include "../../../utils/vector2_math.h"
#include "rigid_body.h"


struct keyboard_rigid_controller {
    struct component base;

    double speed;

    char* up;
    char* down;
    char* left;
    char* right;

    uint128_t rigid_body_id;
};

static void keyboard_rigid_controller_on_create(struct component *base, struct fields *fields);
static void keyboard_rigid_controller_on_awake(struct component* base);
static void keyboard_rigid_controller_on_update(struct component* base, const struct update_context *context);
static void keyboard_rigid_controller_on_destroy(struct component* base);

const struct component_vtable keyboard_rigid_controller_vtable = {
    .component_key = keyboard_rigid_controller_component_key,
    .size = sizeof(struct keyboard_rigid_controller),
    .on_create = keyboard_rigid_controller_on_create,
    .on_awake = keyboard_rigid_controller_on_awake,
    .on_update = keyboard_rigid_controller_on_update,
    .on_destroy = keyboard_rigid_controller_on_destroy
};

const char* keyboard_rigid_controller_component_key(void) {
    return "keyboard_rigid_controller";
}

static void keyboard_rigid_controller_on_update(struct component* base, const struct update_context *context) {
    const struct keyboard_rigid_controller* this = (struct keyboard_rigid_controller*)base;

    struct rigid_body *rigid_body = (struct rigid_body*)scene_try_get_component(this->rigid_body_id);
    if (rigid_body == NULL) {
        return;
    }

    struct vector2 direction = {0.0, 0.0};

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

    const struct vector2 v_target = vector_multiply_scalar(vector_normalize(direction), this->speed);
    const struct vector2 delta_v = vector_sub(v_target, rigid_body_get_velocity(rigid_body));
    rigid_body_drive(rigid_body, vector_multiply_scalar(delta_v, rigid_body_get_mass(rigid_body)));
}

static void keyboard_rigid_controller_on_awake(struct component* base) {
    struct keyboard_rigid_controller* this = (struct keyboard_rigid_controller*)base;
    const struct component *rigid_body = entity_get_component(component_get_parent(base), "rigid_body", entity_query_local);
    if (rigid_body == NULL) {
        entity_mark_destroyed(component_get_parent(base));
        return;
    }
    this->rigid_body_id = component_get_id(rigid_body);
}

static void keyboard_rigid_controller_on_create(struct component *base, struct fields *fields) {
    struct keyboard_rigid_controller *this = (struct keyboard_rigid_controller *) base;
    this->speed = fields_get_double(fields, "speed", 0);
    this->up = fields_dup_string(fields, "up", NULL);
    this->down = fields_dup_string(fields, "down", NULL);
    this->left = fields_dup_string(fields, "left", NULL);
    this->right = fields_dup_string(fields, "right", NULL);
}

static void keyboard_rigid_controller_on_destroy(struct component* base) {
    const struct keyboard_rigid_controller *this = (struct keyboard_rigid_controller*)base;
    if (this->up) free(this->up);
    if (this->down) free(this->down);
    if (this->left) free(this->left);
    if (this->right) free(this->right);
}

