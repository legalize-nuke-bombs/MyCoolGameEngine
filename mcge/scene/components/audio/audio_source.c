//
// Created by nikita on 11.10.2026.
//

#include "audio_source.h"
#include "../component_internal.h"
#include <string.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "../../../audio/audio.h"
#include "../../../audio/sound.h"
#include "../../../audio/sounds.h"
#include "../../../logging/logger.h"
#include "../../../random/random.h"
#include "../../../scene/entity.h"
#include "../../../scene/scene.h"
#include "../../../utils/dictionary.h"
#include "../../../utils/fields.h"
#include "../../../utils/vector2_math.h"

struct audio_source {
    struct component base;

    struct list sounds;
    enum audio_source_on_finished_policy on_finished;
    MIX_Track *track;
    bool started;
    bool listener_missing_reported;

    uint128_t audio_listener_id;
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

static void audio_source_on_disable(struct component *base) {
    const struct audio_source *this = (struct audio_source*)base;
    if (this->track) MIX_StopTrack(this->track, 0);
}

static MIX_Point3D audio_source_get_native_position(struct audio_source *this);

static void audio_source_on_simulation_chunk_update(struct component *base, const struct update_context *context) {
    struct audio_source *this = (struct audio_source*)base;
    if (this->track == NULL) {
        return;
    }
    if (MIX_TrackPlaying(this->track)) {
        const MIX_Point3D point = audio_source_get_native_position(this);
        MIX_SetTrack3DPosition(this->track, &point);
        return;
    }
    if (!this->started) {
        this->started = true;
        audio_source_try_play(this);
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
    .on_disable = audio_source_on_disable,
    .on_destroy = audio_source_on_destroy,
    .on_simulation_chunk_update = audio_source_on_simulation_chunk_update,
    .size = sizeof(struct audio_source)
};


MCGE_API bool audio_source_try_play(struct audio_source *this) {
    if (list_count(&this->sounds) == 0) {
        return false;
    }
    struct sound *sound = list_get(&this->sounds, random_next_int(0, list_count(&this->sounds)));
    const MIX_Point3D point = audio_source_get_native_position(this);
    MIX_SetTrack3DPosition(this->track, &point);
    MIX_SetTrackAudio(this->track, sound_get_native_clip(sound));
    MIX_PlayTrack(this->track, 0);
    return true;
}

static MIX_Point3D audio_source_get_native_position(struct audio_source *this) {
    const MIX_Point3D fallback = {.x = 0, .y = 0, .z = 0};

    struct audio_listener* listener = (struct audio_listener*)scene_try_get_component(this->audio_listener_id);
    if (listener == NULL) {
        const struct dictionary* dict = tmap_try_get_components(scene_get_tmap(), "audio_listener");
        if (dict == NULL || dictionary_count(dict) == 0) {
            if (!this->listener_missing_reported) {
                logger_warn("No audio listeners on the scene");
                this->listener_missing_reported = true;
            }
            return fallback;
        }
        if (dictionary_count(dict) > 1) {
            logger_warn("%d audio listeners on the scene. Expected 1", dictionary_count(dict));
        }
        struct dictionary_iterator iterator = dictionary_begin(dict);
        struct dictionary_node node;
        dictionary_next(dict, &iterator, &node);
        listener = node.value;
        this->audio_listener_id = component_get_id((struct component*)listener);
        this->listener_missing_reported = false;
    }

    const struct vector2 self_position = entity_get_rect(component_get_parent((struct component*)this)).position;
    const struct vector2 listener_position = entity_get_rect(component_get_parent((struct component*)listener)).position;
    const struct vector2 offset = vector_sub(self_position, listener_position);

    const MIX_Point3D result = {.x = (float)offset.x, .y = 0, .z = (float)offset.y};
    return result;
}