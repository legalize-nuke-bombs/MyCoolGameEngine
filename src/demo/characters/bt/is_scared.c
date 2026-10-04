//
// Created by Nikita on 04.10.2026.
//

#include "is_scared.h"

#include <stdlib.h>

#include "../../../modules/bt/bt_node.h"
#include "../../../modules/bt/bt_node_internal.h"
#include "blackboard.h"
#include "../effects.h"
#include "../../../scene/entity.h"

struct is_scared {
    struct bt_node base;
};

static enum bt_status is_scared_run(struct bt_node *base, void *bb);

static const struct bt_node_vtable is_scared_vtable = {
    .key = BT_NODE_IS_SCARED,
    .run = is_scared_run
};

struct bt_node* is_scared_create() {
    struct is_scared* this = calloc(1, sizeof(struct is_scared));
    struct bt_node* base = (struct bt_node*)this;
    bt_node_base_create(base, &is_scared_vtable);
    return base;
}

struct bt_node* is_scared_parse(struct parser *parser, const struct bt_node_factory *factory) {
    return is_scared_create();
}

static enum bt_status is_scared_run(struct bt_node *base, void *bb) {
    const struct player_run_away* this = (struct player_run_away*)base;
    const struct blackboard* blackboard = bb;

    const struct entity *self = blackboard->self;
    const struct effects* effects = (struct effects*)entity_get_component(self, "effects", entity_query_local);
    if (effects == NULL) {
        return bt_failed;
    }

    return effects_has_effect(effects, effect_fear) ? bt_ok : bt_failed;
}