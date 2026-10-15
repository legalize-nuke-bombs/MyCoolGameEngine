//
// Created by nikita on 11.10.2026.
//

#include "audio.h"
#include <stdlib.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "tracks/tracks.h"
#include "../engine/msystem.h"
#include "mcge/logging/logger.h"
#include "mcge/memory/allocator.h"


#define AUDIO_FALLBACK_SAMPLE_RATE 44100
#define AUDIO_FALLBACK_FREQUENCY 440
#define AUDIO_FALLBACK_AMPLITUDE 0.15f


static struct {
    MIX_Mixer *mixer;

    MIX_Audio *fallback_audio;
    struct tracks *tracks;
} audio;


static MIX_Audio* audio_create_one_second_beep(void) {
    const SDL_AudioSpec spec = {.format = SDL_AUDIO_F32, .channels = 1, .freq = AUDIO_FALLBACK_SAMPLE_RATE};
    const size_t size = sizeof(float) * AUDIO_FALLBACK_SAMPLE_RATE;
    float *samples = allocator_new(size);
    for (int i = 0; i < AUDIO_FALLBACK_SAMPLE_RATE; i++) {
        samples[i] = AUDIO_FALLBACK_AMPLITUDE * SDL_sinf(2 * SDL_PI_F * AUDIO_FALLBACK_FREQUENCY * (float)i / AUDIO_FALLBACK_SAMPLE_RATE);
    }
    MIX_Audio *beep = MIX_LoadRawAudio(audio.mixer, samples, size, &spec);
    allocator_delete(samples);
    return beep;
}

static void audio_create_mixer(void) {
    if (MIX_Init() == 0) {
        logger_error("Failed to initializer SDL Mixer");
        return;
    }
    audio.mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (audio.mixer == NULL) {
        logger_error("Failed to create Mixer");
    }
}

static void audio_on_create(void) {
    audio_create_mixer();
    if (audio.mixer) audio.fallback_audio = audio_create_one_second_beep();
    audio.tracks = tracks_create();
}
static void audio_on_destroy(void) {
    if (audio.tracks) tracks_destroy(audio.tracks);
    if (audio.fallback_audio) MIX_DestroyAudio(audio.fallback_audio);
    if (audio.mixer) {
        MIX_DestroyMixer(audio.mixer);
    }
    MIX_Quit();
}

const struct msystem audio_msystem = {
    .name = "audio",
    .on_create = audio_on_create,
    .on_destroy = audio_on_destroy
};

MIX_Mixer* audio_get_mixer() {
    return audio.mixer;
}
MIX_Audio* audio_get_fallback_audio() {
    return audio.fallback_audio;
}
struct tracks* audio_get_tracks() {
    return audio.tracks;
}