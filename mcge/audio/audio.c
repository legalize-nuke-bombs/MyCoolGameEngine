//
// Created by nikita on 11.10.2026.
//

#include "audio.h"
#include <SDL3_mixer/SDL_mixer.h>
#include "../msystems/msystem.h"
#include "mcge/logging/logger.h"


static struct {
    MIX_Mixer *mixer;
} audio;


static void audio_on_create(void) {
    if (MIX_Init() == 0) {
        logger_error("Failed to initializer SDL Mixer");
        return;
    }
    audio.mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (audio.mixer == NULL) {
        logger_error("Failed to create Mixer");
    }
}
static void audio_on_destroy(void) {
    if (audio.mixer) {
        MIX_DestroyMixer(audio.mixer);
    }
    MIX_Quit();
}

const struct msystem audio_msystem = {
    .name = "renderer",
    .on_create = audio_on_create,
    .on_destroy = audio_on_destroy
};
