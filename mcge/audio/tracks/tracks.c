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

#define TRACKS_BUFFER_SIZE 256

struct tracks {
    MIX_Track* native_tracks[TRACKS_BUFFER_SIZE];
    uint64_t play_numbers[TRACKS_BUFFER_SIZE];
    uint64_t last_play_number;
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

struct track tracks_play(struct tracks* this, MIX_Audio *audio, const MIX_Point3D *position, const bool loop) {
    for (unsigned int i = 0; i < TRACKS_BUFFER_SIZE; i++) {
        MIX_Track* native_track = this->native_tracks[i];
        if (native_track == NULL || MIX_TrackPlaying(native_track)) {
            continue;
        }

        MIX_SetTrackAudio(native_track, audio);
        MIX_SetTrack3DPosition(native_track, position);

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

void tracks_set_track_position(struct tracks* this, const struct track *track, const MIX_Point3D *position) {
    if (!tracks_is_track_playing(this, track)) {
        return;
    }
    MIX_SetTrack3DPosition(this->native_tracks[track->index], position);
}

void tracks_stop_track(struct tracks* this, const struct track *track) {
    if (!tracks_is_track_playing(this, track)) {
        return;
    }
    logger_debug("Tracks stop track %d", track->index);
    MIX_StopTrack(this->native_tracks[track->index], 0);
}
