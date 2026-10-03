//
// Created by Nikita on 03.10.2026.
//

#include "bt_node.h"

#include <stdlib.h>

#include "bt_node_internal.h"


enum bt_status bt_node_run(struct bt_node *this, void *bb) {
    return this->vtable->run(this, bb);
}

void bt_node_base_create(struct bt_node *this, const struct bt_node_vtable *vtable) {
    this->vtable = vtable;
}

void bt_node_destroy(struct bt_node *this) {
    if (this->vtable->on_destroy) {
        this->vtable->on_destroy(this);
    }
    free(this);
}