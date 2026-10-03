//
// Created by Nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_BT_MODULE_H
#define MYCOOLGAMEENGINE_BT_MODULE_H

struct subsystem_collection;
struct bt_module;

struct subsystem* bt_module_create(const struct subsystem_collection* subsystems);

struct bt_node_factory* bt_module_node_factory(const struct bt_module *this);

#endif //MYCOOLGAMEENGINE_BT_MODULE_H
