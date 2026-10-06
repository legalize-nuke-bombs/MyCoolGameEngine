//
// Created by nikita on 24.09.2026.
//

#include "renderer_layer.h"

#include <stdlib.h>
#include <string.h>

#include "../assets/asset_storage.h"
#include "../assets/asset_type.h"
#include "../logging/logger.h"
#include "../utils/fields.h"


struct renderer_layer {
    char *name;
    int priority;
};


static void renderer_layer_destroy_item(void *item) {
    renderer_layer_destroy(item);
}

static struct asset_storage renderer_layers = {
    ._key = "renderer_layer",
    ._destroy_item = renderer_layer_destroy_item
};

static void renderer_layer_asset_on_add(struct fields *fields) {
    const char *name = fields_get_string(fields, "name", NULL);
    const int priority = fields_get_int(fields, "priority", 0);
    if (priority < 0 || priority > UINT8_MAX) {
        logger_warn("Renderer layer %s expected priority from 0 to %d, got %d", name ? name : "<null>", UINT8_MAX, priority);
    }
    asset_storage_add(&renderer_layers, name, renderer_layer_create(strdup(name ? name : ""), (uint8_t)priority));
}
static void renderer_layer_asset_on_clear(void) {
    asset_storage_clear(&renderer_layers);
}
static void renderer_layer_asset_on_destroy(void) {
    asset_storage_destroy(&renderer_layers);
}

const struct asset_type renderer_layer_asset_type = {
    .key = "renderer_layer",
    .on_add = renderer_layer_asset_on_add,
    .on_clear = renderer_layer_asset_on_clear,
    .on_destroy = renderer_layer_asset_on_destroy
};

struct renderer_layer* renderer_layer_asset_try_get(const char *name) {
    return asset_storage_try_get(&renderer_layers, name);
}


struct renderer_layer* renderer_layer_create(char *name, uint8_t priority) {
    struct renderer_layer *layer = malloc(sizeof(struct renderer_layer));
    layer->name = name;
    layer->priority = priority;
    return layer;
}
void renderer_layer_destroy(struct renderer_layer *layer) {
    free(layer->name);
    free(layer);
}

const char* renderer_layer_get_name(const struct renderer_layer *layer) {
    if (layer == NULL) {
        return "NULL";
    }
    return layer->name;
}
uint8_t renderer_layer_get_priority(const struct renderer_layer *layer) {
    if (layer == NULL) {
        return 0;
    }
    return layer->priority;
}