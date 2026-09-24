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

void profiler_update(const struct profiler* this) {
    time_estimator_update(this->frame);
    time_estimator_update(this->update);
    time_estimator_update(this->rendering);
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

void profiler_log(const struct profiler* this) {
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

