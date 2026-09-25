//
// Created by Nikita on 25.09.2026.
//

#include "engine_lifecycle.h"

#include <stddef.h>
#include <stdlib.h>

#include "../../subsystems/subsystem_internal.h"


static const char* engine_lifecycle_get_name() {
    return "engine_lifecycle";
}

static void engine_lifecycle_on_enable(struct subsystem* base, struct engine_arguments args);
static void engine_lifecycle_on_disable(struct subsystem* base);


static struct subsystem_vtable engine_lifecycle_vtable = {
    .name = engine_lifecycle_get_name,
    .on_destroy = NULL,
    .on_enable = engine_lifecycle_on_enable,
    .on_disable = engine_lifecycle_on_disable
};


struct engine_lifecycle {
    struct subsystem base;

    bool running;
    bool repeat_required;
};


struct subsystem* engine_lifecycle_create(const struct subsystem_collection* collections) {
    struct engine_lifecycle* this = calloc(1, sizeof(struct engine_lifecycle));
    struct subsystem* base = (struct subsystem*)this;
    subsystem_create(base, &engine_lifecycle_vtable, collections);
    return base;
}

static void engine_lifecycle_on_enable(struct subsystem* base, struct engine_arguments args) {
    struct engine_lifecycle* this = (struct engine_lifecycle*)base;
    this->running = true;
    this->repeat_required = false;
}

static void engine_lifecycle_on_disable(struct subsystem* base) {
    struct engine_lifecycle* this = (struct engine_lifecycle*)base;
    this->running = false;
}

bool engine_lifecycle_is_running(const struct engine_lifecycle *this) {
    return this->running;
}
bool engine_lifecycle_restart_required(const struct engine_lifecycle *this) {
    return this->repeat_required;
}

void engine_lifecycle_mark_stop(struct engine_lifecycle *this) {
    this->running = false;
}
void engine_lifecycle_mark_restart(struct engine_lifecycle *this) {
    this->repeat_required = true;
}
