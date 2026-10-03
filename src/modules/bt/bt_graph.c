//
// Created by Nikita on 03.10.2026.
//

#include "bt_graph.h"

#include <stdlib.h>

#include "bt_module.h"
#include "bt_node.h"
#include "bt_node_factory.h"
#include "../../catalogs/catalog.h"
#include "../../subsystems/subsystem_collection.h"
#include "../../utils/parser.h"


struct bt_graph {
    struct bt_node *root;
};


static const char* bt_graph_catalog_key(void) {
    return "bt_graph";
}

static void* bt_graph_on_create_item(const char *name, struct parser *parser, const struct subsystem_collection *subsystems) {
    const char* type = parser_next(parser);
    const struct bt_node_factory* factory = bt_module_node_factory((struct bt_module*)subsystem_collection_get(subsystems, "bt_module"));
    return bt_graph_create(bt_node_factory_try_produce_node(factory, type, parser));
}
static void bt_graph_on_destroy_item(void *item) {
    bt_graph_destroy(item);
}

const struct catalog_vtable bt_graph_catalog_vtable = {
    .key = bt_graph_catalog_key,
    .on_create_item = bt_graph_on_create_item,
    .on_destroy_item = bt_graph_on_destroy_item
};


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