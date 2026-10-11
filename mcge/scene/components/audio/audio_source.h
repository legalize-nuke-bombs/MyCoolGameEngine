//
// Created by nikita on 11.10.2026.
//

#ifndef MYCOOLGAMEENGINE_AUDIO_SOURCE_H
#define MYCOOLGAMEENGINE_AUDIO_SOURCE_H

#include "../../../api.h"


enum audio_source_on_finished_policy {
    audio_source_on_finished_policy_loop,
    audio_source_on_finished_policy_destroy,
    audio_source_on_finished_policy_idle
};


MCGE_API const char* audio_source_component_key(void);

MCGE_API extern const struct component_vtable audio_source_vtable;

#endif //MYCOOLGAMEENGINE_AUDIO_SOURCE_H
