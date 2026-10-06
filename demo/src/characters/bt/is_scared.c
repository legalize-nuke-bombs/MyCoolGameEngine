//
// Created by Nikita on 04.10.2026.
//

#include "is_scared.h"

#include <stdlib.h>

#include <mcge/mcge.h>
#include "blackboard.h"
#include "../effects.h"

struct is_scared {
    struct bt_node base;
};

static enum bt_status is_scared_run(struct bt_node *base, void *bb);

const struct bt_node_vtable is_scared_vtable = {
    .key = BT_NODE_IS_SCARED,
    .size = sizeof(struct is_scared),
    .run = is_scared_run
};

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