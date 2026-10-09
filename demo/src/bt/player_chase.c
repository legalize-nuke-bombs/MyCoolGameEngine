//
// Created by nikita on 03.10.2026.
//

#include "../bt/player_chase.h"

#include <stdlib.h>

#include <mcge/mcge.h>
#include "blackboard.h"


struct player_chase {
    struct bt_node base;
    double speed;
};

static void player_chase_on_create(struct bt_node *base, struct fields *fields);
static enum bt_status player_chase_run(struct bt_node *base, void *bb);

const struct bt_node_vtable player_chase_vtable = {
    .key = BT_NODE_PLAYER_CHASE,
    .size = sizeof(struct player_chase),
    .on_create = player_chase_on_create,
    .run = player_chase_run
};

static void player_chase_on_create(struct bt_node *base, struct fields *fields) {
    struct player_chase* this = (struct player_chase*)base;
    this->speed = fields_get_double(fields, "speed", 0);
}

static enum bt_status player_chase_run(struct bt_node *base, void *bb) {
    const struct player_chase* this = (struct player_chase*)base;
    const struct blackboard* blackboard = bb;

    const struct entity *self = blackboard->self;
    const struct vector2 self_position = entity_get_rect(self).position;

    const struct player *target = blackboard->target_player;
    if (target == NULL) {
        return bt_failed;
    }
    const struct vector2 target_position = component_get_rect((const struct component*)target).position;

    struct rigid_body *rigid_body = (struct rigid_body*)entity_get_component(self, "rigid_body", entity_query_local);
    if (rigid_body == NULL) {
        return bt_failed;
    }

    const struct vector2 direction = vector_sub(target_position, self_position);

    rigid_body_drive(rigid_body, vector_multiply_scalar(vector_normalize(direction), this->speed));
    return bt_in_progress;
}