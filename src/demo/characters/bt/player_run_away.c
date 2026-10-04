//
// Created by Nikita on 04.10.2026.
//

#include "player_run_away.h"

#include <stdlib.h>

#include "../../../modules/bt/bt_node_internal.h"
#include "../../../utils/parser.h"
#include "../../../modules/bt/bt_node.h"
#include "blackboard.h"
#include "../../../scene/entity.h"
#include "../../../utils/vector2_math.h"
#include "../../../scene/components/physics/rigid_body.h"


struct player_run_away {
    struct bt_node base;
    double speed;
};

static enum bt_status player_run_away_run(struct bt_node *base, void *bb);

static const struct bt_node_vtable player_run_away_vtable = {
    .key = BT_NODE_PLAYER_RUN_AWAY,
    .run = player_run_away_run
};

struct bt_node* player_run_away_create(const double speed) {
    struct player_run_away* this = calloc(1, sizeof(struct player_run_away));
    struct bt_node* base = (struct bt_node*)this;
    bt_node_base_create(base, &player_run_away_vtable);
    this->speed = speed;
    return base;
}

struct bt_node* player_run_away_parse(struct parser *parser, const struct bt_node_factory *factory) {
    double speed;
    parser_next_double(parser, &speed);
    return player_run_away_create(speed);
}

static enum bt_status player_run_away_run(struct bt_node *base, void *bb) {
    const struct player_run_away* this = (struct player_run_away*)base;
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

    const struct vector2 direction = vector_multiply_scalar(vector_sub(target_position, self_position), -1);

    const struct vector2 v_target = vector_multiply_scalar(vector_normalize(direction), this->speed);
    const struct vector2 delta_v = vector_sub(v_target, rigid_body_get_velocity(rigid_body));
    rigid_body_drive(rigid_body, vector_multiply_scalar(delta_v, rigid_body_get_mass(rigid_body)));
    return bt_in_progress;
}