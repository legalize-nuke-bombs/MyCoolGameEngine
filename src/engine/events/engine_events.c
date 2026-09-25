//
// Created by Nikita on 25.09.2026.
//

#include "engine_events.h"

#include <stdlib.h>
#include "../../utils/action.h"
#include "../../subsystems/subsystem_internal.h"


struct engine_events {
    struct subsystem base;

    struct action* pre_frame;

    struct action* pre_physics;
    struct action* on_physics;
    struct action* post_physics;

    struct action* pre_rendering;
    struct action* on_rendering;
    struct action* post_rendering;

    struct action* on_native_event;
};

static const char* engine_events_get_name() {
    return "engine_events";
}

static void engine_events_on_destroy(struct subsystem* base);
static void engine_events_on_enable(struct subsystem* base);
static void engine_events_on_disable(struct subsystem* base);


static struct subsystem_vtable engine_events_vtable = {
    .name = engine_events_get_name,
    .on_destroy = engine_events_on_destroy,
    .on_enable = engine_events_on_enable,
    .on_disable = engine_events_on_disable
};



struct subsystem* engine_events_create(const struct subsystem_collection *subsystems) {
    struct engine_events *this = malloc(sizeof(struct engine_events));
    struct subsystem* base = (struct subsystem*)this;
    subsystem_create(base, &engine_events_vtable, subsystems);
    this->pre_frame = action_create();
    this->pre_physics = action_create();
    this->on_physics = action_create();
    this->post_physics = action_create();
    this->pre_rendering = action_create();
    this->on_rendering = action_create();
    this->post_rendering = action_create();
    this->on_native_event = action_create();
    return base;
}
void engine_events_on_destroy(struct subsystem* base) {
    struct engine_events *this = (struct engine_events*)base;
    action_destroy(this->pre_frame);
    action_destroy(this->pre_physics);
    action_destroy(this->on_physics);
    action_destroy(this->post_physics);
    action_destroy(this->pre_rendering);
    action_destroy(this->on_rendering);
    action_destroy(this->post_rendering);
    action_destroy(this->on_native_event);
}

void engine_events_on_enable(struct subsystem* base) {
    struct engine_events *this = (struct engine_events*)base;
}
void engine_events_on_disable(struct subsystem* base) {
    const struct engine_events *this = (struct engine_events*)base;
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