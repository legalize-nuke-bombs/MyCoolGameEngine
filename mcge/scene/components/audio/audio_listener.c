//
// Created by nikita on 11.10.2026.
//

#include "audio_listener.h"
#include "../component_internal.h"


struct audio_listener {
    struct component base;
};

const struct component_vtable audio_listener_vtable = {
    .component_key = audio_listener_component_key,
    .size = sizeof(struct audio_listener)
};

const char* audio_listener_component_key(void) {
    return "audio_listener";
}