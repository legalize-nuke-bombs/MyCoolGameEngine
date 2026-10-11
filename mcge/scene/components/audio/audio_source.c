//
// Created by nikita on 11.10.2026.
//

#include "audio_source.h"
#include "../component_internal.h"
#include <SDL3_mixer/SDL_mixer.h>
#include "../../../audio/audio.h"
#include "../../../audio/resources/sounds.h"
#include "../../../logging/logger.h"
#include "../../../scene/entity.h"
#include "../../../scene/scene.h"
#include "../../../utils/dictionary.h"
#include "../../../utils/fields.h"
#include "../../../utils/vector2_math.h"
#include "../../../audio/tracks/tracks.h"

#define LIST_NAME tracks_list
#define LIST_TYPE struct track
#include "../../../utils/typed_list.h"
#include "mcge/audio/resources/clip.h"
#include "mcge/audio/resources/sound.h"


struct audio_source {
    struct component base;

    struct tracks_list tracks;

    const struct sound *initial_sound;
    bool initial_loop;
    bool initial_sound_pending;

    bool listener_missing_reported;
    uint128_t audio_listener_id;
};

const char* audio_source_component_key(void) {
    return "audio_source";
}

static void audio_source_on_create(struct component *base, struct fields *fields) {
    struct audio_source *this = (struct audio_source*)base;

    this->tracks = tracks_list_create(1);

    const char *sound_name = fields_get_string(fields, "sound", NULL);
    this->initial_loop = fields_get_bool(fields, "loop", 1);
    if (sound_name) {
        this->initial_sound = sounds_get(sound_name);
        this->initial_sound_pending = true;
    }
}

static void audio_source_stop_all_tracks(const struct audio_source *this) {
    for (int i = 0; i < tracks_list_count(&this->tracks); i++) {
        tracks_stop_track(audio_get_tracks(), tracks_list_get(&this->tracks, i));
    }
}

static void audio_source_on_destroy(struct component *base) {
    struct audio_source *this = (struct audio_source*)base;
    audio_source_stop_all_tracks(this);
    tracks_list_destroy(&this->tracks);
}

static void audio_source_on_disable(struct component *base) {
    const struct audio_source *this = (struct audio_source*)base;
    audio_source_stop_all_tracks(this);
}

static MIX_Point3D audio_source_get_native_position(struct audio_source *this);

static void audio_source_on_simulation_chunk_update(struct component *base, const struct update_context *context) {
    struct audio_source *this = (struct audio_source*)base;

    if (this->initial_sound_pending) {
        this->initial_sound_pending = false;
        audio_source_play(this, this->initial_sound, this->initial_loop);
    }

    for (int i = 0; i < tracks_list_count(&this->tracks); i++) {
        const struct track* track = tracks_list_get(&this->tracks, i);
        if (tracks_is_track_playing(audio_get_tracks(), track)) {
            const MIX_Point3D point = audio_source_get_native_position(this);
            tracks_set_track_position(audio_get_tracks(), track, &point);
        }
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



MCGE_API void audio_source_play(struct audio_source *this, const struct sound *sound, const bool loop) {
    // This function can be called in constructor when audio listener is not exist yet,
    // so we will put proper position during update.
    // Sound will not be heard in the first frame with 1e+9 distances.
    const MIX_Point3D far_away = {.x = (float)1e+9, .y = (float)1e+9, .z = (float)1e+9};
    const MIX_Point3D point = component_is_awake((struct component*)this) ? audio_source_get_native_position(this) : far_away;

    const struct track new_track = tracks_play(audio_get_tracks(), clip_get_native_audio(sound_get_random_clip(sound)), &point, loop);
    if (!tracks_is_track_playing(audio_get_tracks(), &new_track)) {
        return;
    }

    for (int i = 0; i < tracks_list_count(&this->tracks); i++) {
        if (!tracks_is_track_playing(audio_get_tracks(), tracks_list_get(&this->tracks, i))) {
            tracks_list_set(&this->tracks, i, new_track);
            return;
        }
    }
    tracks_list_add(&this->tracks, new_track);
}
