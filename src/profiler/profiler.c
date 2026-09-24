//
// Created by nikita on 24.09.2026.
//

#include "profiler.h"

#include <stdlib.h>

#include "time_estimator.h"
#include "../logging/logger.h"


struct profiler {
    struct time_estimator *frame;
    struct time_estimator *update;
    struct time_estimator *rendering;
};

struct profiler* profiler_create() {
    logger_info("Profiler is creating...");
    struct profiler* this = malloc(sizeof(struct profiler));
    this->frame = time_estimator_create();
    this->update = time_estimator_create();
    this->rendering = time_estimator_create();
    return this;
}
void profiler_destroy(struct profiler* this) {
    logger_info("Profiler is destroying...");
    time_estimator_destroy(this->frame);
    time_estimator_destroy(this->update);
    time_estimator_destroy(this->rendering);
    free(this);
}

struct time_estimator* profiler_get_frame_estimator(const struct profiler* this) {
    return this->frame;
}
struct time_estimator* profiler_get_update_estimator(const struct profiler* this) {
    return this->update;
}
struct time_estimator* profiler_get_rendering_estimator(const struct profiler* this) {
    return this->rendering;
}

