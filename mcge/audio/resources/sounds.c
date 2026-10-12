#include "sounds.h"

#include <string.h>

#include "sound.h"
#include "../../assets/asset_storage.h"
#include "../../assets/asset_type.h"
#include "../../logging/logger.h"
#include "../../utils/fields.h"
#include "../../utils/list.h"


static void sounds_destroy_item(void *item) {
    sound_destroy(item);
}

static struct asset_storage sounds = {
    ._key = "sound",
    ._destroy_item = sounds_destroy_item
};

static void sounds_on_add(struct fields *fields) {
    char *name = fields_dup_string(fields, "name", "<null>");

    struct list clip_names = list_create(1);
    const struct fields_list *clips = fields_get_list(fields, "clips");
    for (int i = 0; i < fields_list_count(clips); i++) {
        list_add(&clip_names, strdup(fields_key(fields_list_get(clips, i))));
    }
    if (list_count(&clip_names) == 0) {
        logger_warn("Sound %s expected a list of clips in field `clips`", name);
    }

    asset_storage_add(&sounds, name, sound_create(name, clip_names));
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
    if (name == NULL) {
        return NULL;
    }
    struct sound *sound = asset_storage_get(&sounds, name);
    return sound ? sound : sound_get_fallback();
}
