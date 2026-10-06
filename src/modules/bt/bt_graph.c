//
// Created by Nikita on 03.10.2026.
//

#include "bt_graph.h"

#include <stdlib.h>

#include "bt_node_factory.h"
#include "../../factories/factories.h"
#include "bt_node.h"
#include "../../catalogs/catalog.h"
#include "../../utils/parser.h"


struct bt_graph {
    struct bt_node *root;
};


static const char* bt_graph_catalog_key(void) {
    return "bt_graph";
}

static void* bt_graph_on_create_item(const char *name, struct parser *parser) {
    const char* type = parser_next(parser);
    const struct bt_node_factory* factory = (struct bt_node_factory*)factories_get("bt_node_factory");
    struct bt_node* node = bt_node_factory_produce(factory, type, parser);
    return bt_graph_create(node);
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