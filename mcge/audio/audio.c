//
// Created by nikita on 11.10.2026.
//

#include "audio.h"
#include <SDL3_mixer/SDL_mixer.h>
#include "../msystems/msystem.h"
#include "mcge/logging/logger.h"


static struct {
    MIX_Mixer *mixer;

    MIX_Audio *fallback_audio;
} audio;


static void audio_on_create(void) {
    if (MIX_Init() == 0) {
        logger_error("Failed to initializer SDL Mixer");
        return;
    }
    audio.mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (audio.mixer == NULL) {
        logger_error("Failed to create Mixer");
        return;
    }
    audio.fallback_audio = MIX_CreateSineWaveAudio(audio.mixer, 100, 0.5f, 100);
}
static void audio_on_destroy(void) {
    if (audio.mixer) {
        MIX_DestroyMixer(audio.mixer);
    }
    if (audio.fallback_audio) MIX_DestroyAudio(audio.fallback_audio);
    MIX_Quit();
}

const struct msystem audio_msystem = {
    .name = "renderer",
    .on_create = audio_on_create,
    .on_destroy = audio_on_destroy
};

MIX_Mixer* audio_get_mixer() {
    return audio.mixer;
}
MIX_Audio* audio_get_fallback_audio() {
    return audio.fallback_audio;
}