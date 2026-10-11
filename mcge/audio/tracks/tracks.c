//
// Created by nikita on 11.10.2026.
//

#include "tracks.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../audio.h"
#include <SDL3_mixer/SDL_mixer.h>
#include "../../logging/logger.h"
#include "../../utils/vector2_math.h"

#define TRACKS_BUFFER_SIZE 256
#define TRACKS_DISTANCE_WITHOUT_LISTENER 1e+9f

struct tracks {
    MIX_Track* native_tracks[TRACKS_BUFFER_SIZE];
    uint64_t play_numbers[TRACKS_BUFFER_SIZE];
    struct vector2 positions[TRACKS_BUFFER_SIZE];
    uint64_t last_play_number;

    bool listener_present;
    struct vector2 listener_position;
};

struct tracks* tracks_create() {
    logger_info("Tracks are creating with buffer size %d...", TRACKS_BUFFER_SIZE);
    struct tracks* this = calloc(1, sizeof(struct tracks));
    for (unsigned int i = 0; i < TRACKS_BUFFER_SIZE; i++) {
        this->native_tracks[i] = MIX_CreateTrack(audio_get_mixer());
    }
    return this;
}
void tracks_destroy(struct tracks* this) {
    logger_debug("Tracks are destroying...");
    for (unsigned int i = 0; i < TRACKS_BUFFER_SIZE; i++) {
        MIX_DestroyTrack(this->native_tracks[i]);
    }
    free(this);
}

static void tracks_apply_position(const struct tracks* this, const unsigned int index) {
    MIX_Point3D point = {
        .x = TRACKS_DISTANCE_WITHOUT_LISTENER,
        .y = TRACKS_DISTANCE_WITHOUT_LISTENER,
        .z = TRACKS_DISTANCE_WITHOUT_LISTENER
    };
    if (this->listener_present) {
        const struct vector2 offset = vector_sub(this->positions[index], this->listener_position);
        point.x = (float)offset.x;
        point.y = 0;
        point.z = (float)offset.y;
    }
    MIX_SetTrack3DPosition(this->native_tracks[index], &point);
}
static void tracks_apply_positions_of_playing(const struct tracks* this) {
    for (unsigned int i = 0; i < TRACKS_BUFFER_SIZE; i++) {
        if (this->native_tracks[i] && MIX_TrackPlaying(this->native_tracks[i])) {
            tracks_apply_position(this, i);
        }
    }
}

void tracks_set_listener_position(struct tracks* this, const struct vector2 position) {
    this->listener_present = true;
    this->listener_position = position;
    tracks_apply_positions_of_playing(this);
}
void tracks_remove_listener_position(struct tracks* this) {
    this->listener_present = false;
    tracks_apply_positions_of_playing(this);
}

struct track tracks_play(struct tracks* this, MIX_Audio *audio, const struct vector2 position, const bool loop) {
    for (unsigned int i = 0; i < TRACKS_BUFFER_SIZE; i++) {
        MIX_Track* native_track = this->native_tracks[i];
        if (native_track == NULL || MIX_TrackPlaying(native_track)) {
            continue;
        }

        this->positions[i] = position;
        MIX_SetTrackAudio(native_track, audio);
        tracks_apply_position(this, i);

        const SDL_PropertiesID props = SDL_CreateProperties();
        if (loop) SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
        const bool started = MIX_PlayTrack(native_track, props);
        SDL_DestroyProperties(props);
        if (!started) {
            break;
        }

        logger_debug("Tracks play on track %d", i);
        this->play_numbers[i] = ++this->last_play_number;
        const struct track track = {
            .index = i,
            .play_number = this->play_numbers[i]
        };
        return track;
    }
    logger_debug("Tracks failed to play");
    return track_invalid;
}

bool tracks_is_track_playing(const struct tracks* this, const struct track *track) {
    if (track->index >= TRACKS_BUFFER_SIZE || this->play_numbers[track->index] != track->play_number) {
        return false;
    }
    return MIX_TrackPlaying(this->native_tracks[track->index]);
}

void tracks_set_track_position(struct tracks* this, const struct track *track, const struct vector2 position) {
    if (!tracks_is_track_playing(this, track)) {
        return;
    }
    this->positions[track->index] = position;
    tracks_apply_position(this, track->index);
}

void tracks_stop_track(struct tracks* this, const struct track *track) {
    if (!tracks_is_track_playing(this, track)) {
        return;
    }
    logger_debug("Tracks stop track %d", track->index);
    MIX_StopTrack(this->native_tracks[track->index], 0);
}
