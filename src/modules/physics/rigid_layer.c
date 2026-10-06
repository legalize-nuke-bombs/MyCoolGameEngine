//
// Created by nikita on 01.10.2026.
//

#include "rigid_layer.h"

#include <stdlib.h>

#include "../../assets/asset_storage.h"
#include "../../assets/asset_type.h"
#include "../../utils/parser.h"

struct rigid_layer {
    uint8_t priority;
};

static void rigid_layer_destroy_item(void *item) {
    free(item);
}

static struct asset_storage rigid_layers = {
    ._key = "rigid_layer",
    ._destroy_item = rigid_layer_destroy_item
};

static void rigid_layer_asset_on_add(struct parser *parser) {
    char *name = asset_storage_parse_name(&rigid_layers, parser);
    if (name == NULL) {
        return;
    }
    struct rigid_layer* this = calloc(1, sizeof(struct rigid_layer));
    parser_next_uint8(parser, &this->priority);
    asset_storage_add(&rigid_layers, name, this);
}
static void rigid_layer_asset_on_clear(void) {
    asset_storage_clear(&rigid_layers);
}
static void rigid_layer_asset_on_destroy(void) {
    asset_storage_destroy(&rigid_layers);
}

const struct asset_type rigid_layer_asset_type = {
    .key = "rigid_layer",
    .on_add = rigid_layer_asset_on_add,
    .on_clear = rigid_layer_asset_on_clear,
    .on_destroy = rigid_layer_asset_on_destroy
};

struct rigid_layer* rigid_layer_asset_get(const char *name) {
    return asset_storage_get(&rigid_layers, name);
}

uint8_t rigid_layer_get_priority(const struct rigid_layer* this) {
    return this->priority;
}
