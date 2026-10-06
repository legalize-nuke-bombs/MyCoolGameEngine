//
// Created by nikita on 24.09.2026.
//

#include "profiler.h"

#include <stddef.h>

#include "time_estimator.h"
#include "../devices/keyboard.h"
#include "../logging/logger.h"
#include "../engine/events/engine_events.h"
#include "../utils/action.h"
#include "../msystems/msystem.h"


static struct {
    struct time_estimator *frame;
    struct time_estimator *update;
    struct time_estimator *rendering;

    struct action* on_hotkey;
    unsigned int on_hotkey_token;

    struct action* pre_frame;
    unsigned int pre_frame_token;

    struct action* pre_physics;
    unsigned int pre_physics_token;

    struct action* post_physics;
    unsigned int post_physics_token;

    struct action* pre_rendering;
    unsigned int pre_rendering_token;

    struct action* post_rendering;
    unsigned int post_rendering_token;
} profiler;


static void profiler_on_create(void) {
    profiler.frame = time_estimator_create();
    profiler.update = time_estimator_create();
    profiler.rendering = time_estimator_create();
}
static void profiler_on_destroy(void) {
    time_estimator_destroy(profiler.frame);
    time_estimator_destroy(profiler.update);
    time_estimator_destroy(profiler.rendering);
}




static void profiler_handle_hotkey_pressed(void *listener, void *context) {
    const double frame_ms = time_estimator_average_block_ms(profiler.frame);
    const double update_ms = time_estimator_average_block_ms(profiler.update);
    const double render_ms = time_estimator_average_block_ms(profiler.rendering);
    const double unknown_ms = frame_ms - update_ms - render_ms;
    const double update_ms_percent = update_ms / frame_ms * 100;
    const double render_ms_percent = render_ms / frame_ms * 100;
    const double unknown_ms_percent = unknown_ms / frame_ms * 100;

    const double fps = 1000 / frame_ms;

    logger_info("Profiler output. FPS: %f. Frame: %f ms (update %f ms %f%%, render %f ms %f%%, unknown %f ms %f%%)", fps, frame_ms, update_ms, update_ms_percent, render_ms, render_ms_percent, unknown_ms, unknown_ms_percent);
}

static void profiler_update(void) {
    time_estimator_update(profiler.frame);
    time_estimator_update(profiler.update);
    time_estimator_update(profiler.rendering);
}

static void profiler_handle_pre_frame(void *listener, void *context) {
    time_estimator_start_block(profiler.frame);
    profiler_update();
}
static void profiler_handle_pre_physics(void *listener, void *context) {
    time_estimator_start_block(profiler.update);
}
static void profiler_handle_post_physics(void *listener, void *context) {
    time_estimator_stop_block(profiler.update);
}
static void profiler_handle_pre_rendering(void *listener, void *context) {
    time_estimator_start_block(profiler.rendering);
}
static void profiler_handle_post_rendering(void *listener, void *context) {
    time_estimator_stop_block(profiler.rendering);
}

static void profiler_on_enable(struct engine_arguments args) {
    profiler.on_hotkey = keyboard_require_action_on_key_pressed("F3");
    action_subscribe(profiler.on_hotkey, NULL, profiler_handle_hotkey_pressed, &profiler.on_hotkey_token);

    profiler.pre_frame = engine_events_pre_frame();
    action_subscribe(profiler.pre_frame, NULL, profiler_handle_pre_frame, &profiler.pre_frame_token);

    profiler.pre_physics = engine_events_pre_physics();
    action_subscribe(profiler.pre_physics, NULL, profiler_handle_pre_physics, &profiler.pre_physics_token);

    profiler.post_physics = engine_events_post_physics();
    action_subscribe(profiler.post_physics, NULL, profiler_handle_post_physics, &profiler.post_physics_token);

    profiler.pre_rendering = engine_events_pre_rendering();
    action_subscribe(profiler.pre_rendering, NULL, profiler_handle_pre_rendering, &profiler.pre_rendering_token);

    profiler.post_rendering = engine_events_post_rendering();
    action_subscribe(profiler.post_rendering, NULL, profiler_handle_post_rendering, &profiler.post_rendering_token);
}

static void profiler_on_disable(void) {
    action_unsubscribe(profiler.on_hotkey, profiler.on_hotkey_token);
    profiler.on_hotkey = NULL;

    action_unsubscribe(profiler.pre_frame, profiler.pre_frame_token);
    profiler.pre_frame = NULL;

    action_unsubscribe(profiler.pre_physics, profiler.pre_physics_token);
    profiler.pre_physics = NULL;

    action_unsubscribe(profiler.post_physics, profiler.post_physics_token);
    profiler.post_physics = NULL;

    action_unsubscribe(profiler.pre_rendering, profiler.pre_rendering_token);
    profiler.pre_rendering = NULL;

    action_unsubscribe(profiler.post_rendering, profiler.post_rendering_token);
    profiler.post_rendering = NULL;
}

const struct msystem profiler_msystem = {
    .name = "profiler",
    .on_create = profiler_on_create,
    .on_enable = profiler_on_enable,
    .on_disable = profiler_on_disable,
    .on_destroy = profiler_on_destroy
};
