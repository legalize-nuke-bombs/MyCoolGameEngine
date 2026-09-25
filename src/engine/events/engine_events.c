//
// Created by Nikita on 25.09.2026.
//

#include "engine_events.h"

#include <stdlib.h>
#include "../../logging/logger.h"
#include "../../utils/action.h"


struct engine_events {
    struct action* pre_frame;

    struct action* pre_physics;
    struct action* on_physics;
    struct action* post_physics;

    struct action* pre_rendering;
    struct action* on_rendering;
    struct action* post_rendering;

    struct action* on_native_event;
};


struct engine_events* engine_events_create() {
    logger_info("Engine events are creating...");
    struct engine_events *this = malloc(sizeof(struct engine_events));
    this->pre_frame = action_create();
    this->pre_physics = action_create();
    this->on_physics = action_create();
    this->post_physics = action_create();
    this->pre_rendering = action_create();
    this->on_rendering = action_create();
    this->post_rendering = action_create();
    this->on_native_event = action_create();
    return this;
}
void engine_events_destroy(struct engine_events *this) {
    logger_info("Engine events are destroying...");
    action_destroy(this->pre_frame);
    action_destroy(this->pre_physics);
    action_destroy(this->on_physics);
    action_destroy(this->post_physics);
    action_destroy(this->pre_rendering);
    action_destroy(this->on_rendering);
    action_destroy(this->post_rendering);
    action_destroy(this->on_native_event);
    free(this);
}

void engine_events_awake(struct engine_events *this) {
    logger_info("Engine events are waking up...");
}
void engine_events_disable(struct engine_events *this) {
    logger_info("Engine events are disabling...");
    action_clear(this->pre_frame);
    action_clear(this->pre_physics);
    action_clear(this->on_physics);
    action_clear(this->post_physics);
    action_clear(this->pre_rendering);
    action_clear(this->on_rendering);
    action_clear(this->post_rendering);
    action_clear(this->on_native_event);
}

struct action* engine_events_pre_frame(const struct engine_events *this) {
    return this->pre_frame;
}

struct action* engine_events_pre_physics(const struct engine_events *this) {
    return this->pre_physics;
}
struct action* engine_events_on_physics(const struct engine_events *this) {
    return this->on_physics;
}
struct action* engine_events_post_physics(const struct engine_events *this) {
    return this->post_physics;
}
struct action* engine_events_pre_rendering(const struct engine_events *this) {
    return this->pre_rendering;
}
struct action* engine_events_on_rendering(const struct engine_events *this) {
    return this->on_rendering;
}
struct action* engine_events_post_rendering(const struct engine_events *this) {
    return this->post_rendering;
}
struct action* engine_events_on_native_event(struct engine_events *this) {
    return this->on_native_event;
}