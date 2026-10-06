//
// Created by Nikita on 03.10.2026.
//

#include "bt_selector.h"

#include <stdlib.h>

#include "../bt_node.h"
#include "../bt_node_factory.h"
#include "../bt_node_internal.h"
#include "../../../utils/list.h"
#include "../../../utils/fields.h"


struct bt_selector {
    struct bt_node base;
    struct list nodes;
};


static void bt_selector_on_create(struct bt_node *base, struct fields *fields);
static enum bt_status bt_selector_run(struct bt_node *base, void* bb);
static void bt_selector_on_destroy(struct bt_node *base);


const struct bt_node_vtable bt_selector_vtable = {
    .key = BT_NODE_SELECTOR,
    .size = sizeof(struct bt_selector),
    .on_create = bt_selector_on_create,
    .run = bt_selector_run,
    .on_destroy = bt_selector_on_destroy
};


static void bt_selector_on_create(struct bt_node *base, struct fields *fields) {
    struct bt_selector* this = (struct bt_selector*)base;
    this->nodes = list_create(1);

    const struct fields_list *nodes = fields_get_list(fields, "nodes");
    for (int i = 0; i < fields_list_count(nodes); i++) {
        struct bt_node* node = bt_node_factory_produce(fields_list_get(nodes, i));
        if (node != NULL) {
            list_add(&this->nodes, node);
        }
    }
}

static void bt_selector_on_destroy(struct bt_node *base) {
    struct bt_selector* this = (struct bt_selector*)base;
    for (int i = 0; i < list_count(&this->nodes); i++) {
        struct bt_node* node = list_get(&this->nodes, i);
        bt_node_destroy(node);
    }
    list_destroy(&this->nodes);
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
