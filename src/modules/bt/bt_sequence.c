//
// Created by Nikita on 03.10.2026.
//

#include "bt_sequence.h"

#include <stdlib.h>

#include "bt_node.h"
#include "bt_node_internal.h"
#include "../../utils/list.h"


struct bt_sequence {
    struct bt_node base;
    struct list nodes;
};


static enum bt_status bt_sequence_run(struct bt_node *base, void* bb);
static void bt_sequence_on_destroy(struct bt_node *base);


struct bt_node_vtable bt_sequence_vtable = {
    .key = "sequence",
    .run = bt_sequence_run,
    .on_destroy = bt_sequence_on_destroy
};


struct bt_sequence* bt_sequence_create() {
    struct bt_sequence* this = calloc(1, sizeof(struct bt_sequence));
    struct bt_node* base = (struct bt_node*)this;
    bt_node_base_create(base, &bt_sequence_vtable);
    this->nodes = list_create(1);
    return this;
}

static void bt_sequence_on_destroy(struct bt_node *base) {
    struct bt_sequence* this = (struct bt_sequence*)base;
    for (int i = 0; i < list_count(&this->nodes); i++) {
        struct bt_node* node = list_get(&this->nodes, i);
        bt_node_destroy(node);
    }
    list_destroy(&this->nodes);
}

void bt_sequence_capture_node(struct bt_sequence *this, struct bt_node *node) {
    list_add(&this->nodes, node);
}

static enum bt_status bt_sequence_run(struct bt_node *base, void* bb) {
    const struct bt_sequence* this = (struct bt_sequence*)base;
    for (int i = 0; i < list_count(&this->nodes); i++) {
        struct bt_node* node = list_get(&this->nodes, i);
        const enum bt_status status = bt_node_run(node, bb);
        if (status != bt_ok) {
            return status;
        }
    }
    return bt_ok;
}
