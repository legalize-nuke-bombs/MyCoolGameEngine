//
// Created by Nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_BT_NODE_INTERNAL_H
#define MYCOOLGAMEENGINE_BT_NODE_INTERNAL_H

struct bt_node_vtable {
    enum bt_status (*run)(struct bt_node *this, void *bb);
    void (*on_destroy)(struct bt_node *this);
};

struct bt_node {
    const struct bt_node_vtable *vtable;
};

void bt_node_base_create(struct bt_node *this, const struct bt_node_vtable *vtable);

#endif //MYCOOLGAMEENGINE_BT_NODE_INTERNAL_H
