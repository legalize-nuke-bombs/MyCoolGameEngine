//
// Created by Nikita on 03.10.2026.
//

#include "bt_node_factory.h"

#include <stdlib.h>


#include "../../factories/factory_internal.h"
#include "core/bt_selector.h"
#include "core/bt_sequence.h"
#include "../../demo/bt/player_is_near.h"
#include "../../demo/bt/player_chase.h"

struct bt_node_factory {
    struct factory base;
    struct dictionary* dict;
};

static const struct factory_vtable bt_node_factory_vtable = {
    .key = "bt_node_factory"
};

static void bt_node_factory_register_all(const struct factory *base) {
    // Core
    factory_register(base, BT_NODE_SEQUENCE, bt_sequence_parse);
    factory_register(base, BT_NODE_SELECTOR, bt_selector_parse);

    // Demo
    factory_register(base, BT_NODE_PLAYER_IS_NEAR, player_is_near_parse);
    factory_register(base, BT_NODE_PLAYER_CHASE, player_chase_parse);
}

struct factory* bt_node_factory_create() {
    struct bt_node_factory* this = calloc(1, sizeof(struct bt_node_factory));
    struct factory* base = (struct factory*)this;
    factory_base_create(base, &bt_node_factory_vtable);
    bt_node_factory_register_all(base);
    return base;
}

struct bt_node* bt_node_factory_produce(const struct bt_node_factory *this, const char *key, struct parser *parser) {
    struct bt_node* (*constructor)(struct parser *parser, const struct bt_node_factory *factory) = factory_get_constructor((struct factory*)this, key);
    if (constructor == NULL) {
        return NULL;
    }
    return constructor(parser, this);
}