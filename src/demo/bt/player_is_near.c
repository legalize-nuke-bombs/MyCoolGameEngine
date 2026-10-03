//
// Created by nikita on 03.10.2026.
//

#include "player_is_near.h"

#include <stdlib.h>

#include "../../modules/bt/bt_node_internal.h"
#include "../../modules/bt/bt_node.h"
#include "blackboard.h"
#include "../../scene/entity.h"
#include "../../scene/scene.h"
#include "../../utils/dictionary.h"
#include "../../utils/parser.h"
#include "../../utils/vector2_math.h"

struct player_is_near {
    struct bt_node base;
    double radius_sqr;
};

static enum bt_status player_is_near_run(struct bt_node *base, void *bb);

static const struct bt_node_vtable player_is_near_vtable = {
    .key = BT_NODE_PLAYER_IS_NEAR,
    .run = player_is_near_run
};

struct bt_node* player_is_near_create(const double radius) {
    struct player_is_near* this = calloc(1, sizeof(struct player_is_near));
    struct bt_node* base = (struct bt_node*)this;
    bt_node_base_create(base, &player_is_near_vtable);
    this->radius_sqr = radius * radius;
    return base;
}

struct bt_node* player_is_near_parse(struct parser *parser, const struct bt_node_factory *factory) {
    double radius;
    parser_next_double(parser, &radius);
    return player_is_near_create(radius);
}

static enum bt_status player_is_near_run(struct bt_node *base, void *bb) {
    const struct player_is_near* this = (struct player_is_near*)base;
    struct blackboard* blackboard = bb;

    const struct entity *self = blackboard->self;
    const struct vector2 position = entity_get_rect(self).position;

    const struct dictionary *players = tmap_try_get_components(scene_get_tmap(entity_get_scene(self)), "player");
    if (players == NULL) {
        return bt_failed;
    }

    struct player *result = NULL;
    double min_sqr_distance = 1e+12;

    struct dictionary_iterator iterator = dictionary_begin(players);
    struct dictionary_node node;
    while (dictionary_next(players, &iterator, &node)) {
        struct player *player = node.value;
        struct vector2 player_position = component_get_rect((struct component*)player).position;
        const double distance_sqr = vector_sqr_distance(&position, &player_position);
        if (min_sqr_distance > distance_sqr) {
            result = player;
            min_sqr_distance = distance_sqr;
        }
    }

    if (this->radius_sqr > min_sqr_distance) {
        blackboard->target_player = result;
        return bt_ok;
    }
    blackboard->target_player = NULL;
    return bt_failed;
}