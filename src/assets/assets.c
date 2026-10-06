#include "assets.h"

#include <stddef.h>
#include <string.h>

#include "asset_type.h"
#include "../logging/logger.h"
#include "../rendering/renderer_layer.h"
#include "../rendering/texture.h"
#include "../modules/physics/rigid_layer.h"
#include "../modules/physics/rigid_material.h"
#include "../scene/prefabs/prefab.h"
#include "../msystems/msystem.h"
#include "../utils/list.h"


static struct {
    struct list types;
} assets;


static const struct asset_type* assets_find(const char *key) {
    if (key == NULL) {
        return NULL;
    }
    for (int i = 0; i < list_count(&assets.types); i++) {
        const struct asset_type *type = list_get(&assets.types, i);
        if (strcmp(type->key, key) == 0) {
            return type;
        }
    }
    return NULL;
}

void assets_register(const struct asset_type *type) {
    if (assets_find(type->key) != NULL) {
        logger_warn("Assets failed to register asset type %s", type->key);
        return;
    }
    list_add(&assets.types, (void*)type);
    logger_debug("Assets know asset type %s", type->key);
}

// Asset types are cleared from the last one to the first one: register a type after the types its assets point to
static void assets_on_create(void) {
    assets.types = list_create(8);
    assets_register(&texture_asset_type);
    assets_register(&renderer_layer_asset_type);
    assets_register(&rigid_layer_asset_type);
    assets_register(&rigid_material_asset_type);
    assets_register(&prefab_asset_type);
}
static void assets_on_destroy(void) {
    for (int i = list_count(&assets.types) - 1; i >= 0; i--) {
        const struct asset_type *type = list_get(&assets.types, i);
        if (type->on_destroy) {
            type->on_destroy();
        }
    }
    list_destroy(&assets.types);
}

static void assets_on_disable(void) {
    for (int i = list_count(&assets.types) - 1; i >= 0; i--) {
        const struct asset_type *type = list_get(&assets.types, i);
        if (type->on_clear) {
            type->on_clear();
        }
    }
}

const struct msystem assets_msystem = {
    .name = "assets",
    .on_create = assets_on_create,
    .on_disable = assets_on_disable,
    .on_destroy = assets_on_destroy
};

void assets_add(const char *key, struct parser *parser) {
    const struct asset_type *type = assets_find(key);
    if (type == NULL) {
        logger_warn("Assets do not know asset type `%s`", key ? key : "<null>");
        return;
    }
    type->on_add(parser);
}
