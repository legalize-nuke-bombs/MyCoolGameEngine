//
// Created by Nikita on 04.10.2026.
//

#include "fear_balls.h"

#include <math.h>
#include <stdlib.h>

#include "fear_ball.h"
#include "../../skill_internal.h"
#include "../../../../../utils/fields.h"
#include "../../../../../scene/prefabs/prefab.h"
#include "../../../../../scene/prefabs/prefabs.h"
#include "../../../../../logging/logger.h"
#include "../../../../../scene/entity.h"
#include "../../../../../scene/scene.h"

struct fear_balls {
    struct skill base;
    char* prefab_name;
    struct prefab* prefab;
    int fear_balls_num;
};

static void fear_balls_on_create(struct skill* base, struct fields *fields);
static void fear_balls_on_destroy(struct skill* base);
static void fear_balls_on_enable(struct skill* base);
static bool fear_balls_invoke(struct skill* base);

const struct skill_vtable fear_balls_vtable = {
    .key = SKILL_FEAR_BALLS_KEY,
    .size = sizeof(struct fear_balls),
    .on_create = fear_balls_on_create,
    .on_destroy = fear_balls_on_destroy,
    .on_enable = fear_balls_on_enable,
    .on_invoke = fear_balls_invoke
};


static void fear_balls_on_create(struct skill* base, struct fields *fields) {
    struct fear_balls* this = (struct fear_balls*)base;
    this->prefab_name = fields_dup_string(fields, "prefab", NULL);
    this->fear_balls_num = fields_get_int(fields, "count", 0);
}

static void fear_balls_on_destroy(struct skill* base) {
    const struct fear_balls* this = (struct fear_balls*)base;
    free(this->prefab_name);
}

static void fear_balls_on_enable(struct skill* base) {
    struct fear_balls* this = (struct fear_balls*)base;
    if (this->prefab_name) {
        this->prefab = prefabs_get(this->prefab_name);
        free(this->prefab_name);
        this->prefab_name = NULL;
    }
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