//
// Created by Nikita on 04.10.2026.
//

#ifndef MYCOOLGAMEENGINE_IS_SCARED_H
#define MYCOOLGAMEENGINE_IS_SCARED_H

struct parser;
struct bt_node_factory;

#define BT_NODE_IS_SCARED "is_scared"

struct bt_node* is_scared_create();
struct bt_node* is_scared_parse(struct parser *parser, const struct bt_node_factory *factory);

#endif //MYCOOLGAMEENGINE_IS_SCARED_H
