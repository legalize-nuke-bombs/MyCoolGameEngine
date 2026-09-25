//
// Created by nikita on 24.09.2026.
//

#include "profiler.h"

#include <stdlib.h>

#include "time_estimator.h"
#include "../devices/keyboard.h"
#include "../logging/logger.h"
#include "../engine/events/engine_events.h"
#include "../utils/action.h"
#include "../subsystems/subsystem_internal.h"


struct profiler {
    struct subsystem base;

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
};


static const char* profiler_get_name() {
    return "profiler";
}
static void profiler_on_destroy(struct subsystem* base);
static void profiler_on_enable(struct subsystem* base, struct engine_arguments args);
static void profiler_on_disable(struct subsystem* base);

static struct subsystem_vtable profiler_vtable = {
    .name = profiler_get_name,
    .on_destroy = profiler_on_destroy,
    .on_enable = profiler_on_enable,
    .on_disable = profiler_on_disable
};



struct subsystem* profiler_create(const struct subsystem_collection *subsystems) {
    struct profiler* this = calloc(1, sizeof(struct profiler));
    struct subsystem* base = (struct subsystem*)this;
    subsystem_create(base, &profiler_vtable, subsystems);
    this->frame = time_estimator_create();
    this->update = time_estimator_create();
    this->rendering = time_estimator_create();
    return base;
}
void profiler_on_destroy(struct subsystem *base) {
    const struct profiler* this = (struct profiler*)(base);
    time_estimator_destroy(this->frame);
    time_estimator_destroy(this->update);
    time_estimator_destroy(this->rendering);
}




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
    struct keyboard *keyboard = (struct keyboard*)subsystem_get_subsystem((struct subsystem*)this, "keyboard");
    this->on_hotkey = keyboard_require_action_on_key_pressed(keyboard, "F3");
    action_subscribe(this->on_hotkey, this, profiler_handle_hotkey_pressed, &this->on_hotkey_token);

    const struct engine_events *events = (struct engine_events*)subsystem_get_subsystem((struct subsystem*)this, "engine_events");

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

void profiler_on_enable(struct subsystem *base, struct engine_arguments args) {
    struct profiler* this = (struct profiler*)(base);
    profiler_subscribe(this);
}
void profiler_on_disable(struct subsystem *base) {
    struct profiler* this = (struct profiler*)(base);
    profiler_unsubscribe(this);
}

