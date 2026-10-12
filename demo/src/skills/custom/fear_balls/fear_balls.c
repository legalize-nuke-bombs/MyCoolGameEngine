//
// Created by Nikita on 04.10.2026.
//

#include "fear_balls.h"

#include <math.h>
#include <stdlib.h>

#include "fear_ball.h"
#include "../../skill_internal.h"
#include <mcge/mcge.h>

struct fear_balls {
    struct skill base;

    struct prefab* prefab;
    struct sound* cast_sound;
    struct sound* invoke_sound;
    int fear_balls_num;

    uint128_t audio_source_id;
};

static void fear_balls_on_create(struct skill* base, struct fields *fields);
static void fear_balls_on_destroy(struct skill* base);
static void fear_balls_on_enable(struct skill* base);
static bool fear_balls_cast(struct skill* base);
static bool fear_balls_invoke(struct skill* base);

const struct skill_vtable fear_balls_vtable = {
    .key = SKILL_FEAR_BALLS_KEY,
    .size = sizeof(struct fear_balls),
    .on_create = fear_balls_on_create,
    .on_destroy = fear_balls_on_destroy,
    .on_enable = fear_balls_on_enable,
    .on_cast = fear_balls_cast,
    .on_invoke = fear_balls_invoke
};


static void fear_balls_on_create(struct skill* base, struct fields *fields) {
    struct fear_balls* this = (struct fear_balls*)base;

    const char* prefab_name = fields_get_string(fields, "prefab", NULL);
    const char* cast_sound_name = fields_get_string(fields, "cast_sound", NULL);
    const char* invoke_sound_name = fields_get_string(fields, "invoke_sound", NULL);
    if (prefab_name) this->prefab = prefabs_get(prefab_name);
    if (cast_sound_name) this->cast_sound = sounds_get(cast_sound_name);
    if (invoke_sound_name) this->invoke_sound = sounds_get(invoke_sound_name);
    this->fear_balls_num = fields_get_int(fields, "count", 0);
}

static void fear_balls_on_destroy(struct skill* base) {
    const struct fear_balls* this = (struct fear_balls*)base;
}

static void fear_balls_on_enable(struct skill* base) {
    struct fear_balls* this = (struct fear_balls*)base;
    const struct component* audio_source = entity_get_component(skill_self(base), "audio_source", entity_query_local);
    if (audio_source) this->audio_source_id = component_get_id(audio_source);
}

static bool fear_balls_cast(struct skill* base) {
    const struct fear_balls* this = (struct fear_balls*)base;
    struct audio_source* audio_source = (struct audio_source*)scene_try_get_component(this->audio_source_id);
    if (audio_source) {
        audio_source_play(audio_source, this->cast_sound, false);
    }
    return true;
}
static bool fear_balls_invoke(struct skill* base) {
    const struct fear_balls* this = (struct fear_balls*)base;
    if (this->prefab == NULL) {
        logger_warn("Failed to invoke fear balls, prefab is null");
        return false;
    }
    if (!prefab_contains_component(this->prefab, "fear_ball", entity_query_local)) {
        logger_warn("Failed to invoke fear balls, prefab does not contain fear_ball component");
        return false;
    }
    struct audio_source* audio_source = (struct audio_source*)scene_try_get_component(this->audio_source_id);
    if (audio_source) {
        audio_source_play(audio_source, this->invoke_sound, false);
    }
    for (int i = 0; i < this->fear_balls_num; i++) {
        struct entity *entity = prefab_instantiate(this->prefab);
        struct rect rect = entity_get_local_rect(entity);
        rect.position = entity_get_local_rect(skill_self(base)).position;
        entity_set_local_rect(entity, rect);

        struct fear_ball* fear_ball = (struct fear_ball*)entity_get_component(entity, "fear_ball", entity_query_local);
        const double angle = (2.0f * M_PI * i) / this->fear_balls_num;
        struct vector2 direction = {
            .x = cosl(angle),
            .y = sinl(angle)
        };
        fear_ball_launch(skill_self(base), fear_ball, direction);

        scene_capture_entity(entity);
    }
    return true;
}