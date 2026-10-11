//
// Created by nikita on 11.10.2026.
//

#include "sounds.h"

#include <string.h>

#include "audio.h"
#include "../assets/asset_storage.h"
#include "../assets/asset_type.h"
#include "sound.h"
#include "mcge/logging/logger.h"
#include "mcge/utils/fields.h"


static void sounds_destroy_item(void *item) {
    sound_destroy(item);
}

static struct asset_storage sounds = {
    ._key = "sound",
    ._destroy_item = sounds_destroy_item
};

static void sounds_on_add(struct fields *fields) {
    char *name = fields_dup_string(fields, "name", "<null>");
    char *path = fields_dup_string(fields, "path", "<null>");

    const char* loading_mode_name = fields_get_string(fields, "loading_mode", "lazy");
    enum sound_loading_mode loading_mode = sound_loading_mode_lazy;
    if (strcmp(loading_mode_name, "lazy") == 0) {
        loading_mode = sound_loading_mode_lazy;
    }
    else if (strcmp(loading_mode_name, "eager") == 0) {
        loading_mode = sound_loading_mode_eager;
    }
    else {
        logger_warn("Sound %s unexpected loading mode %s. Supported: lazy, eager", name, loading_mode_name);
    }

    const char* storing_mode_name = fields_get_string(fields, "storing_mode", "fast");
    enum sound_storing_mode storing_mode = sound_storing_mode_fast;
    if (strcmp(storing_mode_name, "fast") == 0) {
        storing_mode = sound_storing_mode_fast;
    }
    else if (strcmp(storing_mode_name, "compact") == 0) {
        storing_mode = sound_storing_mode_compact;
    }
    else {
        logger_warn("Sound %s unexpected storing mode %s. Supported: fast, compact", name, storing_mode_name);
    }

    asset_storage_add(&sounds, name, sound_create(name, path, loading_mode, storing_mode));
}
static void sounds_on_clear(void) {
    asset_storage_clear(&sounds);
}
static void sounds_on_destroy(void) {
    asset_storage_destroy(&sounds);
}

const struct asset_type sounds_asset_type = {
    .key = "sound",
    .on_add = sounds_on_add,
    .on_clear = sounds_on_clear,
    .on_destroy = sounds_on_destroy
};

struct sound* sounds_get(const char *name) {
    return asset_storage_get(&sounds, name);
}
