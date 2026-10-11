//
// Created by nikita on 11.10.2026.
//

#include "clips.h"

#include <string.h>

#include "audio.h"
#include "../assets/asset_storage.h"
#include "../assets/asset_type.h"
#include "clip.h"
#include "mcge/logging/logger.h"
#include "mcge/utils/fields.h"


static void clips_destroy_item(void *item) {
    clip_destroy(item);
}

static struct asset_storage clips = {
    ._key = "clip",
    ._destroy_item = clips_destroy_item
};

static void clips_on_add(struct fields *fields) {
    char *name = fields_dup_string(fields, "name", "<null>");
    char *path = fields_dup_string(fields, "path", "<null>");

    const char* loading_mode_name = fields_get_string(fields, "loading_mode", "lazy");
    enum clip_loading_mode loading_mode = clip_loading_mode_lazy;
    if (strcmp(loading_mode_name, "lazy") == 0) {
        loading_mode = clip_loading_mode_lazy;
    }
    else if (strcmp(loading_mode_name, "eager") == 0) {
        loading_mode = clip_loading_mode_eager;
    }
    else {
        logger_warn("Clip %s unexpected loading mode %s. Supported: lazy, eager", name, loading_mode_name);
    }

    const char* storing_mode_name = fields_get_string(fields, "storing_mode", "fast");
    enum clip_storing_mode storing_mode = clip_storing_mode_fast;
    if (strcmp(storing_mode_name, "fast") == 0) {
        storing_mode = clip_storing_mode_fast;
    }
    else if (strcmp(storing_mode_name, "compact") == 0) {
        storing_mode = clip_storing_mode_compact;
    }
    else {
        logger_warn("Clip %s unexpected storing mode %s. Supported: fast, compact", name, storing_mode_name);
    }

    asset_storage_add(&clips, name, clip_create(name, path, loading_mode, storing_mode));
}
static void clips_on_clear(void) {
    asset_storage_clear(&clips);
}
static void clips_on_destroy(void) {
    asset_storage_destroy(&clips);
}

const struct asset_type clips_asset_type = {
    .key = "clip",
    .on_add = clips_on_add,
    .on_clear = clips_on_clear,
    .on_destroy = clips_on_destroy
};

struct clip* clips_get(const char *name) {
    return asset_storage_get(&clips, name);
}
