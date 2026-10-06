#include "bt_graphs.h"

#include <stddef.h>

#include "bt_graph.h"
#include "bt_node_factory.h"
#include "../../assets/asset_storage.h"
#include "../../assets/asset_type.h"
#include "../../logging/logger.h"
#include "../../utils/fields.h"

static void bt_graphs_destroy_item(void *item) {
    bt_graph_destroy(item);
}

static struct asset_storage bt_graphs = {
    ._key = "bt_graph",
    ._destroy_item = bt_graphs_destroy_item
};

static void bt_graphs_on_add(struct fields *fields) {
    const char *name = fields_get_string(fields, "name", NULL);

    const struct fields_list *root = fields_get_list(fields, "root");
    struct bt_node* node = NULL;
    if (fields_list_count(root) == 1) {
        node = bt_node_factory_produce(fields_list_get(root, 0));
    }
    else {
        logger_warn("Bt graph %s expected one node in field `root`, got %d", name ? name : "<null>", fields_list_count(root));
    }
    asset_storage_add(&bt_graphs, name, bt_graph_create(node));
}
static void bt_graphs_on_clear(void) {
    asset_storage_clear(&bt_graphs);
}
static void bt_graphs_on_destroy(void) {
    asset_storage_destroy(&bt_graphs);
}

const struct asset_type bt_graphs_asset_type = {
    .key = "bt_graph",
    .on_add = bt_graphs_on_add,
    .on_clear = bt_graphs_on_clear,
    .on_destroy = bt_graphs_on_destroy
};

struct bt_graph* bt_graphs_get(const char *name) {
    return asset_storage_get(&bt_graphs, name);
}
