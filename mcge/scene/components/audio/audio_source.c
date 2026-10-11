//
// Created by nikita on 11.10.2026.
//

#include "audio_source.h"
#include "../component_internal.h"
#include <SDL3_mixer/SDL_mixer.h>
#include "../../../audio/audio.h"
#include "mcge/audio/sound.h"
#include "mcge/audio/sounds.h"
#include "mcge/logging/logger.h"
#include "mcge/random/random.h"
#include "mcge/scene/entity.h"
#include "mcge/utils/fields.h"

struct audio_source {
    struct component base;

    struct list sounds;
    enum audio_source_on_finished_policy on_finished;
    MIX_Track *track;
};

const char* audio_source_component_key(void) {
    return "audio_source";
}

static void audio_source_on_create(struct component *base, struct fields *fields) {
    struct audio_source *this = (struct audio_source*)base;

    if (fields_has(fields, "sounds")) {
        const struct fields_list *sounds = fields_get_list(fields, "sounds");
        for (int i = 0; i < fields_list_count(sounds); i++) {
            const char* sound_name = fields_key(fields_list_get(sounds, i));
            list_add(&this->sounds, sounds_get(sound_name));
        }
    }

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

    logger_debug("Entity %s has audio source with %d sounds", component_get_global_parent_name(base), list_count(&this->sounds));

    this->track = MIX_CreateTrack(audio_get_mixer());

}
static void audio_source_on_destroy(struct component *base) {
    struct audio_source *this = (struct audio_source*)base;
    if (this->track) MIX_DestroyTrack(this->track);
    list_destroy(&this->sounds);
}

static void audio_source_awake(struct component *base) {
    const struct audio_source *this = (struct audio_source*)base;
    audio_source_try_play(this);
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
            audio_source_try_play(this);
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
    .on_awake = audio_source_awake,
    .on_destroy = audio_source_on_destroy,
    .on_simulation_chunk_update = audio_source_on_simulation_chunk_update,
    .size = sizeof(struct audio_source)
};


MCGE_API bool audio_source_try_play(const struct audio_source *this) {
    if (list_count(&this->sounds) == 0) {
        return false;
    }
    struct sound *sound = list_get(&this->sounds, random_next_int(0, list_count(&this->sounds)));
    MIX_SetTrackAudio(this->track, sound_get_native_clip(sound));
    MIX_PlayTrack(this->track, 0);
    return true;
}