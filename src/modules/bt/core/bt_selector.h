//
// Created by Nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_BT_SELECTOR_H
#define MYCOOLGAMEENGINE_BT_SELECTOR_H

struct bt_selector;
struct bt_node;
struct parser;

#define BT_NODE_SELECTOR "selector"

struct bt_selector* bt_selector_create();
struct bt_node* bt_selector_parse(struct parser *parser);

void bt_selector_capture_node(struct bt_selector *this, struct bt_node *node);

#endif //MYCOOLGAMEENGINE_BT_SELECTOR_H
