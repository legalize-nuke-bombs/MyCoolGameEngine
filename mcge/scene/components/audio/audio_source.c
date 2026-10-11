//
// Created by nikita on 11.10.2026.
//

#include "audio_source.h"
#include "../component_internal.h"

struct audio_source {
    struct component base;
};

const struct component_vtable audio_source_vtable = {
    .component_key = audio_source_component_key,
    .size = sizeof(struct audio_source)
};

const char* audio_source_component_key(void) {
    return "audio_source";
}