//
// Created by Nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_BT_NODE_H
#define MYCOOLGAMEENGINE_BT_NODE_H

#include "bt_status.h"
#include "../../api.h"

struct bt_node;

MCGE_API const char* bt_node_key(const struct bt_node *this);

MCGE_API enum bt_status bt_node_run(struct bt_node *this, void *bb);

MCGE_API void bt_node_destroy(struct bt_node *this);

#endif //MYCOOLGAMEENGINE_BT_NODE_H
