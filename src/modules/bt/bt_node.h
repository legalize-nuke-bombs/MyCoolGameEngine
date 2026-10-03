//
// Created by Nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_BT_NODE_H
#define MYCOOLGAMEENGINE_BT_NODE_H

#include "bt_status.h"

struct bt_node;

enum bt_status bt_node_run(struct bt_node *this, void *bb);

#endif //MYCOOLGAMEENGINE_BT_NODE_H
