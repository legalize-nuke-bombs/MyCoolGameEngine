#include "collision_layers.h"

#include <string.h>

#include "collision_layer.h"
#include "../../assets/asset_storage.h"
#include "../../assets/asset_type.h"
#include "../../logging/logger.h"
#include "../../utils/fields.h"

#define COLLISION_LAYERS_DEFAULT_NAME "default"

static void collision_layers_destroy_item(void *item) {
    collision_layer_destroy(item);
}

static struct {
    struct asset_storage storage;
    int count;
} collision_layers = {
    .storage = {
        ._key = "collision_layer",
        ._destroy_item = collision_layers_destroy_item
    },
    .count = 1
};

static void collision_layers_on_add(struct fields *fields) {
    const char *name = fields_get_string(fields, "name", NULL);
    const char *default_name = fields_get_string(fields, "default", "block");

    enum collision_response default_response = collision_response_block;
    if (!collision_response_try_parse(default_name, &default_response)) {
        logger_warn("Collision layer %s got unexpected default `%s`, `block` will be used instead", name ? name : "<null>", default_name);
    }
    if (name != NULL && strcmp(name, COLLISION_LAYERS_DEFAULT_NAME) == 0) {
        logger_warn("Collision layer `%s` is built in, it can not be declared again", COLLISION_LAYERS_DEFAULT_NAME);
        return;
    }
    if (collision_layers.count >= COLLISION_LAYERS_MAX) {
        logger_warn("Collision layer %s does not fit: there can be at most %d layers", name ? name : "<null>", COLLISION_LAYERS_MAX);
        return;
    }
    asset_storage_add(&collision_layers.storage, name, collision_layer_create((uint8_t)collision_layers.count, default_response));
    collision_layers.count++;
}
static void collision_layers_on_clear(void) {
    asset_storage_clear(&collision_layers.storage);
    collision_layers.count = 1;
}
static void collision_layers_on_destroy(void) {
    asset_storage_destroy(&collision_layers.storage);
}

const struct asset_type collision_layers_asset_type = {
    .key = "collision_layer",
    .on_add = collision_layers_on_add,
    .on_clear = collision_layers_on_clear,
    .on_destroy = collision_layers_on_destroy
};

const struct collision_layer* collision_layers_get(const char *name) {
    if (name != NULL && strcmp(name, COLLISION_LAYERS_DEFAULT_NAME) == 0) {
        return &collision_layer_default;
    }
    return asset_storage_get(&collision_layers.storage, name);
}
