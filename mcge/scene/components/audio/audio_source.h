//
// Created by nikita on 11.10.2026.
//

#ifndef MYCOOLGAMEENGINE_AUDIO_SOURCE_H
#define MYCOOLGAMEENGINE_AUDIO_SOURCE_H

#include <stdbool.h>

#include "../../../api.h"

struct audio_source;
struct sound;

MCGE_API const char* audio_source_component_key(void);

MCGE_API extern const struct component_vtable audio_source_vtable;

MCGE_API void audio_source_play(struct audio_source *this, const struct sound *sound, bool loop);

#endif //MYCOOLGAMEENGINE_AUDIO_SOURCE_H
