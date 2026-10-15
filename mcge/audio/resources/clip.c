//
// Created by nikita on 11.10.2026.
//

#include "clip.h"
#include <stdlib.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "../audio.h"
#include "mcge/logging/logger.h"
#include "mcge/memory/allocator.h"


struct clip {
    char *id;
    char *path;
    enum clip_storing_mode storing_mode;
    MIX_Audio *native_audio;
    bool loading_error;
};


struct clip* clip_create(char *id, char *path, const enum clip_loading_mode loading_mode, const enum clip_storing_mode storing_mode) {
    struct clip* this = allocator_new0(1, sizeof(struct clip));
    this->id = id;
    this->path = path;
    this->storing_mode = storing_mode;
    if (loading_mode == clip_loading_mode_eager) {
        clip_get_native_audio(this);
    }
    return this;
}
void clip_destroy(struct clip* this) {
    allocator_delete(this->id);
    allocator_delete(this->path);
    if (this->native_audio) MIX_DestroyAudio(this->native_audio);
    allocator_delete(this);
}

const char* clip_get_id(const struct clip* this) {
    return this->id;
}

MIX_Audio* clip_get_native_audio(struct clip* this) {
    if (this == NULL) {
        logger_warn("Clip nullptr access. Fallback returned");
        return audio_get_fallback_audio();
    }
    if (this->native_audio == NULL && !this->loading_error) {
        logger_debug("Clip %s is loading from %s...", this->id, this->path);
        this->native_audio = MIX_LoadAudio(audio_get_mixer(), this->path, this->storing_mode == clip_storing_mode_fast);
        if (this->native_audio == NULL) {
            logger_warn("Clip %s failed to load from %s", this->id, this->path);
            this->loading_error = true;
        }
    }
    return this->native_audio ? this->native_audio : audio_get_fallback_audio();
}