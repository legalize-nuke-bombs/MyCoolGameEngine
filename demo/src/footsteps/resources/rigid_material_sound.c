//
// Created by nikita on 12.10.2026.
//

#include "rigid_material_sound.h"

#include <stdlib.h>

#include "mcge/audio/resources/sounds.h"
#include "mcge/allocator/allocator.h"

struct rigid_material_sound {
    char *id;
    struct sound *sound;
};


struct rigid_material_sound* rigid_material_sound_create(char *id, char *sound_name) {
    struct rigid_material_sound *this = allocator_new0(1, sizeof(struct rigid_material_sound));
    this->id = id;
    this->sound = sounds_get(sound_name);
    allocator_delete(sound_name);
    return this;
}
void rigid_material_sound_destroy(struct rigid_material_sound *this) {
    allocator_delete(this->id);
    allocator_delete(this);
}

struct sound* rigid_material_sound_get_sound(const struct rigid_material_sound *this) {
    return this->sound;
}
