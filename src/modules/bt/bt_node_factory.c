//
// Created by Nikita on 03.10.2026.
//

#include "bt_node_factory.h"

#include <stdlib.h>

#include "bt_node_internal.h"
#include "../../utils/factory.h"
#include "../../utils/fields.h"
#include "core/bt_selector.h"
#include "core/bt_sequence.h"

static struct factory bt_node_factory;

void bt_node_factory_create(void) {
    bt_node_factory = factory_create("bt_node_factory");

    bt_node_factory_register(&bt_sequence_vtable);
    bt_node_factory_register(&bt_selector_vtable);
}
void bt_node_factory_destroy(void) {
    factory_destroy(&bt_node_factory);
}

void bt_node_factory_register(const struct bt_node_vtable *vtable) {
    factory_register(&bt_node_factory, vtable->key, (void*)vtable);
}

struct bt_node* bt_node_factory_produce(struct fields *fields) {
    const struct bt_node_vtable *vtable = factory_find(&bt_node_factory, fields_key(fields));
    if (vtable == NULL) {
        return NULL;
    }
    struct bt_node *node = calloc(1, vtable->size);
    bt_node_base_create(node, vtable);
    if (vtable->on_create) {
        vtable->on_create(node, fields);
    }
    fields_warn_unknown(fields);
    return node;
}
