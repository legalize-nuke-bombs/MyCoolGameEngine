//
// Created by Nikita on 03.10.2026.
//

#include "bt_module.h"

#include <stdlib.h>

#include "bt_node_factory.h"
#include "../../subsystems/subsystem_internal.h"


static const char* bt_module_get_name() {
    return "bt_module";
}

static void bt_module_on_destroy(struct subsystem* base);

static struct subsystem_vtable engine_lifecycle_vtable = {
    .name = bt_module_get_name,
    .on_destroy = bt_module_on_destroy
};


struct bt_module {
    struct subsystem base;
    struct bt_node_factory *node_factory;
};


struct subsystem* bt_module_create(const struct subsystem_collection* collections) {
    struct bt_module* this = calloc(1, sizeof(struct bt_module));
    struct subsystem* base = (struct subsystem*)this;
    subsystem_create(base, &engine_lifecycle_vtable, collections);
    this->node_factory = bt_node_factory_create();
    return base;
}

void bt_module_on_destroy(struct subsystem* base) {
    const struct bt_module* this = (struct bt_module*)base;
    bt_node_factory_destroy(this->node_factory);
}

struct bt_node_factory* bt_module_node_factory(const struct bt_module *this) {
    return this->node_factory;
}