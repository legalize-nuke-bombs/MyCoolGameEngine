//
// Created by Nikita on 03.10.2026.
//

#include "bt_sequence.h"

#include <stdlib.h>
#include <string.h>

#include "../bt_node.h"
#include "../bt_node_internal.h"
#include "../../../utils/list.h"
#include "../bt_node_factory.h"
#include "../../../utils/parser.h"


struct bt_sequence {
    struct bt_node base;
    struct list nodes;
};


static enum bt_status bt_sequence_run(struct bt_node *base, void* bb);
static void bt_sequence_on_destroy(struct bt_node *base);


struct bt_node_vtable bt_sequence_vtable = {
    .key = BT_NODE_SEQUENCE,
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
struct bt_node* bt_sequence_parse(struct parser *parser, const struct bt_node_factory *factory) {
    struct bt_sequence* this = bt_sequence_create();
    for (; ;) {
        const char* word = parser_next(parser);
        if (word == NULL || strcmp(word, "end") == 0) {
            break;
        }
        struct bt_node* node = bt_node_factory_try_produce_node(factory, word, parser);
        if (node == NULL) {
            break;
        }
        bt_sequence_capture_node(this, node);
    }
    return (struct bt_node*)this;
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
