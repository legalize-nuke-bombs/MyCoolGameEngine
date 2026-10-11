//
// Created by nikita on 11.10.2026.
//

#include "audio_source.h"
#include "../component_internal.h"
#include "../../../audio/audio.h"
#include "../../../audio/resources/clip.h"
#include "../../../audio/resources/sound.h"
#include "../../../audio/resources/sounds.h"
#include "../../../audio/tracks/tracks.h"
#include "../../../utils/fields.h"

#define LIST_NAME tracks_list
#define LIST_TYPE struct track
#include "../../../utils/typed_list.h"


struct audio_source {
    struct component base;

    struct tracks_list tracks;

    const struct sound *initial_sound;
    bool initial_loop;
    bool initial_sound_present;
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
        this->initial_sound_present = true;
    }
}

static void audio_source_on_awake(struct component *base) {
    struct audio_source *this = (struct audio_source*)base;
    if (this->initial_sound_present) {
        audio_source_play(this, this->initial_sound, this->initial_loop);
    }
}

static void audio_source_on_movement(struct component *base) {
    const struct audio_source *this = (struct audio_source*)base;
    const struct vector2 position = component_get_rect(base).position;
    for (int i = 0; i < tracks_list_count(&this->tracks); i++) {
        tracks_set_track_position(audio_get_tracks(), tracks_list_get(&this->tracks, i), position);
    }
}

static void audio_source_stop_all_tracks(const struct audio_source *this) {
    for (int i = 0; i < tracks_list_count(&this->tracks); i++) {
        tracks_stop_track(audio_get_tracks(), tracks_list_get(&this->tracks, i));
    }
}

static void audio_source_on_disable(struct component *base) {
    const struct audio_source *this = (struct audio_source*)base;
    audio_source_stop_all_tracks(this);
}

static void audio_source_on_destroy(struct component *base) {
    struct audio_source *this = (struct audio_source*)base;
    audio_source_stop_all_tracks(this);
    tracks_list_destroy(&this->tracks);
}

const struct component_vtable audio_source_vtable = {
    .component_key = audio_source_component_key,
    .on_create = audio_source_on_create,
    .on_awake = audio_source_on_awake,
    .on_movement = audio_source_on_movement,
    .on_disable = audio_source_on_disable,
    .on_destroy = audio_source_on_destroy,
    .size = sizeof(struct audio_source)
};


MCGE_API void audio_source_play(struct audio_source *this, const struct sound *sound, const bool loop) {
    struct MIX_Audio *audio = clip_get_native_audio(sound_get_random_clip(sound));
    const struct vector2 position = component_get_rect((struct component*)this).position;

    const struct track new_track = tracks_play(audio_get_tracks(), audio, position, loop);
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
