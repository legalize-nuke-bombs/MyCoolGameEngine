//
// Created by Nikita on 03.10.2026.
//

#include "bt_selector.h"

#include <stdlib.h>

#include "../bt_node.h"
#include "../bt_node_internal.h"
#include "../../../utils/list.h"


struct bt_selector {
    struct bt_node base;
    struct list nodes;
};


static enum bt_status bt_selector_run(struct bt_node *base, void* bb);
static void bt_selector_on_destroy(struct bt_node *base);


struct bt_node_vtable bt_selector_vtable = {
    .key = "selector",
    .run = bt_selector_run,
    .on_destroy = bt_selector_on_destroy
};


struct bt_selector* bt_selector_create() {
    struct bt_selector* this = calloc(1, sizeof(struct bt_selector));
    struct bt_node* base = (struct bt_node*)this;
    bt_node_base_create(base, &bt_selector_vtable);
    this->nodes = list_create(1);
    return this;
}

static void bt_selector_on_destroy(struct bt_node *base) {
    struct bt_selector* this = (struct bt_selector*)base;
    for (int i = 0; i < list_count(&this->nodes); i++) {
        struct bt_node* node = list_get(&this->nodes, i);
        bt_node_destroy(node);
    }
    list_destroy(&this->nodes);
}

void bt_selector_capture_node(struct bt_selector *this, struct bt_node *node) {
    list_add(&this->nodes, node);
}

static enum bt_status bt_selector_run(struct bt_node *base, void* bb) {
    const struct bt_selector* this = (struct bt_selector*)base;
    for (int i = 0; i < list_count(&this->nodes); i++) {
        struct bt_node* node = list_get(&this->nodes, i);
        const enum bt_status status = bt_node_run(node, bb);
        if (status != bt_failed) {
            return status;
        }
    }
    return bt_failed;
}