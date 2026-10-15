//
// Created by nikita on 11.10.2026.
//

#ifndef MYCOOLGAMEENGINE_CLIP_H
#define MYCOOLGAMEENGINE_CLIP_H

#include "../../api.h"

enum clip_loading_mode {
    clip_loading_mode_lazy,
    clip_loading_mode_eager
};

enum clip_storing_mode {
    clip_storing_mode_fast,
    clip_storing_mode_compact
};

struct clip;
struct MIX_Mixer;
struct MIX_Audio;

MCGE_API struct clip* clip_create(char *id, char *path, float volume, enum clip_loading_mode loading_mode, enum clip_storing_mode storing_mode);
MCGE_API void clip_destroy(struct clip* this);

MCGE_API const char* clip_get_id(const struct clip* this);

MCGE_API struct MIX_Audio* clip_get_native_audio(struct clip* this);
MCGE_API float clip_get_volume(const struct clip* this);

#endif //MYCOOLGAMEENGINE_CLIP_H
