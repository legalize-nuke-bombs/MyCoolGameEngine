//
// Created by Nikita on 03.10.2026.
//

#include "bt_node_factory.h"

#include <stddef.h>

#include "../../utils/factory.h"
#include "core/bt_selector.h"
#include "core/bt_sequence.h"

static struct factory bt_node_factory;

void bt_node_factory_create(void) {
    bt_node_factory = factory_create("bt_node_factory");

    bt_node_factory_register(BT_NODE_SEQUENCE, bt_sequence_parse);
    bt_node_factory_register(BT_NODE_SELECTOR, bt_selector_parse);
}
void bt_node_factory_destroy(void) {
    factory_destroy(&bt_node_factory);
}

void bt_node_factory_register(const char *key, struct bt_node* (*parse)(struct parser *parser)) {
    factory_register(&bt_node_factory, key, parse);
}

struct bt_node* bt_node_factory_produce(const char *key, struct parser *parser) {
    struct bt_node* (*parse)(struct parser *parser) = factory_find(&bt_node_factory, key);
    if (parse == NULL) {
        return NULL;
    }
    return parse(parser);
}
