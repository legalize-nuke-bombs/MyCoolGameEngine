//
// Created by Nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_BT_SEQUENCE_H
#define MYCOOLGAMEENGINE_BT_SEQUENCE_H

#define BT_NODE_SEQUENCE "sequence"

struct bt_sequence;
struct bt_node;
struct parser;
struct bt_node_factory;

struct bt_sequence* bt_sequence_create();
struct bt_node* bt_sequence_parse(struct parser *parser, const struct bt_node_factory *factory);

void bt_sequence_capture_node(struct bt_sequence *this, struct bt_node *node);

#endif //MYCOOLGAMEENGINE_BT_SEQUENCE_H
