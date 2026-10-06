#include "rigid_layers.h"

#include "rigid_layer.h"
#include "../../assets/asset_storage.h"
#include "../../assets/asset_type.h"
#include "../../logging/logger.h"
#include "../../utils/fields.h"

static void rigid_layers_destroy_item(void *item) {
    rigid_layer_destroy(item);
}

static struct asset_storage rigid_layers = {
    ._key = "rigid_layer",
    ._destroy_item = rigid_layers_destroy_item
};

static void rigid_layers_on_add(struct fields *fields) {
    const char *name = fields_get_string(fields, "name", NULL);
    const int priority = fields_get_int(fields, "priority", 0);
    if (priority < 0 || priority > UINT8_MAX) {
        logger_warn("Rigid layer %s expected priority from 0 to %d, got %d", name ? name : "<null>", UINT8_MAX, priority);
    }
    asset_storage_add(&rigid_layers, name, rigid_layer_create((uint8_t)priority));
}
static void rigid_layers_on_clear(void) {
    asset_storage_clear(&rigid_layers);
}
static void rigid_layers_on_destroy(void) {
    asset_storage_destroy(&rigid_layers);
}

const struct asset_type rigid_layers_asset_type = {
    .key = "rigid_layer",
    .on_add = rigid_layers_on_add,
    .on_clear = rigid_layers_on_clear,
    .on_destroy = rigid_layers_on_destroy
};

struct rigid_layer* rigid_layers_get(const char *name) {
    return asset_storage_get(&rigid_layers, name);
}
