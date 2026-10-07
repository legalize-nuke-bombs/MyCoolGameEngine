//
// Created by nikita on 07.10.2026.
//

#include "box_renderer_rigid_directed.h"
#include "../component_internal.h"
#include "mcge/scene/entity.h"
#include "mcge/scene/scene.h"
#include "mcge/scene/components/physics/rigid_body.h"
#include "mcge/utils/action.h"
#include "mcge/utils/vector2_math.h"
#include <math.h>

#include "box_renderer.h"
#include "mcge/logging/logger.h"


struct box_renderer_rigid_directed {
    struct component base;

    uint128_t box_renderer_id;

    uint128_t rigid_body_id;
    unsigned int rigid_body_on_drive_token;
};

#define BOX_RENDERER_RIGID_DIRECTED_MIN_SPEED_TO_TURN 0.5

static void box_renderer_rigid_directed_on_create(struct component *base, struct fields *fields);
static void box_renderer_rigid_directed_awake(struct component *base);
static void box_renderer_rigid_directed_on_disable(struct component *base);

const struct component_vtable box_renderer_rigid_directed_vtable = {
    .component_key = box_renderer_rigid_directed_component_key,
    .size = sizeof(struct box_renderer_rigid_directed),
    .on_create = box_renderer_rigid_directed_on_create,
    .on_awake = box_renderer_rigid_directed_awake,
    .on_disable = box_renderer_rigid_directed_on_disable,
};

const char* box_renderer_rigid_directed_component_key(void) {
    return "box_renderer_rigid_directed";
}

static void box_renderer_rigid_directed_on_create(struct component *base, struct fields *fields) {
    struct box_renderer_rigid_directed *this = (struct box_renderer_rigid_directed*) base;
}

static void handle_rigid_body_drive(void *listener, void *context);

static void box_renderer_rigid_directed_awake(struct component *base) {
    struct box_renderer_rigid_directed *this = (struct box_renderer_rigid_directed *) base;
    struct entity *entity = component_get_parent(base);

    struct box_renderer* box_renderer = (struct box_renderer*)entity_get_component(entity, "box_renderer", entity_query_local);
    if (box_renderer == NULL) {
        entity_mark_destroyed(entity);
        return;
    }
    struct rigid_body* rigid_body = (struct rigid_body*)entity_get_component(entity, "rigid_body", entity_query_local);
    if (rigid_body == NULL) {
        entity_mark_destroyed(entity);
        return;
    }

    this->box_renderer_id = component_get_id((struct component*)box_renderer);
    this->rigid_body_id = component_get_id((struct component*)rigid_body);
    action_subscribe(rigid_body_get_on_drive(rigid_body), this, handle_rigid_body_drive, &this->rigid_body_on_drive_token);
}

static void box_renderer_rigid_directed_on_disable(struct component *base) {
    struct box_renderer_rigid_directed *this = (struct box_renderer_rigid_directed *) base;
    struct rigid_body *rigid_body = (struct rigid_body*)scene_try_get_component(this->rigid_body_id);
    if (rigid_body) {
        action_unsubscribe(rigid_body_get_on_drive(rigid_body), this->rigid_body_on_drive_token);
    }
}

static void handle_rigid_body_drive(void *listener, void *context) {
    struct box_renderer_rigid_directed* this = listener;
    const struct vector2 *target_velocity = context;

    if (fabs(target_velocity->x) < BOX_RENDERER_RIGID_DIRECTED_MIN_SPEED_TO_TURN) {
        return;
    }

    const struct box_renderer* box_renderer = (struct box_renderer*)scene_try_get_component(this->box_renderer_id);
    if (box_renderer == NULL) {
        return;
    }

    const bool flip = box_renderer_get_flip_x(box_renderer);
    const bool new_flip = target_velocity->x < 0;
    if (new_flip != flip) {
        logger_debug("Entity %s x-flipped to %d via box_renderer_rigid_directed", component_get_global_parent_name((const struct component*)this), new_flip);
        box_renderer_set_flip_x(box_renderer, new_flip);
    }
}