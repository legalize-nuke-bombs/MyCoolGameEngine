#include "renderer_layers.h"

#include <string.h>

#include "renderer_layer.h"
#include "../assets/asset_storage.h"
#include "../assets/asset_type.h"
#include "../logging/logger.h"
#include "../utils/fields.h"

static void renderer_layers_destroy_item(void *item) {
    renderer_layer_destroy(item);
}

static struct asset_storage renderer_layers = {
    ._key = "renderer_layer",
    ._destroy_item = renderer_layers_destroy_item
};

static void renderer_layers_on_add(struct fields *fields) {
    const char *name = fields_get_string(fields, "name", NULL);
    const int priority = fields_get_int(fields, "priority", 0);
    if (priority < 0 || priority > UINT8_MAX) {
        logger_warn("Renderer layer %s expected priority from 0 to %d, got %d", name ? name : "<null>", UINT8_MAX, priority);
    }
    asset_storage_add(&renderer_layers, name, renderer_layer_create(strdup(name ? name : ""), (uint8_t)priority));
}
static void renderer_layers_on_clear(void) {
    asset_storage_clear(&renderer_layers);
}
static void renderer_layers_on_destroy(void) {
    asset_storage_destroy(&renderer_layers);
}

const struct asset_type renderer_layers_asset_type = {
    .key = "renderer_layer",
    .on_add = renderer_layers_on_add,
    .on_clear = renderer_layers_on_clear,
    .on_destroy = renderer_layers_on_destroy
};

struct renderer_layer* renderer_layers_try_get(const char *name) {
    return asset_storage_try_get(&renderer_layers, name);
}
