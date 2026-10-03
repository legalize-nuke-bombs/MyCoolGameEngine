//
// Created by nikita on 03.10.2026.
//

#include "../bt/player_chase.h"

#include <stdlib.h>

#include "../../../modules/bt/bt_node_internal.h"
#include "../../../modules/bt/bt_node.h"
#include "blackboard.h"
#include "../../../scene/entity.h"
#include "../../../scene/scene.h"
#include "../../../scene/components/physics/rigid_body.h"
#include "../../../utils/parser.h"
#include "../../../utils/vector2_math.h"


struct player_chase {
    struct bt_node base;
    double speed;
};

static enum bt_status player_chase_run(struct bt_node *base, void *bb);

static const struct bt_node_vtable player_chase_vtable = {
    .key = BT_NODE_PLAYER_CHASE,
    .run = player_chase_run
};

struct bt_node* player_chase_create(const double speed) {
    struct player_chase* this = calloc(1, sizeof(struct player_chase));
    struct bt_node* base = (struct bt_node*)this;
    bt_node_base_create(base, &player_chase_vtable);
    this->speed = speed;
    return base;
}

struct bt_node* player_chase_parse(struct parser *parser, const struct bt_node_factory *factory) {
    double speed;
    parser_next_double(parser, &speed);
    return player_chase_create(speed);
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

    struct rigid_body *rigid_body = (struct rigid_body*)entity_get_component(self, "rigid_body");
    if (rigid_body == NULL) {
        return bt_failed;
    }

    const struct vector2 direction = vector_sub(target_position, self_position);

    const struct vector2 v_target = vector_multiply_scalar(vector_normalize(direction), this->speed);
    const struct vector2 delta_v = vector_sub(v_target, rigid_body_get_velocity(rigid_body));
    rigid_body_drive(rigid_body, vector_multiply_scalar(delta_v, rigid_body_get_mass(rigid_body)));
    return bt_in_progress;
}