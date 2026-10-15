//
// Created by Nikita on 03.10.2026.
//

#include "bt_graph.h"

#include <stdlib.h>

#include "bt_node.h"
#include "mcge/allocator/allocator.h"


struct bt_graph {
    struct bt_node *root;
};


struct bt_graph* bt_graph_create(struct bt_node *root) {
    struct bt_graph* this = allocator_new0(1, sizeof(struct bt_graph));
    this->root = root;
    return this;
}
void bt_graph_destroy(struct bt_graph *this) {
    if (this->root) bt_node_destroy(this->root);
    allocator_delete(this);
}

struct bt_node* bt_graph_root(const struct bt_graph *this) {
    return this->root;
}
