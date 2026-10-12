//
// Created by nikita on 12.10.2026.
//

#include "rigid_material_sounds.h"
#include <mcge/mcge.h>
#include "rigid_material_sound.h"


static void rigid_material_sounds_destroy_item(void *item) {
    rigid_material_sound_destroy(item);
}
static struct asset_storage rigid_material_sounds = {
    ._key = "rigid_material_sounds",
    ._destroy_item = rigid_material_sounds_destroy_item
};


static void rigid_material_sound_on_add(struct fields *fields) {
    char *name = fields_dup_string(fields, "name", "<null>");
    char *sound = fields_dup_string(fields, "sound", "<null>");

    asset_storage_add(&rigid_material_sounds, name, rigid_material_sound_create(name, sound));
}
static void rigid_material_sound_on_clear(void) {
    asset_storage_clear(&rigid_material_sounds);
}
static void rigid_material_sound_on_destroy(void) {
    asset_storage_destroy(&rigid_material_sounds);
}

const struct asset_type rigid_material_sounds_asset_type = {
    .key = "rigid_material_sound",
    .on_add = rigid_material_sound_on_add,
    .on_clear = rigid_material_sound_on_clear,
    .on_destroy = rigid_material_sound_on_destroy
};

struct sound* rigid_material_sounds_get_sound(const char *name) {
    const struct rigid_material_sound *rigid_material_sound = asset_storage_get(&rigid_material_sounds, name);
    return rigid_material_sound ? rigid_material_sound_get_sound(rigid_material_sound) : NULL;
}
