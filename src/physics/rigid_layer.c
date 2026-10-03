//
// Created by nikita on 01.10.2026.
//

#include "rigid_layer.h"

#include <stdlib.h>

#include "../catalogs/catalog.h"
#include "../utils/parser.h"

struct rigid_layer {
    uint8_t priority;
};

static const char* rigid_layer_catalog_key(void) {
    return "rigid_layer";
}

static void* rigid_layer_on_create_item(const char *name, struct parser *parser, const struct subsystem_collection *subsystems) {
    struct rigid_layer* this = calloc(1, sizeof(struct rigid_layer));
    parser_next_uint8(parser, &this->priority);
    return this;
}
static void rigid_layer_on_destroy_item(void *item) {
    free(item);
}

const struct catalog_vtable rigid_layer_catalog_vtable = {
    .key = rigid_layer_catalog_key,
    .on_create_item = rigid_layer_on_create_item,
    .on_destroy_item = rigid_layer_on_destroy_item
};

uint8_t rigid_layer_get_priority(const struct rigid_layer* this) {
    return this->priority;
}
