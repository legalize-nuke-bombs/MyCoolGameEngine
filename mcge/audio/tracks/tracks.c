//
// Created by nikita on 11.10.2026.
//

#include "tracks.h"

#include <stdbool.h>
#include <stdlib.h>
#include "../audio.h"
#include <SDL3_mixer/SDL_mixer.h>
#include "../../logging/logger.h"

#define TRACKS_BUFFER_SIZE 256

struct tracks {
    MIX_Track* native_tracks[TRACKS_BUFFER_SIZE];
    bool tracks_occupancy[TRACKS_BUFFER_SIZE];
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

struct track tracks_request_track(struct tracks* this) {
    for (unsigned int i = 0; i < TRACKS_BUFFER_SIZE; i++) {
        if (!this->tracks_occupancy[i] && this->native_tracks[i]) {
            logger_debug("Tracks provided track %d", i);
            this->tracks_occupancy[i] = true;
            const struct track track = {
                .native_track = this->native_tracks[i],
                .id = i
            };
            return track;
        }
    }
    logger_debug("Tracks failed to provide track");
    return track_invalid;
}
void tracks_release_track(struct tracks* this, struct track *track) {
    if (track->id == -1) {
        return;
    }
    logger_debug("Tracks got back track %d", track->id);
    MIX_StopTrack(track->native_track, 0);
    this->tracks_occupancy[track->id] = false;
    *track = track_invalid;
}
