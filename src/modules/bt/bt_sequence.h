//
// Created by Nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_BT_SEQUENCE_H
#define MYCOOLGAMEENGINE_BT_SEQUENCE_H

struct bt_sequence;
struct bt_node;

struct bt_sequence* bt_sequence_create();

void bt_sequence_capture_node(struct bt_sequence *this, struct bt_node *node);

#endif //MYCOOLGAMEENGINE_BT_SEQUENCE_H
