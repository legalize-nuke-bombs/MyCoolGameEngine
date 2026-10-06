//
// Created by Nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_BT_NODE_FACTORY_H
#define MYCOOLGAMEENGINE_BT_NODE_FACTORY_H

struct bt_node;
struct bt_node_vtable;
struct fields;

void bt_node_factory_create(void);
void bt_node_factory_destroy(void);

void bt_node_factory_register(const struct bt_node_vtable *vtable);
struct bt_node* bt_node_factory_produce(struct fields *fields);

#endif //MYCOOLGAMEENGINE_BT_NODE_FACTORY_H
