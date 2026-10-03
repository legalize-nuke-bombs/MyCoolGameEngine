//
// Created by Nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_BT_NODE_FACTORY_H
#define MYCOOLGAMEENGINE_BT_NODE_FACTORY_H

struct bt_node_factory;
struct bt_node;
struct parser;

struct factory* bt_node_factory_create();
struct bt_node* bt_node_factory_produce(const struct bt_node_factory *this, const char *key, struct parser *parser);

#endif //MYCOOLGAMEENGINE_BT_NODE_FACTORY_H
