//
// Created by nikita on 12.10.2026.
//

#include "footsteps_sounds.h"
#include <mcge/mcge.h>

#include "footsteps.h"
#include "resources/rigid_material_sounds.h"


struct footsteps_sounds {
    struct component base;

    uint128_t footsteps_id;
    uint128_t audio_source_id;
    unsigned int footsteps_on_footstep_token;
};


const char* footsteps_sounds_component_key(void) {
    return "footsteps_sounds";
}

static void handle_footstep(void *listener, void *context);
static void footsteps_sounds_awake(struct component* base) {
    struct footsteps_sounds* this = (struct footsteps_sounds*)base;
    struct entity* entity = component_get_parent(base);

    struct footsteps* footsteps = (struct footsteps*)entity_get_component(entity, "footsteps", entity_query_local);
    struct component* audio_source = entity_get_component(entity, "audio_source", entity_query_local);
    if (footsteps == NULL || audio_source == NULL) {
        entity_mark_destroyed(entity);
        return;
    }

    this->footsteps_id = component_get_id((struct component*)footsteps);
    this->audio_source_id = component_get_id(audio_source);
    action_subscribe(footsteps_get_on_footstep(footsteps), this, handle_footstep, &this->footsteps_on_footstep_token);
}
static void footsteps_sounds_on_disable(struct component* base) {
    struct footsteps_sounds* this = (struct footsteps_sounds*)base;

    struct footsteps* footsteps = (struct footsteps*)scene_try_get_component(this->footsteps_id);
    if (footsteps) {
        action_unsubscribe(footsteps_get_on_footstep(footsteps), this->footsteps_on_footstep_token);
    }
}

const struct component_vtable footsteps_sounds_vtable = {
    .component_key = footsteps_sounds_component_key,
    .size = sizeof(struct footsteps_sounds),
    .on_awake = footsteps_sounds_awake,
    .on_disable = footsteps_sounds_on_disable
};


static void handle_footstep(void *listener, void *context) {
    struct footsteps_sounds* this = listener;
    struct footsteps_on_footstep_callback_context *cnt = context;

    struct audio_source* audio_source = (struct audio_source*)scene_try_get_component(this->audio_source_id);
    if (audio_source) {
        audio_source_play(audio_source, rigid_material_sounds_get_sound(rigid_surface_get_material_name(cnt->surface)), false);
    }
}
