//
// Created by Nikita on 25.09.2026.
//

#include "engine_lifecycle.h"

#include "../../msystems/msystem.h"


static struct {
    bool running;
    bool repeat_required;
} engine_lifecycle;


static void engine_lifecycle_on_enable(struct engine_arguments args) {
    engine_lifecycle.running = true;
    engine_lifecycle.repeat_required = false;
}

static void engine_lifecycle_on_disable(void) {
    engine_lifecycle.running = false;
}

const struct msystem engine_lifecycle_msystem = {
    .name = "engine_lifecycle",
    .on_enable = engine_lifecycle_on_enable,
    .on_disable = engine_lifecycle_on_disable
};

bool engine_lifecycle_is_running(void) {
    return engine_lifecycle.running;
}
bool engine_lifecycle_restart_required(void) {
    return engine_lifecycle.repeat_required;
}

void engine_lifecycle_mark_stop(void) {
    engine_lifecycle.running = false;
}
void engine_lifecycle_mark_restart(void) {
    engine_lifecycle.repeat_required = true;
}
