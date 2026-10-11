//
// Created by nikita on 11.10.2026.
//

#include "sound.h"
#include <SDL3_mixer/SDL_mixer.h>

#include "audio.h"
#include "mcge/logging/logger.h"


struct sound {
    char *id;
    char *path;
    enum sound_storing_mode storing_mode;
    MIX_Audio *native_clip;
    bool loading_error;
};


struct sound* sound_create(char *id, char *path, const enum sound_loading_mode loading_mode, const enum sound_storing_mode storing_mode) {
    struct sound* this = calloc(1, sizeof(struct sound));
    this->id = id;
    this->path = path;
    this->storing_mode = storing_mode;
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
    if (this == NULL) {
        return audio_get_fallback_audio();
    }
    if (this->native_clip == NULL && !this->loading_error) {
        logger_debug("Sound %s is loading from %s...", this->id, this->path);
        this->native_clip = MIX_LoadAudio(audio_get_mixer(), this->path, this->storing_mode == sound_storing_mode_fast);
        if (this->native_clip == NULL) {
            logger_warn("Sound %s failed to load from %s", this->id, this->path);
            this->loading_error = true;
        }
    }
    return this->native_clip ? this->native_clip : audio_get_fallback_audio();
}