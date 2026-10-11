#include "textures.h"

#include <string.h>

#include "renderer.h"
#include "texture.h"
#include "../assets/asset_storage.h"
#include "../assets/asset_type.h"
#include "../logging/logger.h"
#include "../utils/fields.h"

static void textures_destroy_item(void *item) {
    texture_destroy(item);
}

static struct asset_storage textures = {
    ._key = "texture",
    ._destroy_item = textures_destroy_item
};

static void textures_on_add(struct fields *fields) {
    const char *name = fields_get_string(fields, "name", NULL);
    const struct vector2 tile = fields_get_vector2(fields, "tile", vector2_zero);
    const int tiles_count = fields_get_int(fields, "count", 1);
    const char *loading_mode_name = fields_get_string(fields, "load", "lazy");

    enum texture_loading_mode loading_mode = texture_loading_mode_lazy;
    if (strcmp(loading_mode_name, "eager") == 0) {
        loading_mode = texture_loading_mode_eager;
    }
    else if (strcmp(loading_mode_name, "lazy") != 0) {
        logger_warn("Unexpected texture loading mode `%s`, `lazy` will be used instead", loading_mode_name);
    }

    asset_storage_add(&textures, name, texture_create(strdup(name ? name : ""), fields_dup_string(fields, "path", ""), (int)tile.x, (int)tile.y, tiles_count, loading_mode, renderer_get_native_renderer()));
}
static void textures_on_clear(void) {
    asset_storage_clear(&textures);
}
static void textures_on_destroy(void) {
    asset_storage_destroy(&textures);
}

const struct asset_type textures_asset_type = {
    .key = "texture",
    .on_add = textures_on_add,
    .on_clear = textures_on_clear,
    .on_destroy = textures_on_destroy
};

struct texture* textures_get(const char *name) {
    return asset_storage_get(&textures, name);
}
