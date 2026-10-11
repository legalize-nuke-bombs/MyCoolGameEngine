//
// Created by nikita on 11.10.2026.
//

#include "audio_listener.h"
#include "../component_internal.h"
#include "../../../audio/audio.h"
#include "../../../audio/tracks/tracks.h"
#include "../../../logging/logger.h"
#include "../../../scene/scene.h"
#include "../../../utils/dictionary.h"


struct audio_listener {
    struct component base;
};

static void audio_listener_on_movement(struct component *base) {
    tracks_set_listener_position(audio_get_tracks(), component_get_rect(base).position);
}

static void audio_listener_on_awake(struct component *base) {
    const struct dictionary *listeners = tmap_try_get_components(scene_get_tmap(), audio_listener_component_key());
    if (listeners != NULL && dictionary_count(listeners) > 1) {
        logger_warn("%d audio listeners on the scene. Expected 1", dictionary_count(listeners));
    }
    audio_listener_on_movement(base);
}

static void audio_listener_on_disable(struct component *base) {
    tracks_remove_listener_position(audio_get_tracks());
}

const struct component_vtable audio_listener_vtable = {
    .component_key = audio_listener_component_key,
    .size = sizeof(struct audio_listener),
    .on_awake = audio_listener_on_awake,
    .on_movement = audio_listener_on_movement,
    .on_disable = audio_listener_on_disable
};

const char* audio_listener_component_key(void) {
    return "audio_listener";
}
