//
// Created by Nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_BT_GRAPH_H
#define MYCOOLGAMEENGINE_BT_GRAPH_H

#include "../../api.h"

struct bt_graph;
struct bt_node;

MCGE_API struct bt_graph* bt_graph_create(struct bt_node *root);
MCGE_API void bt_graph_destroy(struct bt_graph *this);

MCGE_API struct bt_node* bt_graph_root(const struct bt_graph *this);

#endif //MYCOOLGAMEENGINE_BT_GRAPH_H
