//
// Created by Nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_BT_GRAPH_H
#define MYCOOLGAMEENGINE_BT_GRAPH_H

struct bt_graph;
struct catalog_vtable;
struct bt_node;

extern const struct catalog_vtable bt_graph_catalog_vtable;

struct bt_graph* bt_graph_create(struct bt_node *root);
void bt_graph_destroy(struct bt_graph *this);

struct bt_node* bt_graph_root(const struct bt_graph *this);

#endif //MYCOOLGAMEENGINE_BT_GRAPH_H
