//
// Created by nikita on 12.10.2026.
//

#include "rigid_surface_sound.h"

#include <stdlib.h>

#include "mcge/audio/resources/sounds.h"

struct rigid_surface_sound {
    char *id;
    struct sound *sound;
};


struct rigid_surface_sound* rigid_surface_sound_create(char *id, char *sound_name) {
    struct rigid_surface_sound *this = calloc(1, sizeof(struct rigid_surface_sound));
    this->id = id;
    this->sound = sounds_get(sound_name);
    free(sound_name);
    return this;
}
void rigid_surface_sound_destroy(struct rigid_surface_sound *this) {
    free(this->id);
    free(this);
}

struct sound* rigid_surface_sound_get_sound(struct rigid_surface_sound *this) {
    return this->sound;
}