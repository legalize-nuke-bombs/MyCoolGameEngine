//
// Created by nikita on 24.09.2026.
//

#include "profiler.h"

#include <stdlib.h>

#include "time_estimator.h"
#include "../devices/devices.h"
#include "../devices/keyboard.h"
#include "../engine/engine.h"
#include "../logging/logger.h"
#include "../engine/events/engine_events.h"
#include "../engine/engine_init_context.h"
#include "../utils/action.h"


struct profiler {
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

    struct engine *engine;
};

static void profiler_handle_hotkey_pressed(void *listener, void *context) {
    const struct profiler* this = listener;

    const double frame_ms = time_estimator_average_block_ms(this->frame);
    const double update_ms = time_estimator_average_block_ms(this->update);
    const double render_ms = time_estimator_average_block_ms(this->rendering);
    const double unknown_ms = frame_ms - update_ms - render_ms;
    const double update_ms_percent = update_ms / frame_ms * 100;
    const double render_ms_percent = render_ms / frame_ms * 100;
    const double unknown_ms_percent = unknown_ms / frame_ms * 100;

    const double fps = 1000 / frame_ms;

    logger_info("Profiler output. FPS: %f. Frame: %f ms (update %f%%, render %f%%, unknown %f%%)", fps, frame_ms, update_ms_percent, render_ms_percent, unknown_ms_percent);
}

static void profiler_update(const struct profiler *this) {
    time_estimator_update(this->frame);
    time_estimator_update(this->update);
    time_estimator_update(this->rendering);
}

static void profiler_handle_pre_frame(void *listener, void *context) {
    const struct profiler* this = listener;
    time_estimator_start_block(this->frame);
    profiler_update(this);
}
static void profiler_handle_pre_physics(void *listener, void *context) {
    const struct profiler* this = listener;
    time_estimator_start_block(this->update);
}
static void profiler_handle_post_physics(void *listener, void *context) {
    const struct profiler* this = listener;
    time_estimator_stop_block(this->update);
}
static void profiler_handle_pre_rendering(void *listener, void *context) {
    const struct profiler* this = listener;
    time_estimator_start_block(this->rendering);
}
static void profiler_handle_post_rendering(void *listener, void *context) {
    const struct profiler* this = listener;
    time_estimator_stop_block(this->rendering);
}

static void profiler_subscribe(struct profiler *this) {
    this->on_hotkey = keyboard_require_action_on_key_pressed(devices_get_keyboard(engine_init_context_get_devices(engine_get_init_context(this->engine))), "F3");
    action_subscribe(this->on_hotkey, this, profiler_handle_hotkey_pressed, &this->on_hotkey_token);

    const struct engine_events* events = engine_init_context_get_events(engine_get_init_context(this->engine));

    this->pre_frame = engine_events_pre_frame(events);
    action_subscribe(this->pre_frame, this, profiler_handle_pre_frame, &this->pre_frame_token);

    this->pre_physics = engine_events_pre_physics(events);
    action_subscribe(this->pre_physics, this, profiler_handle_pre_physics, &this->pre_physics_token);

    this->post_physics = engine_events_post_physics(events);
    action_subscribe(this->post_physics, this, profiler_handle_post_physics, &this->post_physics_token);

    this->pre_rendering = engine_events_pre_rendering(events);
    action_subscribe(this->pre_rendering, this, profiler_handle_pre_rendering, &this->pre_rendering_token);

    this->post_rendering = engine_events_post_rendering(events);
    action_subscribe(this->post_rendering, this, profiler_handle_post_rendering, &this->post_rendering_token);
}

static void profiler_unsubscribe(struct profiler *this) {
    action_unsubscribe(this->on_hotkey, this->on_hotkey_token);
    this->on_hotkey = NULL;

    action_unsubscribe(this->pre_frame, this->pre_frame_token);
    this->pre_frame = NULL;

    action_unsubscribe(this->pre_physics, this->pre_physics_token);
    this->pre_physics = NULL;

    action_unsubscribe(this->post_physics, this->post_physics_token);
    this->post_physics = NULL;

    action_unsubscribe(this->pre_rendering, this->pre_rendering_token);
    this->pre_rendering = NULL;

    action_unsubscribe(this->post_rendering, this->post_rendering_token);
    this->post_rendering = NULL;
}

struct profiler* profiler_create(struct engine *engine) {
    logger_info("Profiler is creating...");
    struct profiler* this = malloc(sizeof(struct profiler));
    this->frame = time_estimator_create();
    this->update = time_estimator_create();
    this->rendering = time_estimator_create();
    this->engine = engine;
    return this;
}
void profiler_destroy(struct profiler* this) {
    logger_info("Profiler is destroying...");
    profiler_unsubscribe(this);
    time_estimator_destroy(this->frame);
    time_estimator_destroy(this->update);
    time_estimator_destroy(this->rendering);
    free(this);
}

void profiler_awake(struct profiler *this) {
    logger_info("Profiler is awaking...");
    profiler_subscribe(this);
}

