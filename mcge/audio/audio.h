//
// Created by nikita on 11.10.2026.
//

#ifndef MYCOOLGAMEENGINE_AUDIO_H
#define MYCOOLGAMEENGINE_AUDIO_H

#include "../api.h"

struct msystem;

MCGE_API extern const struct msystem audio_msystem;

MCGE_API struct MIX_Mixer* audio_get_mixer();
MCGE_API struct MIX_Audio* audio_get_fallback_audio();

#endif //MYCOOLGAMEENGINE_AUDIO_H
