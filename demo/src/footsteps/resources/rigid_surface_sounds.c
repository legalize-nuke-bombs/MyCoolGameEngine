//
// Created by nikita on 12.10.2026.
//

#include "rigid_surface_sounds.h"
#include <mcge/mcge.h>
#include "rigid_surface_sound.h"


static void rigid_surface_sounds_destroy_item(void *item) {
    rigid_surface_sound_destroy(item);
}
static struct asset_storage rigid_surface_sounds = {
    ._key = "rigid_surface_sounds",
    ._destroy_item = rigid_surface_sounds_destroy_item
};


static void rigid_surface_sound_on_add(struct fields *fields) {
    char *name = fields_dup_string(fields, "name", "<null>");
    char *sound = fields_dup_string(fields, "sound", "<null>");

    asset_storage_add(&rigid_surface_sounds, name, rigid_surface_sound_create(name, sound));
}
static void rigid_surface_sound_on_clear(void) {
    asset_storage_clear(&rigid_surface_sounds);
}
static void rigid_surface_sound_on_destroy(void) {
    asset_storage_destroy(&rigid_surface_sounds);
}

const struct asset_type rigid_surface_sounds_asset_type = {
    .key = "rigid_surface_sound",
    .on_add = rigid_surface_sound_on_add,
    .on_clear = rigid_surface_sound_on_clear,
    .on_destroy = rigid_surface_sound_on_destroy
};

struct sound* rigid_surface_sounds_get_sound(const char *name) {
    struct rigid_surface_sound *rigid_surface_sound = asset_storage_get(&rigid_surface_sounds, name);
    return rigid_surface_sound ? rigid_surface_sound_get_sound(rigid_surface_sound) : NULL;
}
