//
// Created by nikita on 11.10.2026.
//

#include "sound.h"
#include <SDL3_mixer/SDL_mixer.h>

#include "mcge/logging/logger.h"


struct sound {
    char *id;
    char *path;
    enum sound_storing_mode storing_mode;
    MIX_Audio *native_clip;
    MIX_Mixer *mixer;
};


struct sound* sound_create(char *id, char *path, enum sound_loading_mode loading_mode, enum sound_storing_mode storing_mode, MIX_Mixer *mixer) {
    struct sound* this = calloc(1, sizeof(struct sound));
    this->id = id;
    this->path = path;
    this->storing_mode = storing_mode;
    this->mixer = mixer;
    if (loading_mode == sound_loading_mode_eager) {
        sound_get_native_clip(this);
    }
    return this;
}
void sound_destroy(struct sound* this) {
    free(this->id);
    free(this->path);
    if (this->native_clip) MIX_DestroyAudio(this->native_clip);
    free(this);
}

const char* sound_get_id(const struct sound* this) {
    return this->id;
}

MIX_Audio* sound_get_native_clip(struct sound* this) {
    if (this->native_clip == NULL) {
        logger_debug("Sound %s is loading from %s...", this->id, this->path);
        this->native_clip = MIX_LoadAudio(this->mixer, this->path, this->storing_mode == sound_storing_mode_fast);
        if (this->native_clip == NULL) {
            logger_warn("Sound %s failed to load from %s", this->id, this->path);
        }
    }
    return this->native_clip;
}