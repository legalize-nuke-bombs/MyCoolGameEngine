//
// Created by nikita on 14.10.2026.
//

#include "fonts.h"

#include "font.h"
#include "mcge/assets/asset_storage.h"
#include "mcge/utils/fields.h"
#include <string.h>

#include "mcge/assets/asset_type.h"
#include "mcge/logging/logger.h"


static void fonts_destroy_item(void *item) {
    font_destroy(item);
}

static struct asset_storage fonts = {
    ._key = "font",
    ._destroy_item = fonts_destroy_item
};


static void fonts_on_add(struct fields *fields) {
    char* name = fields_dup_string(fields, "name", "<null>");
    char* path = fields_dup_string(fields, "path", "<null>");
    const float size = (float)fields_get_double(fields, "size", 48);
    const char* loading_mode_name = fields_get_string(fields, "loading_mode", "lazy");

    enum font_loading_mode loading_mode = font_loading_mode_lazy;
    if (strcmp(loading_mode_name, "lazy") == 0) {
        loading_mode = font_loading_mode_lazy;
    }
    else if (strcmp(loading_mode_name, "eager") == 0) {
        loading_mode = font_loading_mode_eager;
    }
    else {
        logger_warn("Font %s unexpected loading mode %s", name, loading_mode_name);
    }

    asset_storage_add(&fonts, name, font_create(name, path, size, loading_mode));
}

static void fonts_on_clear(void) {
    asset_storage_clear(&fonts);
}

static void fonts_on_destroy(void) {
    asset_storage_destroy(&fonts);
}

const struct asset_type fonts_asset_type = {
    .key = "font",
    .on_add = fonts_on_add,
    .on_clear = fonts_on_clear,
    .on_destroy = fonts_on_destroy
};
