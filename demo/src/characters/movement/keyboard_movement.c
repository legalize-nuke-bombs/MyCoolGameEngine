//
// Created by nikita on 08.10.2026.
//

#include "keyboard_movement.h"
#include <mcge/mcge.h>
#include "movement.h"


struct keyboard_movement {
    struct component base;

    char *up;
    char *down;
    char *left;
    char *right;

    uint128_t movement_id;
};


static void keyboard_movement_on_create(struct component* base, struct fields* fields);
static void keyboard_movement_on_destroy(struct component* base);
static void keyboard_movement_awake(struct component* base);
static void keyboard_movement_simulation_chunk_update(struct component* base, const struct update_context *context);


const char* keyboard_movement_component_key() {
    return "keyboard_movement";
}

const struct component_vtable keyboard_movement_vtable = {
    .component_key = keyboard_movement_component_key,
    .size = sizeof(struct keyboard_movement),
    .on_create = keyboard_movement_on_create,
    .on_destroy = keyboard_movement_on_destroy,
    .on_awake = keyboard_movement_awake,
    .on_simulation_chunk_update = keyboard_movement_simulation_chunk_update
};

static void keyboard_movement_on_create(struct component* base, struct fields* fields) {
    struct keyboard_movement *this = (struct keyboard_movement *) base;
    this->up = fields_dup_string(fields, "up", "W");
    this->down = fields_dup_string(fields, "down", "S");
    this->left = fields_dup_string(fields, "left", "A");
    this->right = fields_dup_string(fields, "right", "D");
}
static void keyboard_movement_on_destroy(struct component* base) {
    const struct keyboard_movement *this = (struct keyboard_movement *) base;
    free(this->up);
    free(this->down);
    free(this->left);
    free(this->right);
}

static void keyboard_movement_awake(struct component* base) {
    struct keyboard_movement *this = (struct keyboard_movement *) base;
    struct movement* movement = (struct movement*)entity_get_component(component_get_parent(base), "movement", entity_query_local);
    if (movement == NULL) {
        entity_mark_destroyed(component_get_parent(base));
        return;
    }
    this->movement_id = component_get_id((struct component*)movement);
}

static void keyboard_movement_simulation_chunk_update(struct component* base, const struct update_context *context) {
    const struct keyboard_movement *this = (struct keyboard_movement *) base;

    struct movement* movement = (struct movement*)scene_try_get_component(this->movement_id);
    if (movement == NULL) {
        return;
    }

    struct vector2 direction = vector2_zero;
    if (keyboard_is_pressed(this->up)) {
        direction.y += 1.0;
    }
    if (keyboard_is_pressed(this->down)) {
        direction.y -= 1.0;
    }
    if (keyboard_is_pressed(this->left)) {
        direction.x -= 1.0;
    }
    if (keyboard_is_pressed(this->right)) {
        direction.x += 1.0;
    }

    if (vectors_equal(direction, vector2_zero)) {
        return;
    }

    movement_try_move(movement, direction);
}