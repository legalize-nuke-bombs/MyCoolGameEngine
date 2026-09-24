//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_PROFILER_H
#define MYCOOLGAMEENGINE_PROFILER_H

struct profiler;
struct time_estimator;

struct profiler* profiler_create();
void profiler_destroy(struct profiler* this);

struct time_estimator* profiler_get_frame_estimator(const struct profiler* this);
struct time_estimator* profiler_get_update_estimator(const struct profiler* this);
struct time_estimator* profiler_get_rendering_estimator(const struct profiler* this);

#endif //MYCOOLGAMEENGINE_PROFILER_H
