//
// Created by nikita on 11.10.2026.
//

#ifndef MYCOOLGAMEENGINE_SOUND_H
#define MYCOOLGAMEENGINE_SOUND_H

#include "../api.h"

enum sound_loading_mode {
    sound_loading_mode_lazy,
    sound_loading_mode_eager
};

enum sound_storing_mode {
    sound_storing_mode_fast,
    sound_storing_mode_compact
};

struct sound;
struct MIX_Mixer;
struct MIX_Audio;

MCGE_API struct sound* sound_create(char *id, char *path, enum sound_loading_mode loading_mode, enum sound_storing_mode storing_mode, struct MIX_Mixer *mixer);
MCGE_API void sound_destroy(struct sound* this);

MCGE_API const char* sound_get_id(const struct sound* this);

MCGE_API struct MIX_Audio* sound_get_native_clip(struct sound* this);

#endif //MYCOOLGAMEENGINE_SOUND_H
