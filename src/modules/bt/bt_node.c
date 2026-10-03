//
// Created by Nikita on 03.10.2026.
//

#include "bt_node.h"
#include "bt_node_internal.h"


enum bt_status bt_node_run(struct bt_node *this, void *bb) {
    return this->vtable->run(this, bb);
}