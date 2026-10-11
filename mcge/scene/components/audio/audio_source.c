//
// Created by nikita on 11.10.2026.
//

#include "audio_source.h"
#include "../component_internal.h"
#include <SDL3_mixer/SDL_mixer.h>
#include "../../../audio/audio.h"
#include "mcge/logging/logger.h"
#include "mcge/scene/entity.h"
#include "mcge/utils/fields.h"

struct audio_source {
    struct component base;

    enum audio_source_on_finished_policy on_finished;
    MIX_Track *track;
};

const char* audio_source_component_key(void) {
    return "audio_source";
}

static void audio_source_on_create(struct component *base, struct fields *fields) {
    struct audio_source *this = (struct audio_source*)base;

    const char* on_finished_str = fields_get_string(fields, "on_finished", "loop");
    if (strcmp(on_finished_str, "loop") == 0) {
        this->on_finished = audio_source_on_finished_policy_loop;
    }
    else if (strcmp(on_finished_str, "destroy") == 0) {
        this->on_finished = audio_source_on_finished_policy_destroy;
    }
    else if (strcmp(on_finished_str, "idle") == 0) {
        this->on_finished = audio_source_on_finished_policy_idle;
    }
    else {
        logger_warn("Audio source on_finished unexpected value: %s. Supported: loop, destroy, idle", on_finished_str);
    }

    this->track = MIX_CreateTrack(audio_get_mixer());
    MIX_SetTrackAudio(this->track, audio_get_fallback_audio());
    MIX_PlayTrack(this->track, 0);
}
static void audio_source_on_destroy(struct component *base) {
    const struct audio_source *this = (struct audio_source*)base;
    if (this->track) MIX_DestroyTrack(this->track);
}

static void audio_source_on_simulation_chunk_update(struct component *base, const struct update_context *context) {
    const struct audio_source *this = (struct audio_source*)base;
    if (this->track == NULL) {
        return;
    }
    if (MIX_TrackPlaying(this->track)) {
        return;
    }
    switch (this->on_finished) {
        case audio_source_on_finished_policy_loop:
            MIX_PlayTrack(this->track, 0);
            break;
        case audio_source_on_finished_policy_destroy:
            component_mark_destroyed(base);
            break;
        case audio_source_on_finished_policy_idle:
            break;
    }
}

const struct component_vtable audio_source_vtable = {
    .component_key = audio_source_component_key,
    .on_create = audio_source_on_create,
    .on_destroy = audio_source_on_destroy,
    .on_simulation_chunk_update = audio_source_on_simulation_chunk_update,
    .size = sizeof(struct audio_source)
};
