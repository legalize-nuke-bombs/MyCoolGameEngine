//
// Created by nikita on 12.10.2026.
//

#include "sound_on_kill.h"
#include <mcge/mcge.h>

#include "damager.h"


struct sound_on_kill {
    struct component base;

    struct sound *sound;

    uint128_t damager_id;
    uint128_t audio_source_id;
    unsigned int damager_on_kill_token;
};

const char* sound_on_kill_component_key(void) {
    return "sound_on_kill";
}

static void sound_on_kill_on_create(struct component* base, struct fields *fields) {
    struct sound_on_kill *this = (struct sound_on_kill *) base;
    const char* sound_name = fields_get_string(fields, "sound", "<null>");
    this->sound = sounds_get(sound_name);
}

static void handle_kill(void *listener, void *context);
static void sound_on_kill_awake(struct component* base) {
    struct sound_on_kill *this = (struct sound_on_kill *) base;
    struct entity* entity = component_get_parent(base);

    struct damager* damager = (struct damager*)entity_get_component(entity, "damager", entity_query_local);
    struct audio_source* audio_source = (struct audio_source*)entity_get_component(entity, "audio_source", entity_query_local);
    if (damager == NULL || audio_source == NULL) {
        entity_mark_destroyed(entity);
        return;
    }
    this->damager_id = component_get_id((struct component*)damager);
    action_subscribe(damager_on_kill(damager), this, handle_kill, &this->damager_on_kill_token);
    this->audio_source_id = component_get_id((struct component*)audio_source);
}
static void sound_on_kill_on_disable(struct component* base) {
    struct sound_on_kill *this = (struct sound_on_kill *) base;
    struct damager* damager = (struct damager*)scene_try_get_component(this->damager_id);
    if (damager) {
        action_unsubscribe(damager_on_kill(damager), this->damager_on_kill_token);
    }
}

const struct component_vtable sound_on_kill_vtable = {
    .component_key = sound_on_kill_component_key,
    .size = sizeof(struct sound_on_kill),
    .on_create = sound_on_kill_on_create,
    .on_awake = sound_on_kill_awake,
    .on_disable = sound_on_kill_on_disable
};


static void handle_kill(void *listener, void *context) {
    struct sound_on_kill *this = listener;

    struct audio_source* audio_source = (struct audio_source*)scene_try_get_component(this->audio_source_id);
    if (audio_source) {
        audio_source_play(audio_source, this->sound, false);
    }
}