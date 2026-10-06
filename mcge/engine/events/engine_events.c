//
// Created by Nikita on 25.09.2026.
//

#include "engine_events.h"

#include "../../utils/action.h"
#include "../../msystems/msystem.h"


static struct {
    struct action pre_frame;

    struct action pre_physics;
    struct action on_physics;
    struct action post_physics;

    struct action pre_rendering;
    struct action on_rendering;
    struct action post_rendering;

    struct action on_native_event;
} engine_events;


static void engine_events_on_create(void) {
    engine_events.pre_frame = action_create();
    engine_events.pre_physics = action_create();
    engine_events.on_physics = action_create();
    engine_events.post_physics = action_create();
    engine_events.pre_rendering = action_create();
    engine_events.on_rendering = action_create();
    engine_events.post_rendering = action_create();
    engine_events.on_native_event = action_create();
}
static void engine_events_on_destroy(void) {
    action_destroy(&engine_events.pre_frame);
    action_destroy(&engine_events.pre_physics);
    action_destroy(&engine_events.on_physics);
    action_destroy(&engine_events.post_physics);
    action_destroy(&engine_events.pre_rendering);
    action_destroy(&engine_events.on_rendering);
    action_destroy(&engine_events.post_rendering);
    action_destroy(&engine_events.on_native_event);
}

static void engine_events_on_disable(void) {
    action_clear(&engine_events.pre_frame);
    action_clear(&engine_events.pre_physics);
    action_clear(&engine_events.on_physics);
    action_clear(&engine_events.post_physics);
    action_clear(&engine_events.pre_rendering);
    action_clear(&engine_events.on_rendering);
    action_clear(&engine_events.post_rendering);
    action_clear(&engine_events.on_native_event);
}

const struct msystem engine_events_msystem = {
    .name = "engine_events",
    .on_create = engine_events_on_create,
    .on_disable = engine_events_on_disable,
    .on_destroy = engine_events_on_destroy
};

struct action* engine_events_pre_frame(void) {
    return &engine_events.pre_frame;
}

struct action* engine_events_pre_physics(void) {
    return &engine_events.pre_physics;
}
struct action* engine_events_on_physics(void) {
    return &engine_events.on_physics;
}
struct action* engine_events_post_physics(void) {
    return &engine_events.post_physics;
}
struct action* engine_events_pre_rendering(void) {
    return &engine_events.pre_rendering;
}
struct action* engine_events_on_rendering(void) {
    return &engine_events.on_rendering;
}
struct action* engine_events_post_rendering(void) {
    return &engine_events.post_rendering;
}
struct action* engine_events_on_native_event(void) {
    return &engine_events.on_native_event;
}
