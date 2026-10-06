//
// Created by Nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_BT_NODE_FACTORY_H
#define MYCOOLGAMEENGINE_BT_NODE_FACTORY_H

struct bt_node;
struct parser;

void bt_node_factory_create(void);
void bt_node_factory_destroy(void);

void bt_node_factory_register(const char *key, struct bt_node* (*parse)(struct parser *parser));
struct bt_node* bt_node_factory_produce(const char *key, struct parser *parser);

#endif //MYCOOLGAMEENGINE_BT_NODE_FACTORY_H
