//
// Created by Nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_BT_NODE_FACTORY_H
#define MYCOOLGAMEENGINE_BT_NODE_FACTORY_H

struct bt_node_factory;
struct bt_node;
struct parser;

struct bt_node_factory* bt_node_factory_create();
void bt_node_factory_destroy(struct bt_node_factory *this);

void bt_node_factory_clear(const struct bt_node_factory *this);

struct bt_node* bt_node_factory_try_produce_node(const struct bt_node_factory *this, const char *key, struct parser *parser);

#endif //MYCOOLGAMEENGINE_BT_NODE_FACTORY_H
