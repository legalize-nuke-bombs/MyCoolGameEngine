//
// Created by Nikita on 03.10.2026.
//

#include "bt_graph.h"

#include <stdlib.h>

#include "bt_node_factory.h"
#include "bt_node.h"
#include "../../assets/asset_storage.h"
#include "../../assets/asset_type.h"
#include "../../utils/parser.h"


struct bt_graph {
    struct bt_node *root;
};


static void bt_graph_destroy_item(void *item) {
    bt_graph_destroy(item);
}

static struct asset_storage bt_graphs = {
    ._key = "bt_graph",
    ._destroy_item = bt_graph_destroy_item
};

static void bt_graph_asset_on_add(struct parser *parser) {
    char *name = asset_storage_parse_name(&bt_graphs, parser);
    if (name == NULL) {
        return;
    }
    const char* type = parser_next(parser);
    struct bt_node* node = bt_node_factory_produce(type, parser);
    asset_storage_add(&bt_graphs, name, bt_graph_create(node));
}
static void bt_graph_asset_on_clear(void) {
    asset_storage_clear(&bt_graphs);
}
static void bt_graph_asset_on_destroy(void) {
    asset_storage_destroy(&bt_graphs);
}

const struct asset_type bt_graph_asset_type = {
    .key = "bt_graph",
    .on_add = bt_graph_asset_on_add,
    .on_clear = bt_graph_asset_on_clear,
    .on_destroy = bt_graph_asset_on_destroy
};

struct bt_graph* bt_graph_asset_get(const char *name) {
    return asset_storage_get(&bt_graphs, name);
}


struct bt_graph* bt_graph_create(struct bt_node *root) {
    struct bt_graph* this = calloc(1, sizeof(struct bt_graph));
    this->root = root;
    return this;
}
void bt_graph_destroy(struct bt_graph *this) {
    if (this->root) bt_node_destroy(this->root);
    free(this);
}

struct bt_node* bt_graph_root(const struct bt_graph *this) {
    return this->root;
}